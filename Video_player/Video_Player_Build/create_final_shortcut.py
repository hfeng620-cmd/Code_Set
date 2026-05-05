#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
创建最终的视频播放器桌面快捷方式（使用批处理文件）
"""

import os
import sys

def main():
    print("创建视频播放器桌面快捷方式（最终版）")
    print("=" * 60)
    
    if sys.platform != 'win32':
        print("此脚本仅支持Windows系统")
        return
    
    try:
        import win32com.client
    except ImportError:
        print("需要安装 pywin32 库")
        print("请运行: pip install pywin32")
        return
    
    # 获取当前脚本的目录
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # 批处理文件路径
    batch_file = os.path.join(script_dir, "启动视频播放器.bat")
    
    if not os.path.exists(batch_file):
        print(f"[错误] 未找到批处理文件: {batch_file}")
        return
    
    # 图标路径
    resources_dir = os.path.join(script_dir, "..", "Video_Player_Resources")
    nap_cn_icon = os.path.join(resources_dir, "nap_cn.ico")
    favicon_icon = os.path.join(resources_dir, "favicon.ico")
    
    print("配置信息：")
    print(f"  快捷方式名称: 漂亮视频播放器.lnk")
    print(f"  目标文件: {os.path.basename(batch_file)}")
    print(f"  桌面快捷方式图标: nap_cn.ico")
    print(f"  程序内部图标: favicon.ico")
    print("=" * 60)
    
    # 检查图标文件
    if not os.path.exists(nap_cn_icon):
        print(f"[警告] 未找到桌面快捷方式图标: {nap_cn_icon}")
        icon_location = batch_file
    else:
        icon_location = nap_cn_icon
        print(f"[找到] 桌面快捷方式图标: {nap_cn_icon}")
    
    if not os.path.exists(favicon_icon):
        print(f"[警告] 未找到程序内部图标: {favicon_icon}")
    else:
        print(f"[找到] 程序内部图标: {favicon_icon}")
    
    # 获取桌面路径
    desktop = os.path.join(os.path.expanduser("~"), "Desktop")
    if not os.path.exists(desktop):
        desktop = os.path.join(os.path.expanduser("~"), "桌面")
        if not os.path.exists(desktop):
            desktop = os.path.join(os.environ.get("USERPROFILE", ""), "Desktop")
    
    # 快捷方式路径
    shortcut_path = os.path.join(desktop, "漂亮视频播放器.lnk")
    
    try:
        # 创建快捷方式
        shell = win32com.client.Dispatch("WScript.Shell")
        shortcut = shell.CreateShortCut(shortcut_path)
        shortcut.Targetpath = batch_file
        shortcut.WorkingDirectory = script_dir
        shortcut.IconLocation = icon_location
        shortcut.Description = "漂亮视频播放器 (Video_Player_Perfect)"
        shortcut.save()
        
        print(f"\n[成功] 快捷方式已创建在桌面: {shortcut_path}")
        print(f"[成功] 快捷方式目标: {os.path.basename(batch_file)}")
        print(f"[成功] 工作目录: {script_dir}")
        print(f"[成功] 桌面快捷方式图标: nap_cn.ico")
        
        print("\n" + "=" * 60)
        print("✅ 快捷方式创建成功！")
        print("=" * 60)
        print("\n使用说明：")
        print("  1. 双击桌面上的 '漂亮视频播放器' 快捷方式")
        print("  2. 批处理文件会自动启动视频播放器")
        print("  3. 桌面快捷方式使用 nap_cn.ico 作为图标")
        print("  4. 程序窗口使用 favicon.ico 作为内部图标")
        print("  5. 这是正常的设计，两个图标可以不同")
        
        print("\n图标文件位置：")
        print(f"  nap_cn.ico: {nap_cn_icon}")
        print(f"  favicon.ico: {favicon_icon}")
        
        print("\n" + "=" * 60)
        print("📝 注意：")
        print("  这个快捷方式使用批处理文件启动，")
        print("  比直接使用Python快捷方式更可靠")
        print("  批处理文件会显示启动过程，便于调试")
        print("=" * 60)
        
    except Exception as e:
        print(f"\n[失败] 创建快捷方式时出错: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()