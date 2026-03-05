import sqlite3
import os

DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "screen_data.db")

def check_database():
    if not os.path.exists(DB_PATH):
        print(f"❌ 数据库文件不存在: {DB_PATH}")
        return
    
    try:
        conn = sqlite3.connect(DB_PATH)
        cursor = conn.cursor()
        
        # 检查表结构
        cursor.execute('PRAGMA table_info(usage_log)')
        columns = cursor.fetchall()
        print("数据库表结构:")
        print("-" * 50)
        for col in columns:
            print(f"列名: {col[1]}, 类型: {col[2]}, 主键: {col[5]}")
        
        # 检查数据量
        cursor.execute("SELECT COUNT(*) FROM usage_log")
        total_rows = cursor.fetchone()[0]
        print(f"\n总记录数: {total_rows}")
        
        # 检查日期范围
        cursor.execute("SELECT MIN(date), MAX(date) FROM usage_log")
        min_date, max_date = cursor.fetchone()
        print(f"日期范围: {min_date} 到 {max_date}")
        
        # 检查最近几天的数据
        cursor.execute("SELECT date, COUNT(*) as apps, SUM(duration) as total_seconds FROM usage_log GROUP BY date ORDER BY date DESC LIMIT 5")
        recent_days = cursor.fetchall()
        print("\n最近5天数据:")
        print("-" * 50)
        for date, apps, seconds in recent_days:
            hours = seconds // 3600
            minutes = (seconds % 3600) // 60
            print(f"{date}: {apps}个应用, 总时长: {hours}小时{minutes}分")
        
        conn.close()
        
    except Exception as e:
        print(f"❌ 检查数据库时出错: {e}")

if __name__ == "__main__":
    check_database()