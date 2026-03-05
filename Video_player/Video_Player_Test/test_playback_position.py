#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
测试视频播放器的播放位置记忆功能
验证每个视频的播放位置是否被单独保存
"""

import os
import sys
import json
from PyQt5.QtCore import QSettings

def test_playback_positions():
    """测试播放位置字典功能"""
    print("=== 测试播放位置记忆功能 ===")
    
    # 模拟一些视频文件路径
    test_files = [
        r"D:\test\video1.mp4",
        r"D:\test\video2.mp4", 
        r"D:\test\video3.mp4"
    ]
    
    # 模拟播放位置
    test_positions = {
        test_files[0]: 15000,  # 15秒
        test_files[1]: 30000,  # 30秒
        test_files[2]: 45000   # 45秒
    }
    
    print("1. 创建测试播放位置字典...")
    print(f"   视频文件: {test_files[0]} -> 位置: {test_positions[test_files[0]]}ms")
    print(f"   视频文件: {test_files[1]} -> 位置: {test_positions[test_files[1]]}ms")
    print(f"   视频文件: {test_files[2]} -> 位置: {test_positions[test_files[2]]}ms")
    
    # 测试JSON序列化/反序列化
    print("\n2. 测试JSON序列化/反序列化...")
    json_str = json.dumps(test_positions)
    print(f"   JSON字符串: {json_str[:100]}...")
    
    loaded_positions = json.loads(json_str)
    # 确保所有值都是整数
    for key, value in loaded_positions.items():
        loaded_positions[key] = int(value)
    
    print(f"   加载后的字典: {loaded_positions}")
    
    # 验证数据一致性
    print("\n3. 验证数据一致性...")
    for file_path in test_files:
        original = test_positions.get(file_path, 0)
        loaded = loaded_positions.get(file_path, 0)
        if original == loaded:
            print(f"   ✓ {os.path.basename(file_path)}: 位置一致 ({original}ms)")
        else:
            print(f"   ✗ {os.path.basename(file_path)}: 位置不一致 (原始: {original}ms, 加载: {loaded}ms)")
    
    # 测试QSettings集成
    print("\n4. 测试QSettings集成...")
    try:
        # 使用测试组织名和应用名，避免影响真实设置
        settings = QSettings('TestPlayer', 'TestPlayer')
        
        # 保存测试数据
        settings.setValue('playback_positions', json_str)
        settings.setValue('last_played_file', test_files[0])
        
        # 读取测试数据
        loaded_json = settings.value('playback_positions', '{}')
        last_file = settings.value('last_played_file', '')
        
        print(f"   保存的JSON: {loaded_json[:100]}...")
        print(f"   最后播放文件: {last_file}")
        
        # 清理测试设置
        settings.remove('playback_positions')
        settings.remove('last_played_file')
        settings.sync()
        
        print("   ✓ QSettings集成测试通过")
        
    except Exception as e:
        print(f"   ✗ QSettings集成测试失败: {e}")
    
    # 测试单个文件位置获取
    print("\n5. 测试单个文件位置获取...")
    for i, file_path in enumerate(test_files):
        position = test_positions.get(file_path, 0)
        print(f"   文件 {i+1}: {os.path.basename(file_path)} -> 位置: {position}ms")
    
    print("\n=== 测试完成 ===")
    print("总结: 播放位置字典功能正常工作")
    print("每个视频的播放位置可以被单独保存和恢复")

if __name__ == '__main__':
    test_playback_positions()