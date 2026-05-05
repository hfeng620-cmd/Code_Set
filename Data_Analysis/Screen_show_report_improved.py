import sqlite3
import datetime
import os
import sys
from contextlib import contextmanager

# --- 配置 ---
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, "screen_data.db")

@contextmanager
def get_db_connection():
    """数据库连接上下文管理器"""
    if not os.path.exists(DB_PATH):
        print(f"❌ 错误：找不到数据库文件！")
        print(f"请确认此脚本是否与 screen_tracker.pyw 在同一目录下。")
        print(f"预期路径: {DB_PATH}")
        sys.exit(1)
    
    conn = sqlite3.connect(DB_PATH)
    try:
        yield conn
    finally:
        conn.close()

def format_time(seconds):
    """把秒数转换成易读格式"""
    seconds = int(seconds)
    m, s = divmod(seconds, 60)
    h, m = divmod(m, 60)
    d, h = divmod(h, 24)
    
    parts = []
    if d > 0:
        parts.append(f"{d}天")
    if h > 0:
        parts.append(f"{h}小时")
    if m > 0:
        parts.append(f"{m}分")
    if s > 0 or not parts:  # 如果没有其他部分，至少显示秒
        parts.append(f"{s}秒")
    
    return " ".join(parts)

def display_table(title, rows, show_total=True):
    """通用表格显示函数"""
    if not rows:
        print("暂无记录。")
        return 0
    
    print(f"\n{title}")
    print("=" * 60)
    
    # 动态计算列宽
    max_name_len = max(len(str(row[0])) for row in rows)
    max_name_len = min(max_name_len, 30)  # 限制最大宽度
    
    # 表头
    header_name = "应用名称"
    header_time = "使用时长"
    print(f"{header_name:<{max_name_len}} | {header_time}")
    print("-" * (max_name_len + 3 + 20))
    
    total_seconds = 0
    for name, seconds in rows:
        total_seconds += seconds
        # 处理长名称
        display_name = name
        if len(name) > max_name_len:
            display_name = name[:max_name_len-2] + ".."
        
        time_str = format_time(seconds)
        print(f"{display_name:<{max_name_len}} | {time_str}")
    
    if show_total:
        print("-" * (max_name_len + 3 + 20))
        total_str = format_time(total_seconds)
        print(f"{'总计':<{max_name_len}} | {total_str}")
    
    return total_seconds

def show_daily(date_str=None):
    """显示指定日期的报表，默认今天"""
    if date_str is None:
        date_str = datetime.date.today().isoformat()
    
    try:
        # 验证日期格式
        datetime.date.fromisoformat(date_str)
    except ValueError:
        print(f"❌ 无效的日期格式: {date_str}，请使用 YYYY-MM-DD 格式")
        return
    
    with get_db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("""
            SELECT app_name, duration 
            FROM usage_log 
            WHERE date = ? 
            ORDER BY duration DESC
        """, (date_str,))
        rows = cursor.fetchall()
    
    title = f"📅 日报表 ({date_str})"
    display_table(title, rows)

def show_monthly(year=None, month=None):
    """显示指定月份的报表，默认当前月"""
    today = datetime.date.today()
    if year is None:
        year = today.year
    if month is None:
        month = today.month
    
    try:
        start_date = datetime.date(year, month, 1)
        if month == 12:
            end_date = datetime.date(year + 1, 1, 1) - datetime.timedelta(days=1)
        else:
            end_date = datetime.date(year, month + 1, 1) - datetime.timedelta(days=1)
    except ValueError as e:
        print(f"❌ 无效的年份或月份: {e}")
        return
    
    with get_db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("""
            SELECT app_name, SUM(duration) as total_duration
            FROM usage_log 
            WHERE date >= ? AND date <= ?
            GROUP BY app_name 
            ORDER BY total_duration DESC
        """, (start_date.isoformat(), end_date.isoformat()))
        rows = cursor.fetchall()
    
    title = f"🗓️ 月报表 ({year}-{month:02d})"
    display_table(title, rows)

def show_yearly(year=None):
    """显示指定年份的报表，默认当前年"""
    today = datetime.date.today()
    if year is None:
        year = today.year
    
    try:
        year = int(year)
        if year < 2000 or year > 2100:
            print("❌ 年份应在 2000-2100 之间")
            return
    except (ValueError, TypeError):
        print("❌ 无效的年份格式，使用当前年份")
        year = today.year
    
    start_date = datetime.date(year, 1, 1)
    end_date = datetime.date(year, 12, 31)
    
    with get_db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("""
            SELECT app_name, SUM(duration) as total_duration
            FROM usage_log 
            WHERE date >= ? AND date <= ?
            GROUP BY app_name 
            ORDER BY total_duration DESC
        """, (start_date.isoformat(), end_date.isoformat()))
        rows = cursor.fetchall()
    
    title = f"📈 年报表 ({year})"
    display_table(title, rows)

def search_app(keyword, case_sensitive=False):
    """搜索应用使用记录"""
    if not keyword.strip():
        print("❌ 请输入搜索关键词")
        return
    
    with get_db_connection() as conn:
        cursor = conn.cursor()
        if case_sensitive:
            query = f"%{keyword}%"
            cursor.execute("""
                SELECT date, app_name, duration 
                FROM usage_log 
                WHERE app_name LIKE ? 
                ORDER BY date DESC, duration DESC
            """, (query,))
        else:
            query = f"%{keyword.lower()}%"
            cursor.execute("""
                SELECT date, app_name, duration 
                FROM usage_log 
                WHERE LOWER(app_name) LIKE ? 
                ORDER BY date DESC, duration DESC
            """, (query,))
        rows = cursor.fetchall()
    
    if not rows:
        print(f"🔍 未找到包含 '{keyword}' 的应用记录。")
        return
    
    print(f"\n🔍 搜索结果: '{keyword}'")
    print("=" * 70)
    print(f"{'日期':<12} | {'应用名称':<25} | {'时长':<20}")
    print("-" * 70)
    
    total_seconds = 0
    for date, name, seconds in rows:
        total_seconds += seconds
        display_name = name[:23] + ".." if len(name) > 25 else name
        time_str = format_time(seconds)
        print(f"{date:<12} | {display_name:<25} | {time_str:<20}")
    
    print("-" * 70)
    total_str = format_time(total_seconds)
    print(f"{'总计':<12} | {'':<25} | {total_str:<20}")

def show_summary():
    """显示数据概览"""
    with get_db_connection() as conn:
        cursor = conn.cursor()
        
        # 总记录数
        cursor.execute("SELECT COUNT(*) FROM usage_log")
        total_records = cursor.fetchone()[0]
        
        # 日期范围
        cursor.execute("SELECT MIN(date), MAX(date) FROM usage_log")
        min_date, max_date = cursor.fetchone()
        
        # 应用总数
        cursor.execute("SELECT COUNT(DISTINCT app_name) FROM usage_log")
        total_apps = cursor.fetchone()[0]
        
        # 总使用时间
        cursor.execute("SELECT SUM(duration) FROM usage_log")
        total_seconds = cursor.fetchone()[0] or 0
    
    print("\n📊 数据概览")
    print("=" * 40)
    print(f"📅 日期范围: {min_date} 到 {max_date}")
    print(f"📝 总记录数: {total_records} 条")
    print(f"📱 应用总数: {total_apps} 个")
    print(f"⏱️  总使用时间: {format_time(total_seconds)}")
    
    # 最近7天使用情况
    print("\n📈 最近7天使用情况:")
    print("-" * 40)
    
    with get_db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("""
            SELECT date, COUNT(*) as apps, SUM(duration) as total_seconds
            FROM usage_log 
            GROUP BY date 
            ORDER BY date DESC 
            LIMIT 7
        """)
        recent_days = cursor.fetchall()
    
    for date, apps, seconds in recent_days:
        time_str = format_time(seconds)
        print(f"{date}: {apps:2d}个应用, {time_str}")

def interactive_menu():
    """交互式菜单"""
    while True:
        print("\n" + "=" * 50)
        print("📊 屏幕使用时间统计报表系统")
        print("=" * 50)
        print("1. 今日报表")
        print("2. 指定日期报表")
        print("3. 本月报表")
        print("4. 指定月份报表")
        print("5. 年度报表")
        print("6. 搜索应用")
        print("7. 数据概览")
        print("8. 退出")
        print("-" * 50)
        
        choice = input("请选择 (1-8): ").strip()
        
        if choice == '1':
            show_daily()
        elif choice == '2':
            date_str = input("请输入日期 (YYYY-MM-DD): ").strip()
            show_daily(date_str)
        elif choice == '3':
            show_monthly()
        elif choice == '4':
            try:
                year = int(input("请输入年份 (如 2025): ").strip())
                month = int(input("请输入月份 (1-12): ").strip())
                show_monthly(year, month)
            except ValueError:
                print("❌ 请输入有效的数字")
        elif choice == '5':
            year = input("请输入年份 (留空使用当前年): ").strip()
            show_yearly(year if year else None)
        elif choice == '6':
            keyword = input("请输入应用名关键词: ").strip()
            case_sensitive = input("是否区分大小写? (y/N): ").strip().lower() == 'y'
            search_app(keyword, case_sensitive)
        elif choice == '7':
            show_summary()
        elif choice == '8':
            print("👋 再见！")
            break
        else:
            print("❌ 无效选择，请重新输入")
        
        input("\n按 Enter 键继续...")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        # 交互模式
        interactive_menu()
    else:
        # 命令行模式
        mode = sys.argv[1].lower()
        
        if mode == 'today' or mode == 'daily':
            if len(sys.argv) > 2:
                show_daily(sys.argv[2])
            else:
                show_daily()
        elif mode == 'month' or mode == 'monthly':
            if len(sys.argv) > 3:
                try:
                    year = int(sys.argv[2])
                    month = int(sys.argv[3])
                    show_monthly(year, month)
                except ValueError:
                    print("❌ 月份参数格式错误")
            else:
                show_monthly()
        elif mode == 'year' or mode == 'yearly':
            if len(sys.argv) > 2:
                show_yearly(sys.argv[2])
            else:
                show_yearly()
        elif mode == 'search':
            if len(sys.argv) > 2:
                search_app(sys.argv[2])
            else:
                print("❌ 请提供搜索关键词")
                print("用法: python show_report.py search <关键词>")
        elif mode == 'summary':
            show_summary()
        elif mode == 'help':
            print("""
用法: python Screen_show_report_improved.py [命令] [参数]
            
命令:
  today [日期]     显示今日或指定日期报表 (日期格式: YYYY-MM-DD)
  month [年 月]   显示本月或指定月份报表
  year [年份]     显示今年或指定年份报表
  search <关键词> 搜索应用使用记录
  summary         显示数据概览
  help            显示此帮助信息
            
示例:
  python Screen_show_report_improved.py today
  python Screen_show_report_improved.py today 2026-01-19
  python Screen_show_report_improved.py month 2026 1
  python Screen_show_report_improved.py search Chrome
  python Screen_show_report_improved.py summary
            """)
        else:
            print("❌ 未知命令")
            print("使用 'help' 查看可用命令")