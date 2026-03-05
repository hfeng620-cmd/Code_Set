#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
测试文件夹扫描功能
"""

import os
import sys

# 添加当前目录到路径，以便导入 Video_Player_Perfect
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

# 模拟 Video_Player_Perfect 中的视频扩展名
video_extensions = {'.mp4', '.avi', '.wmv', '.mkv', '.mov', '.flv', '.m4v', '.webm', 
                    '.mpg', '.mpeg', '.m2v', '.m4v', '.3gp', '.3g2', '.f4v', '.f4p', 
                    '.f4a', '.f4b', '.rm', '.rmvb', '.asf', '.ts', '.mts', '.m2ts'}

def scan_folder_for_videos(folder_path):
    """扫描指定文件夹中的视频文件"""
    if not os.path.isdir(folder_path):
        print(f"错误：文件夹不存在: {folder_path}")
        return []
    
    video_files = []
    try:
        for filename in os.listdir(folder_path):
            filepath = os.path.join(folder_path, filename)
            if os.path.isfile(filepath):
                ext = os.path.splitext(filename)[1].lower()
                if ext in video_extensions:
                    video_files.append(filepath)
                    print(f"找到视频文件: {filename}")
    except Exception as e:
        print(f"扫描文件夹时出错: {e}")
    
    return video_files

def test_specific_folder():
    """测试用户提到的特定文件夹"""
    test_folder = r"E:\Python_Process\python\month01\01 PYTHON_CORE"
    
    print(f"测试文件夹: {test_folder}")
    print("=" * 60)
    
    # 检查文件夹是否存在
    if not os.path.exists(test_folder):
        print(f"文件夹不存在: {test_folder}")
        print("请检查路径是否正确")
        return
    
    if not os.path.isdir(test_folder):
        print(f"路径不是文件夹: {test_folder}")
        return
    
    # 扫描视频文件
    videos = scan_folder_for_videos(test_folder)
    
    print("=" * 60)
    if videos:
        print(f"找到 {len(videos)} 个视频文件:")
        for i, video in enumerate(videos, 1):
            print(f"{i:2d}. {os.path.basename(video)}")
    else:
        print("未找到视频文件")
        print("可能的原因:")
        print("1. 文件夹中没有视频文件")
        print("2. 视频文件扩展名不在支持列表中")
        print("3. 权限问题无法访问文件夹")
        
        # 列出文件夹中的文件类型
        print("\n文件夹中的文件类型:")
        try:
            extensions_found = set()
            for filename in os.listdir(test_folder):
                filepath = os.path.join(test_folder, filename)
                if os.path.isfile(filepath):
                    ext = os.path.splitext(filename)[1].lower()
                    extensions_found.add(ext)
            
            if extensions_found:
                print("找到的扩展名:")
                for ext in sorted(extensions_found):
                    if ext:  # 忽略无扩展名的文件
                        print(f"  {ext}")
            else:
                print("  文件夹为空")
        except Exception as e:
            print(f"  无法列出文件: {e}")

if __name__ == "__main__":
    test_specific_folder()
    
    # 等待用户输入
    input("\n按 Enter 键退出...")