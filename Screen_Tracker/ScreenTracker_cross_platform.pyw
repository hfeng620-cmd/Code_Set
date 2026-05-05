#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
跨平台屏幕使用时长统计工具
支持 Windows 和 Linux (Arch Linux) 双系统
"""

# ============================================================================
# 导入库及其作用说明
# ============================================================================

# Python标准库
import os
import sys
import time
import sqlite3
import datetime
import threading
import multiprocessing
import platform
import subprocess
import shutil

# 第三方库
import psutil
from PIL import Image, ImageDraw

# 平台检测
IS_WINDOWS = sys.platform == 'win32'
IS_LINUX = sys.platform.startswith('linux')

# ============================================================================
# 条件导入平台特定库
# ============================================================================

if IS_WINDOWS:
    try:
        import win32gui
        import win32process
        import win32api
        import winreg
        import ctypes
        WINDOWS_IMPORT_SUCCESS = True
    except ImportError:
        print("警告: Windows 特定库导入失败，部分功能可能受限")
        WINDOWS_IMPORT_SUCCESS = False
        # 创建虚拟模块以避免后续引用错误
        class DummyModule:
            def __getattr__(self, name):
                return None
        win32gui = win32process = win32api = winreg = DummyModule()
        ctypes = DummyModule()

elif IS_LINUX:
    try:
        # Linux 上尝试导入 Xlib 用于获取活动窗口
        from Xlib import display, X
        from Xlib.ext import record
        from Xlib.protocol import rq
        LINUX_XLIB_AVAILABLE = True
    except ImportError:
        print("警告: Xlib 不可用，尝试使用 xdotool 替代")
        LINUX_XLIB_AVAILABLE = False
        
    # 检查 xdotool 是否可用
    try:
        subprocess.run(['which', 'xdotool'], capture_output=True, check=True)
        XDOTOOL_AVAILABLE = True
    except (subprocess.CalledProcessError, FileNotFoundError):
        XDOTOOL_AVAILABLE = False
        print("警告: xdotool 不可用，活动窗口检测可能受限")

# 尝试导入系统托盘库
try:
    import pystray
    PYSYSTRAY_AVAILABLE = True
except ImportError:
    print("警告: pystray 不可用，系统托盘功能将不可用")
    PYSYSTRAY_AVAILABLE = False

# 尝试导入 matplotlib
try:
    import matplotlib.pyplot as plt
    MATPLOTLIB_AVAILABLE = True
    # 设置 matplotlib 支持中文显示
    plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'Arial', 'DejaVu Sans']
    plt.rcParams['axes.unicode_minus'] = False
except ImportError:
    print("警告: matplotlib 不可用，图表功能将不可用")
    MATPLOTLIB_AVAILABLE = False

# ============================================================================
# 配置项
# ============================================================================

# 基础目录：获取当前脚本文件所在的目录
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# 数据库文件路径：在基础目录下创建screen_data.db文件
DB_PATH = os.path.join(BASE_DIR, "screen_data.db")

# 锁文件路径：用于确保程序单实例运行
LOCK_FILE = os.path.join(BASE_DIR, "app.lock")

# 空闲阈值：600秒（10分钟）无操作视为闲置，不记录使用时间
IDLE_THRESHOLD = 600

# 检测间隔：每5秒检测一次活动应用
INTERVAL = 5

# 应用名称美化映射：跨平台应用名称映射
APP_MAP = {
    # 浏览器
    "chrome": "Google Chrome", "google-chrome": "Google Chrome",
    "chrome.exe": "Google Chrome", "msedge.exe": "Edge浏览器",
    "firefox": "Firefox", "firefox.exe": "Firefox",
    "chromium": "Chromium", "chromium-browser": "Chromium",
    
    # 通讯软件
    "wechat": "微信", "wechat.exe": "微信",
    "wxwork": "企业微信", "wxwork.exe": "企业微信",
    "qq": "QQ", "qq.exe": "QQ",
    "telegram": "Telegram", "telegram-desktop": "Telegram",
    "discord": "Discord",
    
    # 开发工具
    "code": "VS Code", "code.exe": "VS Code",
    "pycharm": "PyCharm", "pycharm64.exe": "PyCharm",
    "idea": "IntelliJ IDEA", "idea64.exe": "IntelliJ IDEA",
    "vim": "Vim", "nvim": "Neovim",
    "sublime_text": "Sublime Text",
    
    # 办公软件
    "libreoffice": "LibreOffice", "soffice": "LibreOffice",
    "winword.exe": "Word", "excel.exe": "Excel",
    "powerpnt.exe": "PPT", "wps.exe": "WPS",
    
    # 笔记软件
    "notion": "Notion", "notion.exe": "Notion",
    "obsidian": "Obsidian",
    
    # 媒体播放器
    "vlc": "VLC", "vlc.exe": "VLC",
    "mpv": "MPV", "smplayer": "SMPlayer",
    "potplayer64.exe": "PotPlayer",
    
    # 系统工具
    "explorer.exe": "资源管理器", "nautilus": "文件管理器",
    "dolphin": "Dolphin", "thunar": "Thunar",
    "gnome-terminal": "终端", "konsole": "Konsole",
    "xfce4-terminal": "XFCE终端",
    
    # 其他
    "steam": "Steam", "steam.exe": "Steam",
    "spotify": "Spotify"
}

# ============================================================================
# 跨平台工具类
# ============================================================================

class CrossPlatformUtils:
    """跨平台工具类，提供操作系统无关的接口"""
    
    @staticmethod
    def show_message(title, message, message_type="info"):
        """
        显示跨平台消息框
        
        参数:
            title: 标题
            message: 消息内容
            message_type: 消息类型 ("info", "warning", "error")
        """
        if IS_WINDOWS and hasattr(ctypes, 'windll'):
            try:
                # Windows 消息框
                style = 0x40 if message_type == "info" else 0x30 if message_type == "warning" else 0x10
                ctypes.windll.user32.MessageBoxW(0, message, title, style)
                return
            except:
                pass
        
        # Linux 或备用方案：使用终端输出
        print(f"[{title}] {message}")
        
        # 尝试使用 tkinter 作为跨平台 GUI
        try:
            import tkinter as tk
            from tkinter import messagebox
            
            root = tk.Tk()
            root.withdraw()  # 隐藏主窗口
            
            if message_type == "info":
                messagebox.showinfo(title, message)
            elif message_type == "warning":
                messagebox.showwarning(title, message)
            elif message_type == "error":
                messagebox.showerror(title, message)
            
            root.destroy()
        except ImportError:
            # tkinter 不可用，使用终端输出
            pass
    
    @staticmethod
    def get_idle_time():
        """
        获取系统空闲时间（用户无操作的时间）
        
        返回:
            float: 空闲时间（秒）
        """
        if IS_WINDOWS and WINDOWS_IMPORT_SUCCESS:
            try:
                last_input = win32api.GetLastInputInfo()
                millis = win32api.GetTickCount() - last_input
                return millis / 1000.0
            except:
                pass
        
        elif IS_LINUX:
            # Linux 方案1: 使用 xprintidle
            try:
                result = subprocess.run(['xprintidle'], capture_output=True, text=True)
                if result.returncode == 0:
                    idle_ms = int(result.stdout.strip())
                    return idle_ms / 1000.0
            except (subprocess.CalledProcessError, FileNotFoundError):
                pass
            
            # Linux 方案2: 检查 X11 输入事件
            if LINUX_XLIB_AVAILABLE:
                try:
                    # 这是一个简化的实现，实际需要更复杂的 X11 事件监听
                    # 这里返回一个保守估计
                    return 0
                except:
                    pass
        
        # 备用方案：返回0（假设用户一直在活动）
        return 0
    
    @staticmethod
    def get_active_app():
        """
        获取当前活动应用（前台窗口对应的应用）
        
        返回:
            str: 美化后的应用名称，如果获取失败则返回None
        """
        if IS_WINDOWS and WINDOWS_IMPORT_SUCCESS:
            try:
                hwnd = win32gui.GetForegroundWindow()
                _, pid = win32process.GetWindowThreadProcessId(hwnd)
                if pid > 0:
                    p = psutil.Process(pid)
                    raw_name = p.name().lower()
                    # 使用APP_MAP映射美化应用名称
                    return APP_MAP.get(raw_name, raw_name.replace(".exe", "").capitalize())
            except:
                pass
        
        elif IS_LINUX:
            # Linux 方案1: 使用 xdotool（推荐）
            if XDOTOOL_AVAILABLE:
                try:
                    # 获取活动窗口ID
                    result = subprocess.run(
                        ['xdotool', 'getactivewindow'],
                        capture_output=True, text=True
                    )
                    if result.returncode == 0:
                        window_id = result.stdout.strip()
                        
                        # 获取窗口的PID
                        result = subprocess.run(
                            ['xdotool', 'getwindowpid', window_id],
                            capture_output=True, text=True
                        )
                        if result.returncode == 0:
                            pid = int(result.stdout.strip())
                            p = psutil.Process(pid)
                            raw_name = p.name().lower()
                            return APP_MAP.get(raw_name, raw_name.capitalize())
                except:
                    pass
            
            # Linux 方案2: 使用 wmctrl（备用）
            try:
                result = subprocess.run(['which', 'wmctrl'], capture_output=True)
                if result.returncode == 0:
                    # wmctrl 可用，尝试获取活动窗口
                    result = subprocess.run(
                        ['wmctrl', '-l', '-p'],
                        capture_output=True, text=True
                    )
                    if result.returncode == 0:
                        # 解析输出找到活动窗口
                        lines = result.stdout.strip().split('\n')
                        for line in lines:
                            if '0x' in line and ' ' in line:
                                parts = line.split()
                                if len(parts) >= 4:
                                    try:
                                        pid = int(parts[2])
                                        p = psutil.Process(pid)
                                        raw_name = p.name().lower()
                                        return APP_MAP.get(raw_name, raw_name.capitalize())
                                    except (ValueError, psutil.NoSuchProcess):
                                        continue
            except:
                pass
            
            # Linux 方案3: 使用 xprop（备用）
            try:
                # 获取当前活动窗口的PID
                result = subprocess.run(
                    ['xprop', '-root', '_NET_ACTIVE_WINDOW'],
                    capture_output=True, text=True
                )
                if result.returncode == 0:
                    # 解析输出获取窗口ID
                    lines = result.stdout.strip().split('\n')
                    for line in lines:
                        if '=' in line:
                            window_id = line.split('=')[-1].strip()
                            if window_id.startswith('0x'):
                                # 获取窗口PID
                                result = subprocess.run(
                                    ['xprop', '-id', window_id, '_NET_WM_PID'],
                                    capture_output=True, text=True
                                )
                                if result.returncode == 0:
                                    pid_line = result.stdout.strip()
                                    if '=' in pid_line:
                                        pid = int(pid_line.split('=')[-1].strip())
                                        p = psutil.Process(pid)
                                        raw_name = p.name().lower()
                                        return APP_MAP.get(raw_name, raw_name.capitalize())
            except:
                pass
        
        # 备用方案：返回当前最占CPU的进程
        try:
            processes = []
            for proc in psutil.process_iter(['pid', 'name', 'cpu_percent']):
                try:
                    cpu = proc.info['cpu_percent']
                    if cpu > 0:
                        processes.append((cpu, proc.info['name']))
                except (psutil.NoSuchProcess, psutil.AccessDenied):
                    pass
            
            if processes:
                processes.sort(reverse=True)
                raw_name = processes[0][1].lower()
                return APP_MAP.get(raw_name, raw_name.capitalize())
        except:
            pass
        
        return None
    
    @staticmethod
    def set_startup(enable=True):
        """
        设置或取消开机自启动
        
        参数:
            enable (bool): True表示设置开机自启，False表示取消
        """
        script_path = os.path.abspath(sys.argv[0])
        app_name = "威震天屏幕统计"
        
        if IS_WINDOWS and WINDOWS_IMPORT_SUCCESS:
            # Windows 注册表方案
            key_path = r"Software\Microsoft\Windows\CurrentVersion\Run"
            
            # 确定Python解释器路径
            py_exe = sys.executable
            if "python.exe" in py_exe:
                pyw = py_exe.replace("python.exe", "pythonw.exe")
                if os.path.exists(pyw):
                    py_exe = pyw
            
            # 构建启动命令
            if script_path.endswith(".py") or script_path.endswith(".pyw"):
                cmd = f'"{py_exe}" "{script_path}"'
            else:
                cmd = f'"{script_path}"'
            
            try:
                key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, key_path, 0, winreg.KEY_ALL_ACCESS)
                if enable:
                    winreg.SetValueEx(key, app_name, 0, winreg.REG_SZ, cmd)
                    CrossPlatformUtils.show_message("成功", "已设置为开机自启", "info")
                else:
                    try:
                        winreg.DeleteValue(key, app_name)
                        CrossPlatformUtils.show_message("成功", "已取消开机自启", "info")
                    except FileNotFoundError:
                        pass
                winreg.CloseKey(key)
            except Exception as e:
                CrossPlatformUtils.show_message("错误", f"设置失败: {e}", "error")
        
        elif IS_LINUX:
            # Linux 自启动方案
            autostart_dir = os.path.expanduser("~/.config/autostart")
            desktop_file = os.path.join(autostart_dir, "screen_tracker.desktop")
            
            # 确保目录存在
            os.makedirs(autostart_dir, exist_ok=True)
            
            if enable:
                # 创建 .desktop 文件
                desktop_content = f"""[Desktop Entry]
Type=Application
Name={app_name}
Exec={sys.executable} {script_path}
Comment=屏幕使用时长统计工具
Terminal=false
StartupNotify=false
"""
                try:
                    with open(desktop_file, 'w', encoding='utf-8') as f:
                        f.write(desktop_content)
                    os.chmod(desktop_file, 0o755)
                    CrossPlatformUtils.show_message("成功", "已设置为开机自启", "info")
                except Exception as e:
                    CrossPlatformUtils.show_message("错误", f"创建自启动文件失败: {e}", "error")
            else:
                # 删除 .desktop 文件
                try:
                    if os.path.exists(desktop_file):
                        os.remove(desktop_file)
                        CrossPlatformUtils.show_message("成功", "已取消开机自启", "info")
                except Exception as e:
                    CrossPlatformUtils.show_message("错误", f"删除自启动文件失败: {e}", "error")
        
        else:
            CrossPlatformUtils.show_message("提示", f"当前系统 ({sys.platform}) 不支持自动设置开机自启", "warning")
    
    @staticmethod
    def check_single_instance():
        """
        检查是否已有实例在运行，确保程序单实例运行
        
        返回:
            file: 锁文件句柄，用于程序退出时删除锁文件
        """
        if os.path.exists(LOCK_FILE):
            try:
                # 尝试读取锁文件中的PID
                with open(LOCK_FILE, 'r') as f:
                    pid_str = f.read().strip()
                    if pid_str:
                        pid = int(pid_str)
                        # 检查进程是否仍在运行
                        try:
                            psutil.Process(pid)
                            # 进程仍在运行
                            CrossPlatformUtils.show_message("提示", "程序已经在运行中！请检查系统托盘。", "info")
                            sys.exit(0)
                        except (ValueError, psutil.NoSuchProcess):
                            # PID无效或进程不存在，删除旧锁文件
                            os.remove(LOCK_FILE)
            except:
                # 锁文件损坏或无法读取，删除它
                try:
                    os.remove(LOCK_FILE)
                except:
                    pass
        
        # 创建锁文件，写入当前进程PID
        try:
            f = open(LOCK_FILE, 'w')
            f.write(str(os.getpid()))
            f.flush()
            return f
        except:
            # 如果无法创建锁文件，继续运行但显示警告
            CrossPlatformUtils.show_message("警告", "无法创建锁文件，程序可能运行多个实例", "warning")
            return None

# ============================================================================
# 核心功能类
# ============================================================================

class DatabaseManager:
    """
    数据库管理类，封装数据库操作
    与原始版本兼容，无需修改
    """
    
    def __init__(self, db_path):
        self.db_path = db_path
        self.init_db()
    
    def init_db(self):
        """初始化数据库表结构"""
        conn = sqlite3.connect(self.db_path, check_same_thread=False)
        cursor = conn.cursor()
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS usage_log (
                date TEXT,
                app_name TEXT,
                duration INTEGER,
                PRIMARY KEY (date, app_name)
            )
        ''')
        conn.commit()
        conn.close()
    
    def update_usage(self, app_name, seconds):
        """更新应用使用时间"""
        if not app_name:
            return
        
        today = datetime.date.today().isoformat()
        conn = sqlite3.connect(self.db_path, check_same_thread=False)
        cursor = conn.cursor()
        try:
            cursor.execute('''
                INSERT INTO usage_log (date, app_name, duration)
                VALUES (?, ?, ?)
                ON CONFLICT(date, app_name) 
                DO UPDATE SET duration = duration + ?
            ''', (today, app_name, seconds, seconds))
            conn.commit()
        except Exception as e:
            print(f"数据库错误: {e}")
        finally:
            conn.close()
    
    def get_data(self, date_str):
        """获取指定日期的使用数据"""
        conn = sqlite3.connect(self.db_path, check_same_thread=False)
        cursor = conn.cursor()
        cursor.execute("SELECT app_name, duration FROM usage_log WHERE date = ? ORDER BY duration DESC", (date_str,))
        rows = cursor.fetchall()
        conn.close()
        return rows

class DataPlotter:
    """
    数据可视化类，生成统计报表
    修改为跨平台兼容
    """
    
    @staticmethod
    def format_time_str(seconds):
        """将秒数转换为 xx小时xx分xx秒 的格式"""
        seconds = int(seconds)
        m, s = divmod(seconds, 60)
        h, m = divmod(m, 60)
        
        if h > 0:
            return f"{h}小时{m}分{s}秒"
        elif m > 0:
            return f"{m}分{s}秒"
        else:
            return f"{s}秒"
    
    @staticmethod
    def show_report(db_path, report_type='daily'):
        """显示指定类型的统计报表"""
        if not MATPLOTLIB_AVAILABLE:
            CrossPlatformUtils.show_message("错误", "matplotlib 不可用，无法显示图表", "error")
            return
        
        try:
            # 连接数据库
            conn = sqlite3.connect(db_path)
            cursor = conn.cursor()
            today = datetime.date.today()
            
            title_prefix = ""
            data = []

            # 根据报表类型查询不同的数据
            if report_type == 'daily':
                date_str = today.isoformat()
                title_prefix = f"今日 ({date_str})"
                cursor.execute("SELECT app_name, duration FROM usage_log WHERE date = ? ORDER BY duration DESC", (date_str,))
                data = cursor.fetchall()
            
            elif report_type == 'weekly':
                start_week = today - datetime.timedelta(days=today.weekday())
                title_prefix = f"本周 (自 {start_week})"
                cursor.execute('''
                    SELECT app_name, SUM(duration) FROM usage_log 
                    WHERE date >= ? GROUP BY app_name ORDER BY SUM(duration) DESC
                ''', (start_week.isoformat(),))
                data = cursor.fetchall()

            elif report_type == 'monthly':
                start_month = today.replace(day=1)
                title_prefix = f"本月 ({today.strftime('%Y-%m')})"
                cursor.execute('''
                    SELECT app_name, SUM(duration) FROM usage_log 
                    WHERE date >= ? GROUP BY app_name ORDER BY SUM(duration) DESC
                ''', (start_month.isoformat(),))
                data = cursor.fetchall()

            conn.close()

            # 如果没有数据，显示提示信息并返回
            if not data:
                CrossPlatformUtils.show_message("提示", "该时间段暂无数据", "info")
                return

            # --- 数据准备：为饼图准备数据 ---
            pie_labels = []
            pie_sizes = []
            others_sum_minutes = 0
            
            total_seconds_all = sum([row[1] for row in data])

            # 饼图数据处理
            top_n_pie = 8
            for i, row in enumerate(data):
                minutes = row[1] / 60
                
                if minutes < 1:
                    continue
                
                if i < top_n_pie:
                    pie_labels.append(row[0])
                    pie_sizes.append(minutes)
                else:
                    others_sum_minutes += minutes
            
            if others_sum_minutes > 0:
                pie_labels.append("其他应用")
                pie_sizes.append(others_sum_minutes)

            if not pie_sizes:
                CrossPlatformUtils.show_message("提示", "有效使用时间太短，无法绘图", "info")
                return

            # 生成标题总时间字符串
            total_time_str = DataPlotter.format_time_str(total_seconds_all)
            full_title = f"{title_prefix} 总使用: {total_time_str}"

            # --- 开始绘图 ---
            fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 7))
            fig.canvas.manager.set_window_title(f"统计报表 - {report_type}")
            fig.suptitle(full_title, fontsize=16, fontweight='bold')

            # === 左侧：环形图 ===
            wedges, texts, autotexts = ax1.pie(pie_sizes, labels=pie_labels, autopct='%1.1f%%', startangle=90, pctdistance=0.85)
            centre_circle = plt.Circle((0,0),0.70,fc='white')
            ax1.add_artist(centre_circle)
            ax1.set_title("时间占比分布")

            # === 右侧：纯文本列表 ===
            ax2.axis('off')
            ax2.set_title("详细时长排行 (Top 15)", fontsize=14, pad=10)

            y_pos = 1.0
            line_height = 0.06
            
            for i, row in enumerate(data):
                if i >= 15:
                    break

                app_name = row[0]
                duration_sec = row[1]
                
                time_str = DataPlotter.format_time_str(duration_sec)

                if len(app_name) > 12:
                    app_name = app_name[:11] + "..."
                
                display_text = f"{i+1}. {app_name:<12} —— {time_str}"
                
                font_weight = 'bold' if i < 3 else 'normal'
                font_size = 12 if i < 3 else 11
                color = '#333333'
                
                ax2.text(0.1, y_pos, display_text, 
                         transform=ax2.transAxes, 
                         fontsize=font_size, 
                         fontweight=font_weight,
                         color=color,
                         va='top')
                
                y_pos -= line_height

            plt.tight_layout()
            plt.show()

        except Exception as e:
            print(f"绘图错误: {e}")
            CrossPlatformUtils.show_message("错误", f"生成报表失败: {e}", "error")

class MonitorThread(threading.Thread):
    """
    监控线程类，使用跨平台工具
    """
    def __init__(self, db_manager):
        super().__init__(daemon=True)
        self.db = db_manager
        self.running = True

    def run(self):
        """线程主循环，定期检测用户活动并更新数据库"""
        while self.running:
            try:
                # 使用跨平台工具获取空闲时间
                idle_time = CrossPlatformUtils.get_idle_time()
                
                # 如果空闲时间小于阈值（用户正在活动）
                if idle_time < IDLE_THRESHOLD:
                    app = CrossPlatformUtils.get_active_app()
                    if app:
                        self.db.update_usage(app, INTERVAL)
                time.sleep(INTERVAL)
            except Exception as e:
                print(f"监控线程错误: {e}")
                time.sleep(INTERVAL)

    def stop(self):
        """停止监控线程"""
        self.running = False

def create_icon_image():
    """
    创建系统托盘图标
    
    优先尝试加载 icon.png 文件作为图标，
    如果找不到文件或文件损坏，则生成一个默认的蓝色方块图标
    """
    icon_path = os.path.join(BASE_DIR, "icon.png")
    
    # 1. 尝试加载自定义图标图片
    if os.path.exists(icon_path):
        try:
            return Image.open(icon_path)
        except Exception:
            pass

    # 2. 生成默认图标（蓝色背景上的白色方块）
    w, h = 64, 64
    img = Image.new('RGB', (w, h), color=(73, 109, 137))
    d = ImageDraw.Draw(img)
    d.rectangle([16, 16, 48, 48], fill=(255, 255, 255))
    return img

# ============================================================================
# 主程序入口
# ============================================================================

if __name__ == "__main__":
    """
    程序主入口
    1. 支持多进程冻结（用于PyInstaller打包）
    2. 检查单实例运行
    3. 初始化数据库和监控线程
    4. 创建系统托盘图标和菜单
    5. 进入主事件循环
    """
    # 多进程支持（用于PyInstaller打包后避免子进程问题）
    multiprocessing.freeze_support()
    
    # 显示启动信息
    print(f"屏幕使用时长统计工具 - 跨平台版本")
    print(f"操作系统: {sys.platform}")
    print(f"Python版本: {sys.version}")
    
    # 检查是否已有实例在运行，确保单实例
    lock_file_handle = CrossPlatformUtils.check_single_instance()
    
    # 初始化数据库管理器
    db = DatabaseManager(DB_PATH)
    
    # 创建并启动监控线程
    monitor = MonitorThread(db)
    monitor.start()
    
    # 检查系统托盘是否可用
    if not PYSYSTRAY_AVAILABLE:
        print("警告: 系统托盘功能不可用，程序将在后台运行")
        print("按 Ctrl+C 停止程序")
        
        try:
            # 保持程序运行
            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            print("\n正在停止程序...")
            monitor.stop()
            if lock_file_handle:
                lock_file_handle.close()
                try:
                    os.remove(LOCK_FILE)
                except:
                    pass
            sys.exit(0)
    
    # ============================================================================
    # 系统托盘菜单回调函数
    # ============================================================================

    def on_report(icon, item):
        """报表菜单项回调函数"""
        report_type = 'daily'
        if '周' in str(item):
            report_type = 'weekly'
        if '月' in str(item):
            report_type = 'monthly'
        
        # 使用独立进程生成报表，避免阻塞系统托盘响应
        p = multiprocessing.Process(target=DataPlotter.show_report, args=(DB_PATH, report_type))
        p.start()

    def on_startup(icon, item):
        """设置开机自启菜单项回调函数"""
        CrossPlatformUtils.set_startup(True)

    def on_disable_startup(icon, item):
        """取消开机自启菜单项回调函数"""
        CrossPlatformUtils.set_startup(False)

    def on_exit(icon, item):
        """退出程序菜单项回调函数"""
        monitor.stop()
        icon.stop()
        if lock_file_handle:
            lock_file_handle.close()
            try:
                os.remove(LOCK_FILE)
            except:
                pass
        sys.exit(0)

    # ============================================================================
    # 创建系统托盘菜单
    # ============================================================================

    # 定义菜单结构
    menu = pystray.Menu(
        pystray.MenuItem('📊 今日报表', on_report),
        pystray.MenuItem('📈 本周报表', on_report),
        pystray.MenuItem('📅 本月报表', on_report),
        pystray.Menu.SEPARATOR,
        pystray.MenuItem('✅ 开机自启', on_startup),
        pystray.MenuItem('❌ 取消自启', on_disable_startup),
        pystray.Menu.SEPARATOR,
        pystray.MenuItem('退出程序', on_exit)
    )

    # 创建系统托盘图标
    icon = pystray.Icon("ScreenTracker", create_icon_image(), "威震天屏幕统计", menu)
    
    # 在单独线程中运行系统托盘图标，避免阻塞主线程
    icon_thread = threading.Thread(target=icon.run, daemon=True)
    icon_thread.start()
    
    print("程序已启动，请检查系统托盘（右下角）...")
    
    # 主线程循环，保持程序运行
    try:
        while icon_thread.is_alive():
            time.sleep(1)
    except KeyboardInterrupt:
        on_exit(icon, None)
