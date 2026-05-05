# ============================================================================
# 导入库及其作用说明
# ============================================================================

# Python标准库
import os           # 操作系统接口，用于文件路径、环境变量等操作
import sys          # 系统相关功能，如命令行参数、退出程序等
import time         # 时间相关功能，用于延时、时间戳等
import sqlite3      # SQLite数据库操作，用于存储和查询屏幕使用数据
import datetime     # 日期时间处理，用于生成报表的时间范围
import threading    # 多线程支持，用于后台监控线程
import winreg       # Windows注册表操作，用于设置开机自启动
import ctypes       # C语言兼容类型，用于调用Windows API显示消息框
import multiprocessing  # 多进程支持，用于启动报表生成进程避免阻塞主线程

# 第三方库
import psutil       # 系统进程和系统利用率工具，用于获取活动进程信息
from PIL import Image, ImageDraw  # 图像处理库，用于生成系统托盘图标

# 本地模块
from smart_tracker import DatabaseManager  # 自定义数据库管理类，封装数据库操作

# --- 第三方库依赖检测 ---
# 检测是否安装了必要的Windows GUI和系统托盘库，如果缺少则提示用户安装
try:
    import win32gui      # Windows GUI操作，用于获取前台窗口句柄
    import win32process  # Windows进程操作，用于获取窗口对应的进程ID
    import win32api      # Windows API，用于获取系统输入信息（检测空闲时间）
    import pystray       # 系统托盘库，用于创建系统托盘图标和菜单
    import matplotlib.pyplot as plt  # 数据可视化库，用于生成统计图表
    
    # 设置 matplotlib 支持中文显示
    plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'Arial']
    plt.rcParams['axes.unicode_minus'] = False
except ImportError as e:
    # 如果缺少必要的库，显示错误消息框并退出程序
    ctypes.windll.user32.MessageBoxW(0, f"缺少必要库: {e}\n请运行 pip install pywin32 psutil pillow pystray matplotlib", "错误", 0x10)
    sys.exit(1)

# ============================================================================
# 总体设计思路
# ============================================================================
"""
设计路线：
1. 多线程监控：主线程负责系统托盘交互，后台线程定期检测用户活动
2. 数据持久化：使用SQLite数据库记录应用使用时长，按日期存储
3. 可视化报表：使用matplotlib生成饼图和文本列表，展示使用统计
4. 系统集成：支持开机自启、单实例运行、系统托盘常驻
5. 用户友好：应用名称美化、空闲检测、异常处理

代码难点：
1. Windows API调用：需要正确使用win32gui、win32process等库获取活动窗口信息
2. 多线程同步：监控线程与主线程的数据共享和状态管理
3. 跨进程通信：报表生成使用独立进程，避免阻塞系统托盘响应
4. 异常处理：网络断开、数据库锁定、权限不足等情况下的健壮性
5. 用户体验：系统托盘菜单、中文显示、图标生成等细节处理
"""

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

# 应用名称美化映射：将进程名转换为更友好的中文名称
APP_MAP = {
    "chrome.exe": "Google Chrome", "msedge.exe": "Edge浏览器", "firefox.exe": "Firefox",
    "wechat.exe": "微信", "wxwork.exe": "企业微信", "qq.exe": "QQ", "dingtalk.exe": "钉钉",
    "feishu.exe": "飞书", "lark.exe": "Lark",
    "code.exe": "VS Code", "pycharm64.exe": "PyCharm", "idea64.exe": "IntelliJ IDEA",
    "winword.exe": "Word", "excel.exe": "Excel", "powerpnt.exe": "PPT", "wps.exe": "WPS",
    "notion.exe": "Notion", "onenote.exe": "OneNote",
    "steam.exe": "Steam", "explorer.exe": "资源管理器", "searchhost.exe": "Windows搜索",
    "potplayer64.exe": "PotPlayer", "vlc.exe": "VLC"
}

# ============================================================================
# 核心功能类
# ============================================================================

class DataPlotter:
    """
    独立的绘图逻辑类，负责生成可视化报表
    使用matplotlib创建饼图和文本列表展示应用使用时间统计
    """
    
    @staticmethod
    def format_time_str(seconds):
        """
        将秒数转换为 xx小时xx分xx秒 的格式
        
        参数:
            seconds (int/float): 总秒数
            
        返回:
            str: 格式化的时间字符串
        """
        seconds = int(seconds)
        m, s = divmod(seconds, 60)  # 将秒转换为分钟和秒
        h, m = divmod(m, 60)        # 将分钟转换为小时和分钟
        
        # 根据时间长度选择不同的显示格式
        if h > 0:
            return f"{h}小时{m}分{s}秒"
        elif m > 0:
            return f"{m}分{s}秒"
        else:
            return f"{s}秒"

    @staticmethod
    def show_report(db_path, report_type='daily'):
        """
        显示指定类型的统计报表
        
        参数:
            db_path (str): 数据库文件路径
            report_type (str): 报表类型，可选 'daily'(日报表)、'weekly'(周报表)、'monthly'(月报表)
        """
        try:
            # 连接数据库
            conn = sqlite3.connect(db_path)
            cursor = conn.cursor()
            today = datetime.date.today()
            
            title_prefix = ""  # 报表标题前缀
            data = []          # 存储查询结果

            # 根据报表类型查询不同的数据
            if report_type == 'daily':
                # 日报表：查询当天的数据
                date_str = today.isoformat()
                title_prefix = f"今日 ({date_str})"
                cursor.execute("SELECT app_name, duration FROM usage_log WHERE date = ? ORDER BY duration DESC", (date_str,))
                data = cursor.fetchall()
            
            elif report_type == 'weekly':
                # 周报表：查询本周的数据（从周一开始）
                start_week = today - datetime.timedelta(days=today.weekday())
                title_prefix = f"本周 (自 {start_week})"
                cursor.execute('''
                    SELECT app_name, SUM(duration) FROM usage_log 
                    WHERE date >= ? GROUP BY app_name ORDER BY SUM(duration) DESC
                ''', (start_week.isoformat(),))
                data = cursor.fetchall()

            elif report_type == 'monthly':
                # 月报表：查询本月的数据（从本月1日开始）
                start_month = today.replace(day=1)
                title_prefix = f"本月 ({today.strftime('%Y-%m')})"
                cursor.execute('''
                    SELECT app_name, SUM(duration) FROM usage_log 
                    WHERE date >= ? GROUP BY app_name ORDER BY SUM(duration) DESC
                ''', (start_month.isoformat(),))
                data = cursor.fetchall()

            conn.close()  # 关闭数据库连接

            # 如果没有数据，显示提示信息并返回
            if not data:
                ctypes.windll.user32.MessageBoxW(0, "该时间段暂无数据", "提示", 0)
                return

            # --- 数据准备：为饼图准备数据 ---
            pie_labels = []   # 饼图标签（应用名称）
            pie_sizes = []    # 饼图大小（使用分钟数）
            others_sum_minutes = 0  # 其他应用的总使用时间
            
            # 计算所有数据的总秒数（用于标题显示）
            total_seconds_all = sum([row[1] for row in data])

            # 饼图数据处理（饼图用分钟计算，方便显示比例）
            top_n_pie = 8  # 饼图最多显示8个主要应用，其余合并为"其他应用"
            for i, row in enumerate(data):
                minutes = row[1] / 60  # 将秒转换为分钟
                
                if minutes < 1:  # 饼图忽略小于1分钟的应用
                    continue
                
                if i < top_n_pie:
                    # 前8个应用单独显示
                    pie_labels.append(row[0])
                    pie_sizes.append(minutes)
                else:
                    # 其他应用合并计算
                    others_sum_minutes += minutes
            
            # 如果有其他应用，添加到饼图数据中
            if others_sum_minutes > 0:
                pie_labels.append("其他应用")
                pie_sizes.append(others_sum_minutes)

            # 如果没有有效的饼图数据，显示提示信息并返回
            if not pie_sizes:
                ctypes.windll.user32.MessageBoxW(0, "有效使用时间太短，无法绘图", "提示", 0)
                return

            # 生成标题总时间字符串
            total_time_str = DataPlotter.format_time_str(total_seconds_all)
            full_title = f"{title_prefix} 总使用: {total_time_str}"

            # --- 开始绘图 ---
            # 创建包含两个子图的画布：左侧饼图，右侧文本列表
            fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 7))
            fig.canvas.manager.set_window_title(f"统计报表 - {report_type}")
            fig.suptitle(full_title, fontsize=16, fontweight='bold')

            # === 左侧：环形图 ===
            wedges, texts, autotexts = ax1.pie(pie_sizes, labels=pie_labels, autopct='%1.1f%%', startangle=90, pctdistance=0.85)
            centre_circle = plt.Circle((0,0),0.70,fc='white')  # 创建中心白色圆形，形成环形图效果
            ax1.add_artist(centre_circle)
            ax1.set_title("时间占比分布")

            # === 右侧：纯文本列表 ===
            ax2.axis('off')  # 隐藏坐标轴
            ax2.set_title("详细时长排行 (Top 15)", fontsize=14, pad=10)

            y_pos = 1.0      # 文本起始垂直位置（从顶部开始）
            line_height = 0.06  # 每行文本的高度
            
            # 显示前15个应用的详细使用时间
            for i, row in enumerate(data):
                if i >= 15:  # 只显示前15个
                    break

                app_name = row[0]        # 应用名称
                duration_sec = row[1]    # 使用秒数
                
                # 格式化时间字符串
                time_str = DataPlotter.format_time_str(duration_sec)

                # 如果应用名称太长，进行截断
                if len(app_name) > 12:
                    app_name = app_name[:11] + "..."
                
                # 调整显示格式：序号、应用名、使用时间
                display_text = f"{i+1}. {app_name:<12} —— {time_str}"
                
                # 前3名使用粗体和大字号突出显示
                font_weight = 'bold' if i < 3 else 'normal'
                font_size = 12 if i < 3 else 11
                color = '#333333'  # 文字颜色
                
                # 在指定位置添加文本
                ax2.text(0.1, y_pos, display_text, 
                         transform=ax2.transAxes, 
                         fontsize=font_size, 
                         fontweight=font_weight,
                         color=color,
                         va='top')
                
                y_pos -= line_height  # 移动到下一行

            plt.tight_layout()  # 自动调整子图参数，使之填充整个图像区域
            plt.show()  # 显示图表

        except Exception as e:
            # 如果绘图过程中出现异常，打印错误信息（在后台运行时不显示）
            print(f"Plot Error: {e}")

class MonitorThread(threading.Thread):
    """
    监控线程类，继承自threading.Thread
    负责在后台定期检测用户活动和应用使用情况
    """
    def __init__(self, db_manager):
        """
        初始化监控线程
        
        参数:
            db_manager: DatabaseManager实例，用于更新数据库
        """
        super().__init__(daemon=True)  # 设置为守护线程，主程序退出时自动结束
        self.db = db_manager  # 数据库管理器
        self.running = True   # 线程运行标志

    def get_idle_time(self):
        """
        获取系统空闲时间（用户无操作的时间）
        
        返回:
            float: 空闲时间（秒）
        """
        last_input = win32api.GetLastInputInfo()  # 获取最后一次输入的时间
        millis = win32api.GetTickCount() - last_input  # 计算从最后一次输入到现在的毫秒数
        return millis / 1000.0  # 转换为秒

    def get_active_app(self):
        """
        获取当前活动应用（前台窗口对应的应用）
        
        返回:
            str: 美化后的应用名称，如果获取失败则返回None
        """
        try:
            hwnd = win32gui.GetForegroundWindow()  # 获取前台窗口句柄
            _, pid = win32process.GetWindowThreadProcessId(hwnd)  # 获取窗口对应的进程ID
            if pid > 0:
                p = psutil.Process(pid)  # 通过进程ID获取进程对象
                raw_name = p.name().lower()  # 获取进程名并转换为小写
                # 使用APP_MAP映射美化应用名称，如果没有映射则使用原始名称（去掉.exe并首字母大写）
                return APP_MAP.get(raw_name, raw_name.replace(".exe", "").capitalize())
        except:
            # 获取失败时返回None（例如进程已结束、权限不足等情况）
            return None

    def run(self):
        """
        线程主循环，定期检测用户活动并更新数据库
        """
        while self.running:  # 循环直到running标志为False
            try:
                # 如果空闲时间小于阈值（用户正在活动）
                if self.get_idle_time() < IDLE_THRESHOLD:
                    app = self.get_active_app()  # 获取当前活动应用
                    if app:
                        self.db.update_usage(app, INTERVAL)  # 更新该应用的使用时间
                time.sleep(INTERVAL)  # 等待检测间隔
            except Exception:
                # 出现异常时继续运行（避免线程崩溃）
                pass

    def stop(self):
        """
        停止监控线程
        """
        self.running = False  # 设置运行标志为False，使循环结束

class SystemUtils:
    """
    系统工具类，提供与Windows系统相关的功能
    包括开机自启设置和单实例检查
    """
    
    @staticmethod
    def set_startup(enable=True):
        """
        设置或取消开机自启动
        
        参数:
            enable (bool): True表示设置开机自启，False表示取消
        """
        # Windows注册表中开机自启动的键路径
        key_path = r"Software\Microsoft\Windows\CurrentVersion\Run"
        app_name = "威震天屏幕统计"  # 在注册表中显示的应用名称
        script_path = os.path.abspath(sys.argv[0])  # 获取当前脚本的绝对路径
        
        # 确定Python解释器路径
        py_exe = sys.executable  # 当前Python解释器路径
        if "python.exe" in py_exe:
            # 如果当前是python.exe，尝试使用pythonw.exe（无控制台窗口）
            pyw = py_exe.replace("python.exe", "pythonw.exe")
            if os.path.exists(pyw):
                py_exe = pyw
        
        # 构建启动命令
        if script_path.endswith(".py") or script_path.endswith(".pyw"):
            # 如果是Python脚本，使用Python解释器执行
            cmd = f'"{py_exe}" "{script_path}"'
        else:
            # 如果是可执行文件，直接执行
            cmd = f'"{script_path}"'

        try:
            # 打开注册表键（当前用户）
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, key_path, 0, winreg.KEY_ALL_ACCESS)
            if enable:
                # 设置开机自启：添加注册表值
                winreg.SetValueEx(key, app_name, 0, winreg.REG_SZ, cmd)
                ctypes.windll.user32.MessageBoxW(0, "已设置为开机自启", "成功", 0)
            else:
                # 取消开机自启：删除注册表值
                try:
                    winreg.DeleteValue(key, app_name)
                    ctypes.windll.user32.MessageBoxW(0, "已取消开机自启", "成功", 0)
                except FileNotFoundError:
                    # 如果值不存在，说明之前没有设置过自启动
                    pass
            winreg.CloseKey(key)  # 关闭注册表键
        except Exception as e:
            # 如果设置失败，显示错误信息
            ctypes.windll.user32.MessageBoxW(0, f"设置失败: {e}", "错误", 0)

    @staticmethod
    def check_single_instance():
        """
        检查是否已有实例在运行，确保程序单实例运行
        
        返回:
            file: 锁文件句柄，用于程序退出时删除锁文件
        """
        if os.path.exists(LOCK_FILE):
            # 如果锁文件存在，说明可能有实例在运行
            try:
                os.remove(LOCK_FILE)  # 尝试删除锁文件（可能是上次异常退出留下的）
            except:
                # 如果无法删除，说明程序确实在运行中
                ctypes.windll.user32.MessageBoxW(0, "程序已经在运行中！请检查系统托盘。", "提示", 0)
                sys.exit(0)  # 退出当前实例
        # 创建锁文件，标记程序正在运行
        f = open(LOCK_FILE, 'w')
        return f

def create_icon_image():
    """
    创建系统托盘图标
    
    优先尝试加载 icon.png 文件作为图标，
    如果找不到文件或文件损坏，则生成一个默认的蓝色方块图标
    
    返回:
        PIL.Image.Image: 图标图像
    """
    icon_path = os.path.join(BASE_DIR, "icon.png")
    
    # 1. 尝试加载自定义图标图片
    if os.path.exists(icon_path):
        try:
            return Image.open(icon_path)
        except Exception:
            # 如果图片损坏，继续使用默认图标
            pass

    # 2. 生成默认图标（蓝色背景上的白色方块）
    w, h = 64, 64  # 图标尺寸
    img = Image.new('RGB', (w, h), color=(73, 109, 137))  # 创建蓝色背景
    d = ImageDraw.Draw(img)
    d.rectangle([16, 16, 48, 48], fill=(255, 255, 255))  # 绘制白色方块
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
    
    # 检查是否已有实例在运行，确保单实例
    lock_file_handle = SystemUtils.check_single_instance()
    
    # 初始化数据库管理器
    db = DatabaseManager(DB_PATH)
    
    # 创建并启动监控线程
    monitor = MonitorThread(db)
    monitor.start()

    # ============================================================================
    # 系统托盘菜单回调函数
    # ============================================================================

    def on_report(icon, item):
        """
        报表菜单项回调函数
        
        参数:
            icon: 系统托盘图标对象
            item: 菜单项对象，包含菜单文本
        """
        # 根据菜单项文本判断报表类型
        report_type = 'daily'
        if '周' in str(item):
            report_type = 'weekly'
        if '月' in str(item):
            report_type = 'monthly'
        
        # 使用独立进程生成报表，避免阻塞系统托盘响应
        p = multiprocessing.Process(target=DataPlotter.show_report, args=(DB_PATH, report_type))
        p.start()

    def on_startup(icon, item):
        """
        设置开机自启菜单项回调函数
        """
        SystemUtils.set_startup(True)

    def on_disable_startup(icon, item):
        """
        取消开机自启菜单项回调函数
        """
        SystemUtils.set_startup(False)

    def on_exit(icon, item):
        """
        退出程序菜单项回调函数
        
        执行清理操作：
        1. 停止监控线程
        2. 停止系统托盘图标
        3. 关闭并删除锁文件
        4. 退出程序
        """
        monitor.stop()           # 停止监控线程
        icon.stop()              # 停止系统托盘图标
        lock_file_handle.close() # 关闭锁文件句柄
        try:
            os.remove(LOCK_FILE)  # 删除锁文件
        except:
            pass
        sys.exit(0)  # 退出程序

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
    
    # 主线程循环，保持程序运行
    try:
        # 保持主线程存活，直到系统托盘线程结束
        while icon_thread.is_alive():
            time.sleep(1)
    except KeyboardInterrupt:
        # 捕获Ctrl+C中断信号，优雅退出
        on_exit(icon, None)
