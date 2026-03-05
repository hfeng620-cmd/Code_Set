#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Interactive Screen Data Query Tool
Run: python screen_data_query_interactive.py
Then press number keys to view different statistics
"""

import os
import sys
import sqlite3
import datetime
from datetime import timedelta

def format_time(seconds):
    """Format time display"""
    seconds = int(seconds)
    hours = seconds // 3600
    minutes = (seconds % 3600) // 60
    secs = seconds % 60
    
    if hours > 0:
        return f"{hours}小时{minutes}分{secs}秒"
    elif minutes > 0:
        return f"{minutes}分{secs}秒"
    else:
        return f"{secs}秒"

def clear_screen():
    """Clear terminal screen"""
    os.system('cls' if os.name == 'nt' else 'clear')

def print_header():
    """Print application header"""
    print("=" * 80)
    print("🖥️  威震天屏幕统计 - 交互式数据查询工具")
    print("=" * 80)

def print_menu():
    """Print main menu"""
    print("\n请选择要查看的统计内容：")
    print("=" * 80)
    print("1. 📊 历史各个应用总使用时长")
    print("2. 📅 今日各应用使用时长")
    print("3. 📅 本周各应用使用时长")
    print("4. 📅 本月各应用使用时长")
    print("5. 📅 今年各应用使用时长")
    print("6. 🔍 查询特定应用使用历史")
    print("7. 📈 查看数据库概览")
    print("0. 🚪 退出程序")
    print("=" * 80)
    print("提示：输入数字后按 Enter 键")

def get_all_time_stats(db_path):
    """Get all-time statistics"""
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("SELECT SUM(duration) FROM usage_log")
    total_seconds = cursor.fetchone()[0] or 0
    
    cursor.execute("SELECT COUNT(*) FROM usage_log")
    total_records = cursor.fetchone()[0]
    
    cursor.execute("SELECT MIN(date), MAX(date) FROM usage_log")
    min_date, max_date = cursor.fetchone()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        GROUP BY app_name 
        ORDER BY total_duration DESC
    """)
    all_time_apps = cursor.fetchall()
    
    conn.close()
    
    return {
        'total_seconds': total_seconds,
        'total_records': total_records,
        'min_date': min_date,
        'max_date': max_date,
        'all_time_apps': all_time_apps
    }

def get_daily_stats(db_path):
    """Get today's statistics"""
    today = datetime.date.today().isoformat()
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        WHERE date = ? 
        GROUP BY app_name 
        ORDER BY total_duration DESC
    """, (today,))
    
    data = cursor.fetchall()
    total_seconds = sum(row[1] for row in data) if data else 0
    
    conn.close()
    
    return {
        'date': today,
        'total_seconds': total_seconds,
        'apps': data
    }

def get_weekly_stats(db_path):
    """Get this week's statistics"""
    today = datetime.date.today()
    week_start = today - timedelta(days=today.weekday())
    week_end = week_start + timedelta(days=6)
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        WHERE date >= ? AND date <= ?
        GROUP BY app_name 
        ORDER BY total_duration DESC
    """, (week_start.isoformat(), week_end.isoformat()))
    
    data = cursor.fetchall()
    total_seconds = sum(row[1] for row in data) if data else 0
    
    conn.close()
    
    return {
        'week_start': week_start.isoformat(),
        'week_end': week_end.isoformat(),
        'total_seconds': total_seconds,
        'apps': data
    }

def get_monthly_stats(db_path):
    """Get this month's statistics"""
    today = datetime.date.today()
    month_start = today.replace(day=1)
    
    if today.month == 12:
        month_end = datetime.date(today.year + 1, 1, 1) - timedelta(days=1)
    else:
        month_end = datetime.date(today.year, today.month + 1, 1) - timedelta(days=1)
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        WHERE date >= ? AND date <= ?
        GROUP BY app_name 
        ORDER BY total_duration DESC
    """, (month_start.isoformat(), month_end.isoformat()))
    
    data = cursor.fetchall()
    total_seconds = sum(row[1] for row in data) if data else 0
    
    conn.close()
    
    return {
        'month': today.strftime("%Y-%m"),
        'month_start': month_start.isoformat(),
        'month_end': month_end.isoformat(),
        'total_seconds': total_seconds,
        'apps': data
    }

def get_yearly_stats(db_path):
    """Get this year's statistics"""
    today = datetime.date.today()
    year_start = datetime.date(today.year, 1, 1)
    year_end = datetime.date(today.year, 12, 31)
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        WHERE date >= ? AND date <= ?
        GROUP BY app_name 
        ORDER BY total_duration DESC
    """, (year_start.isoformat(), year_end.isoformat()))
    
    data = cursor.fetchall()
    total_seconds = sum(row[1] for row in data) if data else 0
    
    conn.close()
    
    return {
        'year': today.year,
        'year_start': year_start.isoformat(),
        'year_end': year_end.isoformat(),
        'total_seconds': total_seconds,
        'apps': data
    }

def get_app_history(db_path, app_name):
    """Get usage history for specific application"""
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT date, SUM(duration) as daily_duration
        FROM usage_log 
        WHERE app_name = ?
        GROUP BY date
        ORDER BY date DESC
        LIMIT 30
    """, (app_name,))
    
    daily_usage = cursor.fetchall()
    
    cursor.execute("""
        SELECT strftime('%Y-%m', date) as month, SUM(duration) as monthly_duration
        FROM usage_log 
        WHERE app_name = ?
        GROUP BY month
        ORDER BY month DESC
    """, (app_name,))
    
    monthly_usage = cursor.fetchall()
    
    cursor.execute("SELECT SUM(duration) FROM usage_log WHERE app_name = ?", (app_name,))
    total_duration = cursor.fetchone()[0] or 0
    
    conn.close()
    
    return {
        'app_name': app_name,
        'total_duration': total_duration,
        'daily_usage': daily_usage,
        'monthly_usage': monthly_usage
    }

def print_stats(title, stats, limit=20):
    """Print statistics"""
    clear_screen()
    print_header()
    
    print(f"\n📊 {title}")
    print("=" * 80)
    
    if 'apps' in stats and stats['apps']:
        apps = stats['apps']
        total_seconds = stats['total_seconds']
        
        print(f"总使用时间: {format_time(total_seconds)}")
        print(f"应用数量: {len(apps)}")
        
        if 'date' in stats:
            print(f"日期: {stats['date']}")
        elif 'week_start' in stats:
            print(f"周期: {stats['week_start']} 至 {stats['week_end']}")
        elif 'month' in stats:
            print(f"月份: {stats['month']} ({stats['month_start']} 至 {stats['month_end']})")
        elif 'year' in stats:
            print(f"年份: {stats['year']}年 ({stats['year_start']} 至 {stats['year_end']})")
        
        print("\n排名 | 应用名称                      | 使用时间          | 占比")
        print("-" * 80)
        
        for i, (app_name, duration) in enumerate(apps[:limit], 1):
            time_str = format_time(duration)
            percentage = (duration / total_seconds * 100) if total_seconds > 0 else 0
            
            # 截断过长的应用名称
            if len(app_name) > 25:
                display_name = app_name[:22] + "..."
            else:
                display_name = app_name.ljust(25)
            
            print(f"{i:4} | {display_name} | {time_str:15} | {percentage:5.1f}%")
        
        if len(apps) > limit:
            print(f"\n... 还有 {len(apps) - limit} 个应用未显示")
    else:
        print("暂无数据")
    
    print("\n" + "=" * 80)
    input("按 Enter 键返回主菜单...")

def print_app_history(app_history):
    """Print application history"""
    clear_screen()
    print_header()
    
    print(f"\n🔍 应用使用历史: {app_history['app_name']}")
    print("=" * 80)
    
    total_duration = app_history['total_duration']
    print(f"总使用时间: {format_time(total_duration)}")
    
    # 每日使用记录
    daily_usage = app_history['daily_usage']
    if daily_usage:
        print(f"\n📅 每日使用记录 (最近{len(daily_usage)}天):")
        print("-" * 80)
        for date, duration in daily_usage:
            print(f"  {date}: {format_time(duration)}")
    
    # 每月使用记录
    monthly_usage = app_history['monthly_usage']
    if monthly_usage:
        print(f"\n📅 每月使用记录:")
        print("-" * 80)
        for month, duration in monthly_usage:
            print(f"  {month}: {format_time(duration)}")
    
    print("\n" + "=" * 80)
    input("按 Enter 键返回主菜单...")

def print_database_overview(db_path):
    """Print database overview"""
    clear_screen()
    print_header()
    
    print("\n📈 数据库概览")
    print("=" * 80)
    
    try:
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()
        
        # 基本统计
        cursor.execute("SELECT COUNT(*) FROM usage_log")
        total_records = cursor.fetchone()[0]
        
        cursor.execute("SELECT SUM(duration) FROM usage_log")
        total_seconds = cursor.fetchone()[0] or 0
        
        cursor.execute("SELECT MIN(date), MAX(date) FROM usage_log")
        min_date, max_date = cursor.fetchone()
        
        # 应用数量
        cursor.execute("SELECT COUNT(DISTINCT app_name) FROM usage_log")
        app_count = cursor.fetchone()[0]
        
        # 日期数量
        cursor.execute("SELECT COUNT(DISTINCT date) FROM usage_log")
        date_count = cursor.fetchone()[0]
        
        # 今日记录数
        today = datetime.date.today().isoformat()
        cursor.execute("SELECT COUNT(*) FROM usage_log WHERE date = ?", (today,))
        today_records = cursor.fetchone()[0]
        
        conn.close()
        
        print(f"📊 总记录数: {total_records:,} 条")
        print(f"⏱️  总使用时间: {format_time(total_seconds)}")
        print(f"📅 数据范围: {min_date} 至 {max_date}")
        print(f"📱 应用数量: {app_count} 个")
        print(f"📅 记录天数: {date_count} 天")
        print(f"📅 今日记录: {today_records} 条")
        
        # 计算平均每日使用时间
        if date_count > 0:
            avg_daily = total_seconds / date_count
            print(f"📊 平均每日: {format_time(avg_daily)}")
        
    except Exception as e:
        print(f"数据库错误: {e}")
    
    print("\n" + "=" * 80)
    input("按 Enter 键返回主菜单...")

def get_app_names(db_path):
    """Get list of all application names"""
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration
        FROM usage_log 
        GROUP BY app_name 
        ORDER BY total_duration DESC
        LIMIT 50
    """)
    
    apps = cursor.fetchall()
    conn.close()
    
    return apps

def select_application(db_path):
    """Let user select an application"""
    clear_screen()
    print_header()
    
    print("\n🔍 选择要查询的应用")
    print("=" * 80)
    
    apps = get_app_names(db_path)
    
    if not apps:
        print("数据库中暂无应用数据")
        input("\n按 Enter 键返回主菜单...")
        return None
    
    print("排名 | 应用名称                      | 总使用时间")
    print("-" * 80)
    
    for i, (app_name, duration) in enumerate(apps[:30], 1):
        time_str = format_time(duration)
        
        if len(app_name) > 25:
            display_name = app_name[:22] + "..."
        else:
            display_name = app_name.ljust(25)
        
        print(f"{i:4} | {display_name} | {time_str}")
    
    print("\n" + "=" * 80)
    print("输入应用编号 (1-30)，或输入应用名称，或输入 0 返回")
    
    while True:
        choice = input("\n请选择: ").strip()
        
        if choice == '0':
            return None
        
        # 如果是数字，选择对应的应用
        if choice.isdigit():
            index = int(choice) - 1
            if 0 <= index < len(apps[:30]):
                return apps[index][0]
            else:
                print("编号无效，请重新输入")
        else:
            # 如果是文本，直接使用
            return choice

def main():
    """Main interactive function"""
    # Database path
    db_path = "screen_data.db"
    
    if not os.path.exists(db_path):
        print(f"错误: 找不到数据库文件 {db_path}")
        print("请确保在 Screen_Tracker 目录下运行此脚本")
        input("\n按 Enter 键退出...")
        return
    
    # Check database
    try:
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='usage_log'")
        if not cursor.fetchone():
            print("错误: 数据库中找不到 usage_log 表")
            conn.close()
            input("\n按 Enter 键退出...")
            return
        conn.close()
    except Exception as e:
        print(f"数据库错误: {e}")
        input("\n按 Enter 键退出...")
        return
    
    while True:
        clear_screen()
        print_header()
        print_menu()
        
        choice = input("\n请输入选择 (0-7): ").strip()
        
        if choice == '0':
            clear_screen()
            print_header()
            print("\n感谢使用威震天屏幕统计查询工具！")
            print("=" * 80)
            break
            
        elif choice == '1':
            # 历史各个应用总使用时长
            stats = get_all_time_stats(db_path)
            print_stats("历史各个应用总使用时长", {
                'total_seconds': stats['total_seconds'],
                'apps': stats['all_time_apps']
            })
            
        elif choice == '2':
            # 今日各应用使用时长
            stats = get_daily_stats(db_path)
            print_stats("今日各应用使用时长", stats)
            
        elif choice == '3':
            # 本周各应用使用时长
            stats = get_weekly_stats(db_path)
            print_stats("本周各应用使用时长", stats)
            
        elif choice == '4':
            # 本月各应用使用时长
            stats = get_monthly_stats(db_path)
            print_stats("本月各应用使用时长", stats)
            
        elif choice == '5':
            # 今年各应用使用时长
            stats = get_yearly_stats(db_path)
            print_stats("今年各应用使用时长", stats)
            
        elif choice == '6':
            # 查询特定应用使用历史
            app_name = select_application(db_path)
            if app_name:
                app_history = get_app_history(db_path, app_name)
                print_app_history(app_history)
                
        elif choice == '7':
            # 查看数据库概览
            print_database_overview(db_path)
            
        else:
            print("\n❌ 无效选择，请重新输入")
            input("按 Enter 键继续...")

if __name__ == "__main__":
    main()