#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
创建视频播放器桌面快捷方式（简单版）
直接指向播放器程序文件夹中的可执行文件
"""

import os
import sys
from pathlib import Path

def create_desktop_shortcut():
    """创建桌面快捷方式"""
    
    # 获取桌面路径
    desktop = Path.home() / "Desktop"
    
    # 目标文件路径 - 指向播放器程序文件夹中的可执行文件
    target_path = r"D:\VS_Code\播放器程序\漂亮视频播放器.exe"
    
    # 图标文件路径
    icon_path = r"D:\VS_Code\Code_Set\Video_player\Video_Player_Resources\nap_cn.ico"
    
    # 快捷方式路径
    shortcut_path = desktop / "漂亮视频播放器.lnk"
    
    # 确保目标文件存在
    if not os.path.exists(target_path):
        print(f"❌ 错误: 目标文件不存在: {target_path}")
        print(f"请先确保已打包程序并复制到播放器程序文件夹")
        return False
    
    # 确保图标文件存在
    if not os.path.exists(icon_path):
        print(f"⚠️ 警告: 图标文件不存在: {icon_path}")
        icon_path = target_path  # 使用可执行文件自身的图标
    
    try:
        # 导入必要的模块
        import win32com.client
        
        # 创建快捷方式
        shell = win32com.client.Dispatch("WScript.Shell")
        shortcut = shell.CreateShortcut(str(shortcut_path))
        
        # 设置快捷方式属性
        shortcut.TargetPath = target_path
        shortcut.WorkingDirectory = os.path.dirname(target_path)
        shortcut.IconLocation = icon_path
        shortcut.Description = "漂亮视频播放器 - 功能强大的视频播放软件"
        
        # 保存快捷方式
        shortcut.save()
        
        print(f"✅ 快捷方式创建成功: {shortcut_path}")
        print(f"📁 目标文件: {target_path}")
        print(f"📁 工作目录: {os.path.dirname(target_path)}")
        print(f"🎨 图标文件: {icon_path}")
        
        return True
        
    except ImportError:
        print("❌ 错误: 需要安装 pywin32 模块")
        print("请运行: pip install pywin32")
        return False
    except Exception as e:
        print(f"❌ 错误: 创建快捷方式失败: {e}")
        return False

def main():
    """主函数"""
    print("=" * 60)
    print("创建视频播放器桌面快捷方式（简单版）")
    print("=" * 60)
    print()
    
    # 检查目标文件是否存在
    target_path = r"D:\VS_Code\播放器程序\漂亮视频播放器.exe"
    if os.path.exists(target_path):
        print(f"✅ 找到目标文件: {target_path}")
        print(f"📊 文件大小: {os.path.getsize(target_path) / 1024 / 1024:.2f} MB")
        print(f"📅 修改时间: {os.path.getmtime(target_path)}")
    else:
        print(f"❌ 目标文件不存在: {target_path}")
        print("请先打包程序并复制到播放器程序文件夹")
        return
    
    print()
    
    # 创建快捷方式
    if create_desktop_shortcut():
        print()
        print("=" * 60)
        print("✅ 快捷方式创建成功！")
        print("=" * 60)
        print()
        print("使用说明:")
        print("  1. 双击桌面上的 '漂亮视频播放器' 快捷方式")
        print("  2. 程序将直接启动，无需命令行窗口")
        print("  3. 程序启动时窗口最大化，全屏退出后恢复原大小")
        print()
        print("功能特性:")
        print("  ✅ 程序启动时窗口最大化")
        print("  ✅ 全屏退出后窗口恢复原大小")
        print("  ✅ 支持快捷键操作 (F: 全屏, Space: 播放/暂停)")
        print("  ✅ 支持智能快进/快退")
        print("  ✅ 支持视频文件管理")
    else:
        print()
        print("=" * 60)
        print("❌ 快捷方式创建失败")
        print("=" * 60)

if __name__ == "__main__":
    main()