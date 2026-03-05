import os
import sys
import time
import sqlite3
import datetime
import psutil
import threading
import winreg  # 用于操作注册表实现开机自启
from PIL import Image, ImageDraw # 用于生成托盘图标
import pystray # 用于系统托盘
import matplotlib.pyplot as plt # 用于绘图

# 仅在 Windows 下有效
try:
    import win32gui
    import win32process
    import win32api
    import win32con
except ImportError:
    print("请先运行 pip install pywin32")
    sys.exit(1)

# --- 配置项 ---
DB_NAME = "smart_screen_time.db"
IDLE_THRESHOLD = 60  # 闲置阈值（秒）：超过60秒无操作则不计入时间
INTERVAL = 5         # 检测间隔（秒）
APP_NAME_MAP = {     # 简单的应用名称映射（可选优化）
    "chrome.exe": "Google Chrome",
    "wechat.exe": "WeChat",
    "explorer.exe": "File Explorer",
    "winword.exe": "Word",
    "excel.exe": "Excel",
    "code.exe": "VS Code"
}

class DatabaseManager:
    def __init__(self, db_name):
        self.db_name = db_name
        self.init_db()

    def init_db(self):
        conn = sqlite3.connect(self.db_name, check_same_thread=False)
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
        if not app_name: return
        today = datetime.date.today().isoformat()
        conn = sqlite3.connect(self.db_name, check_same_thread=False)
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
            print(f"DB Error: {e}")
        finally:
            conn.close()

    def get_data(self, date_str):
        conn = sqlite3.connect(self.db_name, check_same_thread=False)
        cursor = conn.cursor()
        cursor.execute("SELECT app_name, duration FROM usage_log WHERE date = ? ORDER BY duration DESC", (date_str,))
        rows = cursor.fetchall()
        conn.close()
        return rows

class TrackerCore:
    def __init__(self, db_manager):
        self.db = db_manager
        self.running = False
        self.monitor_thread = None

    def get_idle_time(self):
        """获取系统闲置时间（秒）"""
        last_input_info = win32api.GetLastInputInfo()
        # GetTickCount() 返回系统启动后的毫秒数
        millis = win32api.GetTickCount() - last_input_info
        return millis / 1000.0

    def get_active_app(self):
        try:
            hwnd = win32gui.GetForegroundWindow()
            _, pid = win32process.GetWindowThreadProcessId(hwnd)
            if pid > 0:
                process = psutil.Process(pid)
                exe_name = process.name().lower()
                # 使用映射字典美化名称，如果没有映射则使用原名
                return APP_NAME_MAP.get(exe_name, exe_name)
        except Exception:
            pass
        return None

    def loop(self):
        while self.running:
            try:
                idle_sec = self.get_idle_time()
                
                # 只有当闲置时间小于阈值时，才记录
                if idle_sec < IDLE_THRESHOLD:
                    app_name = self.get_active_app()
                    if app_name:
                        self.db.update_usage(app_name, INTERVAL)
                else:
                    # 可以在这里打印 "User is idle" 用于调试
                    pass 
                
                time.sleep(INTERVAL)
            except Exception as e:
                print(f"Loop Error: {e}")

    def start(self):
        if not self.running:
            self.running = True
            self.monitor_thread = threading.Thread(target=self.loop, daemon=True)
            self.monitor_thread.start()

    def stop(self):
        self.running = False

class Visualizer:
    @staticmethod
    def show_daily_chart(db_manager):
        today = datetime.date.today().isoformat()
        data = db_manager.get_data(today)
        
        if not data:
            print("今日暂无数据")
            return

        # 过滤掉使用时间极短的应用（例如小于1分钟），避免饼图太乱
        filtered_data = [row for row in data if row[1] > 60]
        if not filtered_data:
            print("数据太少，无法绘图")
            return

        labels = [row[0] for row in filtered_data]
        sizes = [row[1] for row in filtered_data]
        
        # 将秒转换为分钟显示
        sizes_min = [s/60 for s in sizes]

        plt.figure(figsize=(10, 6))
        # 解决中文乱码问题 (Windows下通常是 SimHei 或 Microsoft YaHei)
        plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei'] 
        plt.rcParams['axes.unicode_minus'] = False

        plt.pie(sizes_min, labels=labels, autopct='%1.1f%%', startangle=140)
        plt.axis('equal')
        plt.title(f"今日屏幕使用统计 ({today})")
        plt.show()

class SystemUtils:
    @staticmethod
    def set_startup(enable=True):
        """设置开机自启"""
        key_path = r"Software\Microsoft\Windows\CurrentVersion\Run"
        app_name = "MyScreenTracker"
        exe_path = os.path.abspath(sys.argv[0])
        
        # 如果是 python 脚本运行，路径指向 python.exe，参数是脚本路径
        # 实际生产中建议打包成 exe 后再设置自启
        if exe_path.endswith(".py") or exe_path.endswith(".pyw"):
            # 注意：这在某些 Python 环境下可能需要调整
            cmd = f'"{sys.executable}" "{exe_path}"'
        else:
            cmd = f'"{exe_path}"'

        try:
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, key_path, 0, winreg.KEY_ALL_ACCESS)
            if enable:
                winreg.SetValueEx(key, app_name, 0, winreg.REG_SZ, cmd)
                print("已开启开机自启")
            else:
                try:
                    winreg.DeleteValue(key, app_name)
                    print("已关闭开机自启")
                except FileNotFoundError:
                    pass
            winreg.CloseKey(key)
        except Exception as e:
            print(f"设置自启失败 (可能需要管理员权限): {e}")

# --- 主程序逻辑 ---

def create_image():
    # 生成一个简单的纯色图标
    width = 64
    height = 64
    color1 = "blue"
    color2 = "white"
    image = Image.new('RGB', (width, height), color1)
    dc = ImageDraw.Draw(image)
    dc.rectangle((width // 2, 0, width, height // 2), fill=color2)
    dc.rectangle((0, height // 2, width // 2, height), fill=color2)
    return image

def main():
    db = DatabaseManager(DB_NAME)
    tracker = TrackerCore(db)
    tracker.start()

    # 定义托盘图标动作
    def on_quit(icon, item):
        tracker.stop()
        icon.stop()

    def on_show_chart(icon, item):
        # 绘图需要在主线程或者独立的GUI线程，这里简单起见直接调用
        # 注意：matplotlib 可能会阻塞托盘，实际开发最好用子进程或非阻塞方式
        Visualizer.show_daily_chart(db)

    def on_toggle_startup(icon, item):
        # 这里做一个简单的切换，实际需要检测当前状态
        SystemUtils.set_startup(True)

    menu = pystray.Menu(
        pystray.MenuItem('查看今日图表', on_show_chart),
        pystray.MenuItem('设置开机自启', on_toggle_startup),
        pystray.MenuItem('退出', on_quit)
    )

    icon = pystray.Icon("ScreenTracker", create_image(), "屏幕时间统计", menu)
    
    print("程序已启动，请检查系统托盘（右下角）...")
    icon.run()

if __name__ == "__main__":
    main()