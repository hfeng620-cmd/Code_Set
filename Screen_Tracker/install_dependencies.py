#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
跨平台依赖安装脚本
根据操作系统自动安装所需的依赖包
"""

import os
import sys
import subprocess
import platform

def check_pip_available():
    """检查 pip 是否可用"""
    try:
        subprocess.run([sys.executable, "-m", "pip", "--version"], 
                      capture_output=True, check=True)
        return True
    except (subprocess.CalledProcessError, FileNotFoundError):
        return False

def install_package(package_name):
    """安装指定的 Python 包"""
    try:
        print(f"正在安装 {package_name}...")
        subprocess.run([sys.executable, "-m", "pip", "install", package_name], 
                      check=True, capture_output=True, text=True)
        print(f"✓ {package_name} 安装成功")
        return True
    except subprocess.CalledProcessError as e:
        print(f"✗ {package_name} 安装失败: {e.stderr}")
        return False

def check_system_tools():
    """检查系统工具是否可用"""
    system = platform.system()
    
    if system == "Linux":
        print("检测到 Linux 系统，检查必要的系统工具...")
        
        # 检查 xdotool
        try:
            subprocess.run(['which', 'xdotool'], capture_output=True, check=True)
            print("✓ xdotool 已安装")
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("⚠ xdotool 未安装，活动窗口检测功能可能受限")
            print("  在 Arch Linux 上可以使用: sudo pacman -S xdotool")
            print("  在 Ubuntu/Debian 上可以使用: sudo apt install xdotool")
        
        # 检查 xprintidle
        try:
            subprocess.run(['which', 'xprintidle'], capture_output=True, check=True)
            print("✓ xprintidle 已安装")
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("⚠ xprintidle 未安装，空闲时间检测功能可能受限")
            print("  在 Arch Linux 上可以使用: sudo pacman -S xprintidle")
            print("  在 Ubuntu/Debian 上可以使用: sudo apt install xprintidle")
        
        # 检查 wmctrl
        try:
            subprocess.run(['which', 'wmctrl'], capture_output=True, check=True)
            print("✓ wmctrl 已安装")
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("⚠ wmctrl 未安装，窗口管理功能可能受限")
            print("  在 Arch Linux 上可以使用: sudo pacman -S wmctrl")
            print("  在 Ubuntu/Debian 上可以使用: sudo apt install wmctrl")
    
    elif system == "Windows":
        print("检测到 Windows 系统...")
        print("Windows 系统需要 pywin32 库，将通过 pip 安装")
    
    else:
        print(f"检测到 {system} 系统，请手动检查依赖")

def main():
    """主函数"""
    print("=" * 60)
    print("屏幕使用时长统计工具 - 依赖安装脚本")
    print("=" * 60)
    
    # 检查 pip 是否可用
    if not check_pip_available():
        print("错误: pip 不可用，请先安装 pip")
        print("在 Linux 上: sudo apt install python3-pip (Ubuntu/Debian)")
        print("在 Arch Linux 上: sudo pacman -S python-pip")
        return 1
    
    # 基础依赖包列表
    base_packages = [
        "psutil",      # 系统进程信息
        "pillow",      # 图像处理 (PIL)
        "matplotlib",  # 数据可视化
        "pystray",     # 系统托盘
    ]
    
    # 平台特定包
    system = platform.system()
    if system == "Windows":
        base_packages.append("pywin32")  # Windows API
    elif system == "Linux":
        base_packages.append("python3-xlib")  # X11 库 (可选)
    
    print(f"操作系统: {system}")
    print(f"Python 路径: {sys.executable}")
    print()
    
    # 检查系统工具
    check_system_tools()
    print()
    
    # 安装 Python 包
    print("开始安装 Python 依赖包...")
    print("-" * 40)
    
    success_count = 0
    fail_count = 0
    
    for package in base_packages:
        if install_package(package):
            success_count += 1
        else:
            fail_count += 1
    
    print("-" * 40)
    print(f"安装完成: {success_count} 个成功, {fail_count} 个失败")
    print()
    
    # 提供使用说明
    print("使用说明:")
    print("1. 运行跨平台版本: python ScreenTracker_cross_platform.pyw")
    print("2. 如果系统托盘不可用，程序将在后台运行")
    print("3. 按 Ctrl+C 停止后台运行的程序")
    print()
    
    if system == "Linux":
        print("Linux 系统额外说明:")
        print("1. 确保已安装必要的系统工具:")
        print("   - xdotool: 用于活动窗口检测")
        print("   - xprintidle: 用于空闲时间检测")
        print("   - wmctrl: 用于窗口管理 (可选)")
        print("2. 如果使用 GNOME 桌面环境，可能需要安装:")
        print("   - sudo apt install gir1.2-appindicator3-0.1 (Ubuntu)")
        print("   - sudo pacman -S libappindicator-gtk3 (Arch)")
        print("3. 对于 KDE Plasma，系统托盘支持通常更好")
    
    elif system == "Windows":
        print("Windows 系统额外说明:")
        print("1. 确保已安装 Microsoft Visual C++ Redistributable")
        print("2. 如果系统托盘图标不显示，尝试以管理员身份运行")
        print("3. 开机自启功能需要注册表写入权限")
    
    print()
    print("安装完成！现在可以运行屏幕统计工具了。")
    
    return 0 if fail_count == 0 else 1

if __name__ == "__main__":
    sys.exit(main())