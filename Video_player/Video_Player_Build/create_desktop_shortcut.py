#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
创建视频播放器桌面快捷方式的脚本
"""
import os
import sys

# 设置控制台编码为 UTF-8（Windows 兼容性）
if sys.platform == 'win32':
    try:
        import io
        sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')
        sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8')
    except:
        pass  # 如果设置失败，继续使用默认编码

# Windows系统图标列表（shell32.dll中的图标索引）
WINDOWS_ICONS = {
    '1': {'name': '视频播放器', 'dll': 'shell32.dll', 'index': 150, 'desc': '媒体播放器图标'},
    '2': {'name': '视频文件', 'dll': 'shell32.dll', 'index': 135, 'desc': '视频文件图标'},
    '3': {'name': '多媒体文件', 'dll': 'shell32.dll', 'index': 134, 'desc': '多媒体文件图标'},
    '4': {'name': '媒体文件', 'dll': 'shell32.dll', 'index': 133, 'desc': '媒体文件图标'},
    '5': {'name': '电影胶片', 'dll': 'shell32.dll', 'index': 226, 'desc': '电影胶片图标'},
    '6': {'name': '播放', 'dll': 'shell32.dll', 'index': 238, 'desc': '播放按钮图标'},
    '7': {'name': '文件夹（媒体）', 'dll': 'shell32.dll', 'index': 240, 'desc': '媒体文件夹图标'},
    '8': {'name': 'DVD', 'dll': 'shell32.dll', 'index': 137, 'desc': 'DVD图标'},
    '9': {'name': '计算机', 'dll': 'shell32.dll', 'index': 15, 'desc': '计算机图标'},
    '10': {'name': '默认Python图标', 'dll': None, 'index': None, 'desc': '使用Python默认图标'},
}

def show_icon_selection(icon_index=None):
    """显示图标选择界面"""
    print("\n请选择桌面快捷方式的图标：")
    print("=" * 50)
    for key, icon_info in WINDOWS_ICONS.items():
        print(f"{key:>3}. {icon_info['name']:<20} - {icon_info['desc']}")
    print("=" * 50)
    
    # 如果提供了图标索引，直接返回
    if icon_index and str(icon_index) in WINDOWS_ICONS:
        choice = WINDOWS_ICONS[str(icon_index)]
        print(f"\n[已选择] {choice['name']} - {choice['desc']}")
        return choice
    
    # 如果没有提供索引，尝试交互式选择
    try:
        while True:
            choice = input("\n请输入选项编号 (1-10，直接回车使用默认:视频播放器): ").strip()
            if not choice:
                # 默认选择视频播放器图标
                print("\n[使用默认] 视频播放器图标")
                return WINDOWS_ICONS['1']
            elif choice in WINDOWS_ICONS:
                return WINDOWS_ICONS[choice]
            else:
                print("无效的选项，请重新输入！")
    except (EOFError, KeyboardInterrupt):
        # 非交互式环境，使用默认图标
        print("\n[非交互式模式] 使用默认图标: 视频播放器")
        return WINDOWS_ICONS['1']

def get_icon_location(icon_choice):
    """获取图标位置字符串"""
    if icon_choice['dll'] is None:
        # 使用默认图标（Python图标）
        return None
    
    # Windows系统DLL路径
    dll_path = os.path.join(os.environ.get('SystemRoot', 'C:\\Windows'), 
                           'System32', icon_choice['dll'])
    
    if os.path.exists(dll_path):
        # 格式：路径,图标索引
        return f"{dll_path},{icon_choice['index']}"
    else:
        return None

def create_shortcut_windows(icon_choice=None):
    """在Windows系统创建桌面快捷方式"""
    try:
        import win32com.client
    except ImportError:
        print("需要安装 pywin32 库")
        print("请运行: pip install pywin32")
        return False
    
    try:
        # 获取当前脚本的目录
        script_dir = os.path.dirname(os.path.abspath(__file__))
        
        # 使用 pythonw.exe 而不是 python.exe，避免显示控制台窗口
        python_exe = sys.executable
        pythonw_exe = python_exe.replace("python.exe", "pythonw.exe")
        if not os.path.exists(pythonw_exe):
            # 如果 pythonw.exe 不存在，尝试查找同目录下的 pythonw.exe
            python_dir = os.path.dirname(python_exe)
            pythonw_exe = os.path.join(python_dir, "pythonw.exe")
            if not os.path.exists(pythonw_exe):
                print("[警告] 未找到 pythonw.exe，将使用 python.exe（可能仍会显示控制台窗口）")
                pythonw_exe = python_exe
        
        # 视频播放器脚本在 Video_Player_Source 目录中
        video_player_source_dir = os.path.join(script_dir, "..", "Video_Player_Source")
        video_player_script = os.path.join(video_player_source_dir, "Video_Player_Perfect.py")
        
        if not os.path.exists(video_player_script):
            print(f"[错误] 未找到视频播放器脚本: {video_player_script}")
            print(f"[错误] 请确保 Video_Player_Perfect.py 文件存在")
            return False
        
        # 获取桌面路径
        desktop = os.path.join(os.path.expanduser("~"), "Desktop")
        if not os.path.exists(desktop):
            # 尝试其他可能的桌面路径
            desktop = os.path.join(os.path.expanduser("~"), "桌面")
            if not os.path.exists(desktop):
                desktop = os.path.join(os.environ.get("USERPROFILE", ""), "Desktop")
        
        # 快捷方式路径
        shortcut_path = os.path.join(desktop, "漂亮视频播放器.lnk")
        
        # 获取图标位置 - 使用 nap_cn.ico 作为桌面快捷方式图标
        icon_location = None
        
        # 首先尝试在 Video_Player_Resources 目录中查找 nap_cn.ico
        resources_dir = os.path.join(script_dir, "..", "Video_Player_Resources")
        nap_cn_icon = os.path.join(resources_dir, "nap_cn.ico")
        favicon_icon = os.path.join(resources_dir, "favicon.ico")
        
        if os.path.exists(nap_cn_icon):
            icon_location = nap_cn_icon
            print(f"[找到图标] nap_cn.ico (桌面快捷方式图标)")
        else:
            # 如果在资源目录没找到，尝试在当前目录查找
            icon_paths = [
                os.path.join(script_dir, "nap_cn.ico"),
                os.path.join(script_dir, "player_icon.ico"),
                os.path.join(script_dir, "favicon.ico"),
                os.path.join(script_dir, "icon.ico"),
                os.path.join(script_dir, "app_icon.ico"),
            ]
            for icon_path in icon_paths:
                if os.path.exists(icon_path):
                    icon_location = icon_path
                    print(f"[找到图标] {os.path.basename(icon_path)} (桌面快捷方式图标)")
                    break
        
        # 如果没找到图标，使用默认图标
        if not icon_location:
            print("[提示] 未找到 nap_cn.ico 图标文件，使用默认图标")
            print(f"[提示] 请将 nap_cn.ico 放在: {resources_dir}")
            icon_location = pythonw_exe  # 使用 pythonw.exe 的图标
        
        # 检查 favicon.ico 是否存在（程序内部图标）
        if os.path.exists(favicon_icon):
            print(f"[找到图标] favicon.ico (程序内部图标)")
        else:
            print(f"[提示] 未找到 favicon.ico (程序内部图标)")
        
        # 创建快捷方式 - 使用与批处理文件相同的逻辑
        shell = win32com.client.Dispatch("WScript.Shell")
        shortcut = shell.CreateShortCut(shortcut_path)
        
        # 使用批处理文件的路径作为目标
        batch_file = os.path.join(script_dir, "启动视频播放器.bat")
        if os.path.exists(batch_file):
            # 如果批处理文件存在，使用批处理文件作为目标
            shortcut.Targetpath = batch_file
            shortcut.Arguments = ""
            shortcut.WorkingDirectory = script_dir
            print(f"[使用批处理文件] {os.path.basename(batch_file)}")
        else:
            # 如果批处理文件不存在，使用原始Python方式
            shortcut.Targetpath = pythonw_exe  # 使用 pythonw.exe 避免显示控制台
            shortcut.Arguments = f'"{video_player_script}"'
            # 工作目录设置为视频播放器脚本所在目录
            shortcut.WorkingDirectory = os.path.dirname(video_player_script)
            print(f"[使用Python直接运行] {os.path.basename(video_player_script)}")
        
        shortcut.IconLocation = icon_location
        shortcut.Description = "漂亮视频播放器 (Video_Player_Perfect)"
        shortcut.save()
        
        print(f"\n[成功] 快捷方式已创建在桌面: {shortcut_path}")
        
        if os.path.exists(batch_file):
            print(f"[成功] 快捷方式目标: {os.path.basename(batch_file)} (批处理文件)")
            print(f"[成功] 目标完整路径: {batch_file}")
        else:
            print(f"[成功] 快捷方式目标: {pythonw_exe} \"{os.path.basename(video_player_script)}\"")
            print(f"[成功] 目标完整路径: {video_player_script}")
        
        if icon_location and icon_location != pythonw_exe:
            icon_name = os.path.basename(icon_location)
            print(f"[成功] 桌面快捷方式图标: {icon_name}")
        else:
            print(f"[提示] 使用默认Python图标作为桌面快捷方式图标")
        
        print("\n" + "=" * 60)
        print("图标配置说明：")
        print("   1. 桌面快捷方式图标: nap_cn.ico")
        print("   2. 程序窗口内部图标: favicon.ico")
        print("   3. 图标文件位置: D:\\VS_Code\\Code_Set\\Video_Player_Resources\\")
        print("=" * 60)
        print("\n注意：")
        print("   - 桌面快捷方式使用 nap_cn.ico 作为图标")
        print("   - 程序运行后，窗口标题栏使用 favicon.ico 作为图标")
        print("   - 这是正常的设计，两个图标可以不同")
        return True
        
    except Exception as e:
        import traceback
        print(f"创建快捷方式失败: {e}")
        print(f"错误详情: {traceback.format_exc()}")
        return False

def create_shortcut_manual():
    """手动创建快捷方式的说明"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    video_player_script = os.path.join(script_dir, "Video_Player_Perfect.py")
    python_exe = sys.executable
    pythonw_exe = python_exe.replace("python.exe", "pythonw.exe")
    if not os.path.exists(pythonw_exe):
        pythonw_exe = python_exe
    
    print("\n手动创建快捷方式的方法：")
    print("1. 在桌面上右键 -> 新建 -> 快捷方式")
    print(f"2. 输入目标位置: {pythonw_exe} \"{video_player_script}\"")
    print("3. 输入快捷方式名称: 漂亮视频播放器")
    print("4. 右键快捷方式 -> 属性 -> 更改图标，选择图标文件")
    print("5. 完成")

if __name__ == '__main__':
    print("正在创建视频播放器桌面快捷方式...")
    
    if sys.platform == 'win32':
        # 获取脚本目录
        script_dir = os.path.dirname(os.path.abspath(__file__))
        
        # 获取资源目录路径
        resources_dir = os.path.join(script_dir, "..", "Video_Player_Resources")
        resources_dir = os.path.abspath(resources_dir)
        
        print("=" * 60)
        print("视频播放器桌面快捷方式创建工具")
        print("=" * 60)
        print("配置信息：")
        print(f"  桌面快捷方式图标: nap_cn.ico")
        print(f"  程序内部图标: favicon.ico")
        print(f"  图标文件位置: {resources_dir}")
        print("=" * 60)
        
        # 检查图标文件是否存在
        nap_cn_icon = os.path.join(resources_dir, "nap_cn.ico")
        favicon_icon = os.path.join(resources_dir, "favicon.ico")
        
        if not os.path.exists(nap_cn_icon):
            print(f"[警告] 未找到 nap_cn.ico 文件: {nap_cn_icon}")
            print("请确保图标文件存在")
        else:
            print(f"[找到] nap_cn.ico: {nap_cn_icon}")
        
        if not os.path.exists(favicon_icon):
            print(f"[警告] 未找到 favicon.ico 文件: {favicon_icon}")
        else:
            print(f"[找到] favicon.ico: {favicon_icon}")
        
        print("=" * 60)
        
        # 创建快捷方式
        if not create_shortcut_windows(icon_choice=None):
            print("\n自动创建失败，请使用手动方法：")
            create_shortcut_manual()
        else:
            print("\n" + "=" * 60)
            print("[完成] 桌面快捷方式创建成功！")
            print("=" * 60)
            print("\n使用说明：")
            print("  1. 双击桌面上的 '漂亮视频播放器' 快捷方式启动程序")
            print("  2. 桌面快捷方式使用 nap_cn.ico 作为图标")
            print("  3. 程序窗口使用 favicon.ico 作为内部图标")
            print("  4. 这是正常的设计，两个图标可以不同")
            print("\n图标文件位置：")
            print(f"  nap_cn.ico: {nap_cn_icon}")
            print(f"  favicon.ico: {favicon_icon}")
    else:
        print("此脚本仅支持Windows系统")
        print("在其他系统上，请手动创建启动脚本")
