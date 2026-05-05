#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
屏幕使用数据查询工具
在 Screen_Tracker 目录下右键打开终端，运行此脚本查询数据
"""

import os
import sys
import sqlite3
import datetime
from datetime import timedelta

def format_time(seconds):
    """格式化时间显示"""
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

def get_today_data(db_path):
    """获取今日数据"""
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
    conn.close()
    return data

def get_week_data(db_path):
    """获取本周数据"""
    today = datetime.date.today()
    start_week = today - timedelta(days=today.weekday())  # 本周一
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        WHERE date >= ? 
        GROUP BY app_name 
        ORDER BY total_duration DESC
    """, (start_week.isoformat(),))
    
    data = cursor.fetchall()
    conn.close()
    return data

def get_month_data(db_path):
    """获取本月数据"""
    today = datetime.date.today()
    start_month = today.replace(day=1)  # 本月1日
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        WHERE date >= ? 
        GROUP BY app_name 
        ORDER BY total_duration DESC
    """, (start_month.isoformat(),))
    
    data = cursor.fetchall()
    conn.close()
    return data

def get_all_time_stats(db_path):
    """获取所有时间统计"""
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    # 总使用时间
    cursor.execute("SELECT SUM(duration) FROM usage_log")
    total_seconds = cursor.fetchone()[0] or 0
    
    # 总记录数
    cursor.execute("SELECT COUNT(*) FROM usage_log")
    total_records = cursor.fetchone()[0]
    
    # 最早和最晚记录日期
    cursor.execute("SELECT MIN(date), MAX(date) FROM usage_log")
    min_date, max_date = cursor.fetchone()
    
    # 使用最多的应用
    cursor.execute("""
        SELECT app_name, SUM(duration) as total_duration 
        FROM usage_log 
        GROUP BY app_name 
        ORDER BY total_duration DESC 
        LIMIT 5
    """)
    top_apps = cursor.fetchall()
    
    conn.close()
    
    return {
        'total_seconds': total_seconds,
        'total_records': total_records,
        'min_date': min_date,
        'max_date': max_date,
        'top_apps': top_apps
    }

def print_data(title, data, limit=15):
    """打印数据"""
    print(f"\n{'='*60}")
    print(f"📊 {title}")
    print('='*60)
    
    if not data:
        print("暂无数据")
        return
    
    total_seconds = sum(row[1] for row in data)
    print(f"总使用时间: {format_time(total_seconds)}")
    print(f"应用数量: {len(data)}")
    print("\n排名 | 应用名称                | 使用时间")
    print("-" * 50)
    
    for i, (app_name, duration) in enumerate(data[:limit], 1):
        time_str = format_time(duration)
        # 截断过长的应用名称
        display_name = app_name[:20] + "..." if len(app_name) > 23 else app_name.ljust(23)
        print(f"{i:3} | {display_name} | {time_str}")
    
    if len(data) > limit:
        print(f"... 还有 {len(data) - limit} 个应用未显示")

def main():
    """主函数"""
    print("🖥️  威震天屏幕统计 - 数据查询工具")
    print("="*60)
    
    # 数据库路径
    db_path = "screen_data.db"
    
    if not os.path.exists(db_path):
        print(f"错误: 找不到数据库文件 {db_path}")
        print("请确保在 Screen_Tracker 目录下运行此脚本")
        input("\n按 Enter 键退出...")
        return
    
    # 检查数据库
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
    
    # 所有时间统计
    print("📈 数据库统计信息")
    print("-"*40)
    stats = get_all_time_stats(db_path)
    print(f"📅 数据范围: {stats['min_date']} 至 {stats['max_date']}")
    print(f"📊 总记录数: {stats['total_records']:,} 条")
    print(f"⏱️  总使用时间: {format_time(stats['total_seconds'])}")
    
    if stats['top_apps']:
        print("\n🏆 历史使用最多的应用:")
        for i, (app_name, duration) in enumerate(stats['top_apps'], 1):
            print(f"  {i}. {app_name[:20]:20} - {format_time(duration)}")
    
    # 今日数据
    today_data = get_today_data(db_path)
    print_data("今日使用统计", today_data)
    
    # 本周数据
    week_data = get_week_data(db_path)
    print_data("本周使用统计", week_data)
    
    # 本月数据
    month_data = get_month_data(db_path)
    print_data("本月使用统计", month_data)
    
    # 使用说明
    print("\n" + "="*60)
    print("💡 使用说明:")
    print("1. 在 Screen_Tracker 目录下右键打开终端")
    print("2. 运行: python 查询屏幕数据.py")
    print("3. 或双击此脚本文件")
    print("\n⚙️  其他功能:")
    print("   - 双击 ScreenTracker.pyw 启动屏幕统计工具")
    print("   - 双击 更新开机自启.bat 更新开机自启动")
    print("   - 系统托盘右键菜单可查看图表报表")
    
    input("\n按 Enter 键退出...")

if __name__ == "__main__":
    main()