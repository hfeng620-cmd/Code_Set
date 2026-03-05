#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
诊断播放器扫描问题
"""

import os
import sys

# 模拟 Video_Player_Perfect 中的视频扩展名
video_extensions = {'.mp4', '.avi', '.wmv', '.mkv', '.mov', '.flv', '.m4v', '.webm', 
                    '.mpg', '.mpeg', '.m2v', '.m4v', '.3gp', '.3g2', '.f4v', '.f4p', 
                    '.f4a', '.f4b', '.rm', '.rmvb', '.asf', '.ts', '.mts', '.m2ts'}

def simulate_player_scan(folder_path):
    """模拟播放器的扫描逻辑"""
    print(f"模拟播放器扫描: {folder_path}")
    print("=" * 60)
    
    if not os.path.isdir(folder_path):
        print(f"错误：文件夹不存在: {folder_path}")
        return []
    
    # 模拟播放器的扫描逻辑（从 refresh_video_list 方法）
    seen_paths = set()
    seen_filenames = set()
    video_files = []
    
    try:
        for filename in os.listdir(folder_path):
            filepath = os.path.join(folder_path, filename)
            if os.path.isfile(filepath):
                ext = os.path.splitext(filename)[1].lower()
                if ext in video_extensions:
                    # 规范化路径
                    try:
                        real_path = os.path.realpath(filepath)
                        normalized_path = os.path.normpath(real_path).lower()
                    except:
                        normalized_path = os.path.normpath(os.path.abspath(filepath)).lower()
                    
                    # 检查路径是否已存在
                    if normalized_path in seen_paths:
                        print(f"  跳过重复路径: {filename}")
                        continue
                    
                    # 规范化文件名
                    filename_normalized = filename.lower().strip()
                    
                    # 检查文件名是否已存在
                    if filename_normalized in seen_filenames:
                        print(f"  跳过重复文件名: {filename}")
                        continue
                    
                    # 添加到集合中
                    seen_paths.add(normalized_path)
                    seen_filenames.add(filename_normalized)
                    video_files.append(filepath)
                    print(f"  找到视频文件: {filename} (扩展名: {ext})")
    except Exception as e:
        print(f"扫描文件夹时出错: {e}")
    
    return video_files

def test_ts_files():
    """专门测试 .ts 文件"""
    print("\n专门测试 .ts 文件支持:")
    print("=" * 60)
    
    # 检查 .ts 是否在扩展名列表中
    if '.ts' in video_extensions:
        print("✓ .ts 扩展名在支持列表中")
    else:
        print("✗ .ts 扩展名不在支持列表中")
    
    # 创建一个测试 .ts 文件路径
    test_ts_file = r"E:\Python_Process\python\month01\01 PYTHON_CORE\python_core_day07_pm.ts"
    
    if os.path.exists(test_ts_file):
        print(f"✓ 测试文件存在: {test_ts_file}")
        
        # 检查文件扩展名
        ext = os.path.splitext(test_ts_file)[1].lower()
        print(f"  文件扩展名: {ext}")
        
        if ext in video_extensions:
            print(f"  ✓ 扩展名 {ext} 在支持列表中")
        else:
            print(f"  ✗ 扩展名 {ext} 不在支持列表中")
    else:
        print(f"✗ 测试文件不存在: {test_ts_file}")

def main():
    """主函数"""
    test_folder = r"E:\Python_Process\python\month01\01 PYTHON_CORE"
    
    # 检查文件夹是否存在
    if not os.path.exists(test_folder):
        print(f"文件夹不存在: {test_folder}")
        print("请检查路径是否正确")
        return
    
    if not os.path.isdir(test_folder):
        print(f"路径不是文件夹: {test_folder}")
        return
    
    # 模拟播放器扫描
    videos = simulate_player_scan(test_folder)
    
    print("=" * 60)
    if videos:
        print(f"播放器扫描找到 {len(videos)} 个视频文件:")
        for i, video in enumerate(videos, 1):
            print(f"{i:2d}. {os.path.basename(video)}")
    else:
        print("播放器扫描未找到视频文件")
        
        # 列出文件夹中的所有文件
        print("\n文件夹中的所有文件:")
        try:
            all_files = []
            for filename in os.listdir(test_folder):
                filepath = os.path.join(test_folder, filename)
                if os.path.isfile(filepath):
                    all_files.append(filename)
            
            if all_files:
                print(f"找到 {len(all_files)} 个文件:")
                for i, filename in enumerate(all_files, 1):
                    ext = os.path.splitext(filename)[1].lower()
                    print(f"{i:2d}. {filename} (扩展名: {ext})")
            else:
                print("  文件夹为空")
        except Exception as e:
            print(f"  无法列出文件: {e}")
    
    # 测试 .ts 文件支持
    test_ts_files()
    
    # 检查路径编码问题
    print("\n检查路径编码问题:")
    print("=" * 60)
    try:
        # 尝试访问文件夹
        file_count = 0
        for filename in os.listdir(test_folder):
            filepath = os.path.join(test_folder, filename)
            if os.path.isfile(filepath):
                file_count += 1
                ext = os.path.splitext(filename)[1].lower()
                if ext == '.ts':
                    print(f"  可以访问 .ts 文件: {filename}")
                    break
        
        if file_count > 0:
            print(f"✓ 可以访问文件夹，包含 {file_count} 个文件")
        else:
            print("✗ 文件夹为空或无法访问")
    except Exception as e:
        print(f"✗ 访问文件夹时出错: {e}")

if __name__ == "__main__":
    main()
    
    # 等待用户输入
    input("\n按 Enter 键退出...")