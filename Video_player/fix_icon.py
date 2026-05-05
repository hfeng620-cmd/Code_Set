#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修复灭霸快捷方式图标
"""
import os
import sys

def fix_thanos_shortcut():
    print("修复灭霸快捷方式图标")
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
    
    # 获取桌面路径
    desktop = os.path.join(os.path.expanduser("~"), "Desktop")
    if not os.path.exists(desktop):
        desktop = os.path.join(os.path.expanduser("~"), "桌面")
        if not os.path.exists(desktop):
            desktop = os.path.join(os.environ.get("USERPROFILE", ""), "Desktop")
    
    # 快捷方式路径
    shortcut_path = os.path.join(desktop, "灭霸.lnk")
    
    if not os.path.exists(shortcut_path):
        print(f"[错误] 未找到快捷方式: {shortcut_path}")
        print("请确保桌面有名为'灭霸.lnk'的快捷方式")
        return
    
    # 图标路径
    resources_dir = os.path.join(os.path.dirname(__file__), "Video_Player_Resources")
    nap_cn_icon = os.path.join(resources_dir, "nap_cn.ico")
    
    print(f"快捷方式路径: {shortcut_path}")
    print(f"图标文件路径: {nap_cn_icon}")
    print("=" * 60)
    
    # 检查图标文件
    if not os.path.exists(nap_cn_icon):
        print(f"[错误] 未找到图标文件: {nap_cn_icon}")
        print("请确保图标文件存在")
        return
    
    try:
        # 打开快捷方式
        shell = win32com.client.Dispatch("WScript.Shell")
        shortcut = shell.CreateShortcut(shortcut_path)
        
        print("当前快捷方式信息:")
        print(f"  目标: {shortcut.TargetPath}")
        print(f"  参数: {shortcut.Arguments}")
        print(f"  工作目录: {shortcut.WorkingDirectory}")
        print(f"  图标位置: {shortcut.IconLocation}")
        print(f"  描述: {shortcut.Description}")
        print("=" * 60)
        
        # 修复图标
        old_icon = shortcut.IconLocation
        shortcut.IconLocation = nap_cn_icon
        shortcut.save()
        
        print(f"[成功] 已修复图标路径")
        print(f"  原图标: {old_icon}")
        print(f"  新图标: {nap_cn_icon}")
        print("=" * 60)
        
        # 验证修复
        shortcut2 = shell.CreateShortcut(shortcut_path)
        print("修复后验证:")
        print(f"  图标位置: {shortcut2.IconLocation}")
        
        if shortcut2.IconLocation == nap_cn_icon:
            print("✅ 图标修复成功！")
        else:
            print("❌ 图标修复失败")
            
    except Exception as e:
        print(f"[失败] 修复快捷方式时出错: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    fix_thanos_shortcut()