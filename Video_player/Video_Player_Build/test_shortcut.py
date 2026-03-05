#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
测试快捷方式是否正常工作
"""

import os
import sys
import subprocess

def main():
    print("测试视频播放器快捷方式...")
    print("=" * 60)
    
    # 获取桌面快捷方式路径
    desktop = os.path.join(os.path.expanduser("~"), "Desktop")
    shortcut_path = os.path.join(desktop, "漂亮视频播放器.lnk")
    
    if not os.path.exists(shortcut_path):
        print(f"错误: 快捷方式不存在: {shortcut_path}")
        return
    
    print(f"快捷方式路径: {shortcut_path}")
    
    # 视频播放器脚本路径
    video_player_script = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        "..", "Video_Player_Source", "Video_Player_Perfect.py"
    )
    video_player_script = os.path.abspath(video_player_script)
    
    print(f"视频播放器脚本: {video_player_script}")
    
    if not os.path.exists(video_player_script):
        print(f"错误: 视频播放器脚本不存在")
        return
    
    # Pythonw.exe 路径
    python_exe = sys.executable
    pythonw_exe = python_exe.replace("python.exe", "pythonw.exe")
    if not os.path.exists(pythonw_exe):
        pythonw_exe = python_exe
    
    print(f"Pythonw.exe: {pythonw_exe}")
    
    # 测试直接运行
    print("\n" + "=" * 60)
    print("测试直接运行视频播放器...")
    
    try:
        # 切换到视频播放器脚本目录
        script_dir = os.path.dirname(video_player_script)
        os.chdir(script_dir)
        
        # 运行命令
        cmd = [pythonw_exe, "Video_Player_Perfect.py"]
        print(f"运行命令: {' '.join(cmd)}")
        print(f"工作目录: {script_dir}")
        
        # 使用 subprocess 运行
        process = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            creationflags=subprocess.CREATE_NO_WINDOW if sys.platform == 'win32' else 0
        )
        
        # 等待几秒钟看是否有输出
        try:
            stdout, stderr = process.communicate(timeout=5)
            if stdout:
                print(f"标准输出: {stdout}")
            if stderr:
                print(f"错误输出: {stderr}")
            
            if process.returncode == 0:
                print("✓ 视频播放器启动成功")
            else:
                print(f"✗ 视频播放器启动失败，返回码: {process.returncode}")
                
        except subprocess.TimeoutExpired:
            print("✓ 视频播放器正在运行（进程未退出）")
            process.terminate()
            
    except Exception as e:
        print(f"运行错误: {e}")
    
    print("\n" + "=" * 60)
    print("故障排除建议:")
    print("1. 检查 Python 环境是否正确")
    print("2. 检查视频播放器依赖库是否安装")
    print("3. 尝试手动运行: pythonw.exe Video_Player_Perfect.py")
    print("4. 检查是否有错误日志")
    
    input("\n按 Enter 键退出...")

if __name__ == "__main__":
    main()