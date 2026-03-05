import sqlite3
import datetime
import os
import sys

# --- 配置 ---
# 自动寻找同目录下的数据库文件
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, "screen_data.db")

def format_time(seconds):
    """把秒数转换成易读格式"""
    m, s = divmod(seconds, 60)
    h, m = divmod(m, 60)
    if h > 0:
        return f"{int(h)}小时 {int(m)}分 {int(s)}秒"
    else:
        return f"{int(m)}分 {int(s)}秒"

def get_db_connection():
    if not os.path.exists(DB_PATH):
        print(f"❌ 错误：找不到数据库文件！\n请确认此脚本是否与 screen_tracker.pyw 在同一目录下。")
        sys.exit(1)
    return sqlite3.connect(DB_PATH)

def show_daily():
    conn = get_db_connection()
    cursor = conn.cursor()
    today = datetime.date.today().isoformat()
    
    print(f"\n====== 📅 今日报表 ({today}) ======")
    cursor.execute("SELECT app_name, duration FROM usage_log WHERE date = ? ORDER BY duration DESC", (today,))
    rows = cursor.fetchall()
    conn.close()

    if not rows:
        print("今日暂无记录。")
        return

    print(f"{'应用名称':<25} | {'使用时长'}")
    print("-" * 45)
    total_seconds = 0
    for name, seconds in rows:
        total_seconds += seconds
        # 处理中文对齐问题的简单方案：限制长度
        display_name = (name[:20] + '..') if len(name) > 20 else name
        # <25 表示左对齐占25格
        # 注意：cmd中中文字符宽度不同，完美对齐较难，这里做基础对齐
        print(f"{display_name:<25} | {format_time(seconds)}")
    
    print("-" * 45)
    print(f"{'总计':<25} | {format_time(total_seconds)}")

def show_monthly():
    conn = get_db_connection()
    cursor = conn.cursor()
    today = datetime.date.today()
    start_month = today.replace(day=1).isoformat()
    
    print(f"\n====== 🗓️ 本月报表 ({today.strftime('%Y-%m')}) ======")
    cursor.execute('''
        SELECT app_name, SUM(duration) FROM usage_log 
        WHERE date >= ? GROUP BY app_name ORDER BY SUM(duration) DESC
    ''', (start_month,))
    rows = cursor.fetchall()
    conn.close()

    if not rows:
        print("本月暂无记录。")
        return

    print(f"{'应用名称':<25} | {'累计时长'}")
    print("-" * 45)
    total_seconds = 0
    for name, seconds in rows:
        total_seconds += seconds
        display_name = (name[:20] + '..') if len(name) > 20 else name
        print(f"{display_name:<25} | {format_time(seconds)}")
    
    print("-" * 45)
    print(f"{'本月总计':<25} | {format_time(total_seconds)}")

def show_yearly(year=None):
    conn = get_db_connection()
    cursor = conn.cursor()
    today = datetime.date.today()
    if year is None:
        year = today.year
    try:
        year = int(year)
    except Exception:
        print("无效的年份格式，使用当前年份")
        year = today.year

    start_year = datetime.date(year, 1, 1).isoformat()
    end_year = datetime.date(year, 12, 31).isoformat()

    print(f"\n====== 📈 年度报表 ({year}) ======")
    cursor.execute('''
        SELECT app_name, SUM(duration) FROM usage_log
        WHERE date >= ? AND date <= ?
        GROUP BY app_name ORDER BY SUM(duration) DESC
    ''', (start_year, end_year))
    rows = cursor.fetchall()
    conn.close()

    if not rows:
        print(f"{year} 年暂无记录。")
        return

    print(f"{'应用名称':<25} | {'累计时长'}")
    print("-" * 45)
    total_seconds = 0
    for name, seconds in rows:
        total_seconds += seconds
        display_name = (name[:20] + '..') if len(name) > 20 else name
        print(f"{display_name:<25} | {format_time(seconds)}")

    print("-" * 45)
    print(f"{'年度总计':<25} | {format_time(total_seconds)}")

def search_app(keyword):
    conn = get_db_connection()
    cursor = conn.cursor()
    
    print(f"\n====== 🔍 搜索应用: {keyword} ======")
    # 模糊搜索
    cursor.execute('''
        SELECT date, app_name, duration FROM usage_log 
        WHERE app_name LIKE ? ORDER BY date DESC
    ''', (f'%{keyword}%',))
    rows = cursor.fetchall()
    conn.close()

    if not rows:
        print(f"未找到包含 '{keyword}' 的应用记录。")
        return

    print(f"{'日期':<15} | {'应用名称':<20} | {'时长'}")
    print("-" * 55)
    total_search = 0
    for date, name, seconds in rows:
        total_search += seconds
        print(f"{date:<15} | {name:<20} | {format_time(seconds)}")
    
    print("-" * 55)
    print(f"该应用历史总计使用: {format_time(total_search)}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        # 如果没有参数，显示交互菜单
        while True:
            print("\n请选择查询模式：")
            print("1. 今日报表")
            print("2. 本月报表")
            print("3. 搜索特定应用")
            print("4. 年度报表")
            print("q. 退出")
            choice = input("输入选项 (1/2/3/q): ").strip()
            
            if choice == '1': show_daily()
            elif choice == '2': show_monthly()
            elif choice == '3': 
                kw = input("请输入应用名关键词 (如 Chrome): ")
                search_app(kw)
            elif choice == '4':
                y = input("请输入年份 (如 2025，留空使用当前年): ").strip()
                show_yearly(y if y else None)
            elif choice.lower() == 'q': break
            else: print("无效输入")
    else:
        # 命令行参数模式
        mode = sys.argv[1]
        if mode == 'today':
            show_daily()
        elif mode == 'month':
            show_monthly()
        elif mode == 'search' and len(sys.argv) > 2:
            search_app(sys.argv[2])
        elif mode == 'year':
            if len(sys.argv) > 2:
                show_yearly(sys.argv[2])
            else:
                show_yearly()
        else:
            print("用法: python show_report.py [today | month | search <名字>]")