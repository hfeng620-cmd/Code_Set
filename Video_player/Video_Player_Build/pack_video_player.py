#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
视频播放器打包脚本
将 Video_Player_Perfect.py 打包成可执行文件
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path

def main():
    print("=" * 60)
    print("视频播放器打包工具")
    print("=" * 60)
    
    # 获取当前目录
    current_dir = Path(__file__).parent
    source_dir = current_dir.parent / "Video_Player_Source"
    resources_dir = current_dir.parent / "Video_Player_Resources"
    dist_dir = current_dir / "dist"
    build_dir = current_dir / "build"
    
    # 检查源文件
    source_file = source_dir / "Video_Player_Perfect.py"
    if not source_file.exists():
        print(f"❌ 错误: 找不到源文件: {source_file}")
        return 1
    
    print(f"📁 源文件: {source_file}")
    print(f"📁 资源目录: {resources_dir}")
    print(f"📁 输出目录: {dist_dir}")
    print()
    
    # 检查图标文件
    icon_file = resources_dir / "favicon.ico"
    if not icon_file.exists():
        print(f"⚠️  警告: 找不到图标文件: {icon_file}")
        icon_file = None
    else:
        print(f"✅ 图标文件: {icon_file}")
    
    # 清理旧的构建目录
    print("\n🧹 清理旧的构建文件...")
    if dist_dir.exists():
        shutil.rmtree(dist_dir)
        print(f"  已删除: {dist_dir}")
    if build_dir.exists():
        shutil.rmtree(build_dir)
        print(f"  已删除: {build_dir}")
    
    # 构建 PyInstaller 命令
    cmd = [
        sys.executable,
        "-m", "PyInstaller",
        "--name=漂亮视频播放器",
        "--onefile",  # 打包成单个文件
        "--windowed",  # 窗口模式，不显示控制台
        "--clean",  # 清理临时文件
        "--noconfirm",  # 不确认覆盖
        "--distpath", str(dist_dir),
        "--workpath", str(build_dir),
        "--specpath", str(current_dir),
    ]
    
    # 添加图标
    if icon_file:
        cmd.extend(["--icon", str(icon_file)])
    
    # 添加隐藏导入（PyQt5 模块）
    cmd.extend([
        "--hidden-import=PyQt5",
        "--hidden-import=PyQt5.QtCore",
        "--hidden-import=PyQt5.QtWidgets",
        "--hidden-import=PyQt5.QtGui",
        "--hidden-import=PyQt5.QtMultimedia",
        "--hidden-import=PyQt5.QtMultimediaWidgets",
    ])
    
    # 添加数据文件（图标资源）
    if resources_dir.exists():
        for file in resources_dir.glob("*.ico"):
            cmd.extend(["--add-data", f"{file};Video_Player_Resources"])
    
    # 添加源文件
    cmd.append(str(source_file))
    
    print("\n🔨 开始打包视频播放器...")
    print(f"命令: {' '.join(cmd)}")
    print()
    
    # 执行打包命令
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, encoding='utf-8')
        
        if result.returncode == 0:
            print("✅ 打包成功！")
            
            # 显示输出信息
            if result.stdout:
                print("\n📋 标准输出:")
                print(result.stdout[:500] + "..." if len(result.stdout) > 500 else result.stdout)
            
            # 检查生成的可执行文件
            exe_file = dist_dir / "漂亮视频播放器.exe"
            if exe_file.exists():
                print(f"\n🎉 可执行文件已生成: {exe_file}")
                print(f"📏 文件大小: {exe_file.stat().st_size / (1024*1024):.2f} MB")
                
                # 创建使用说明
                create_readme(dist_dir, exe_file, resources_dir)
                
                print("\n📝 使用说明:")
                print("  1. 将 'dist' 文件夹发送给其他人")
                print("  2. 运行 '漂亮视频播放器.exe' 即可使用")
                print("  3. 不需要安装 Python 或任何依赖")
                print("  4. 支持 Windows 7/8/10/11 系统")
                
                return 0
            else:
                print(f"❌ 错误: 找不到生成的可执行文件: {exe_file}")
                return 1
        else:
            print("❌ 打包失败！")
            print(f"返回码: {result.returncode}")
            
            if result.stdout:
                print("\n📋 标准输出:")
                print(result.stdout)
            
            if result.stderr:
                print("\n❌ 错误输出:")
                print(result.stderr)
            
            return result.returncode
            
    except Exception as e:
        print(f"❌ 打包过程中出现异常: {e}")
        return 1

def create_readme(dist_dir, exe_file, resources_dir):
    """创建使用说明文件"""
    readme_file = dist_dir / "使用说明.txt"
    
    readme_content = f"""漂亮视频播放器 - 使用说明
========================================

🎬 程序介绍
这是一个功能强大的视频播放器，支持：
- 播放多种视频格式（MP4、AVI、WMV、MKV等）
- 智能快进/快退功能
- 全屏播放模式
- 视频文件管理
- 快捷键操作
- 窗口自适应

🚀 使用方法
1. 双击运行 '{exe_file.name}'
2. 程序会自动启动，无需安装
3. 首次运行可能需要几秒钟时间

📁 文件结构
- {exe_file.name} - 主程序
- Video_Player_Resources/ - 图标资源文件夹

🎯 主要功能
1. 打开视频：点击"打开视频"按钮或拖放视频文件到窗口
2. 播放控制：空格键播放/暂停，方向键快进/快退
3. 全屏模式：按 F 键进入/退出全屏
4. 音量控制：W/S 键或上下方向键调节音量
5. 文件管理：右侧列表显示视频文件，支持最近播放记录

⚙️ 系统要求
- Windows 7/8/10/11 操作系统
- 不需要安装 Python 或其他依赖
- 需要 DirectX 9.0c 或更高版本

🔧 故障排除
1. 如果程序无法启动，请确保系统已安装 Visual C++ Redistributable
2. 如果视频无法播放，请检查视频文件是否损坏
3. 如果界面显示异常，请尝试调整系统显示缩放设置

📞 技术支持
如有问题，请检查程序目录下的日志文件或联系开发者。

祝您使用愉快！ 🎉

版本: 1.0.0
更新日期: 2026年1月24日
"""
    
    with open(readme_file, 'w', encoding='utf-8') as f:
        f.write(readme_content)
    
    print(f"📄 已创建使用说明: {readme_file}")

if __name__ == "__main__":
    sys.exit(main())