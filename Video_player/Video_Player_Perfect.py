# ============================================================================
# 导入库说明
# ============================================================================
# 1. Python标准库
import sys      # 系统相关功能，如命令行参数、退出程序等
import os       # 操作系统接口，用于文件路径操作、目录扫描等
import time     # 时间相关功能，用于快进/快退的时间计算
import json     # JSON数据处理，用于保存和加载设置、最近播放列表

# 2. PyQt5 GUI库 - 界面组件
from PyQt5.QtWidgets import (QApplication, QWidget, QPushButton, QHBoxLayout, QVBoxLayout, 
                             QLabel, QSlider, QStyle, QFileDialog, QShortcut, QComboBox, 
                             QSpinBox, QListWidget, QListWidgetItem, QSplitter, QTabWidget, 
                             QColorDialog, QMenu, QMessageBox, QToolTip, QGridLayout, QSizePolicy, 
                             QInputDialog)
# QApplication: 应用程序主类，管理GUI程序的控制流和主要设置
# QWidget: 所有用户界面对象的基类，用于创建窗口和控件
# QPushButton: 按钮控件，用于触发操作
# QHBoxLayout/QVBoxLayout: 水平和垂直布局管理器，用于界面布局
# QLabel: 文本标签，用于显示文字信息
# QSlider: 滑块控件，用于进度条和音量控制
# QStyle: 界面样式管理，用于获取标准图标
# QFileDialog: 文件对话框，用于选择文件和文件夹
# QShortcut: 快捷键管理，用于键盘快捷键
# QComboBox: 下拉选择框，用于模式选择
# QSpinBox: 数字输入框，用于设置快进速度
# QListWidget/QListWidgetItem: 列表控件，用于显示视频文件列表
# QSplitter: 分割器，用于可调整大小的界面区域
# QTabWidget: 标签页控件，用于组织不同功能区域
# QColorDialog: 颜色选择对话框，用于选择高亮颜色
# QMenu: 右键菜单，用于上下文操作
# QMessageBox: 消息对话框，用于显示提示和警告
# QToolTip: 工具提示，用于显示悬停信息
# QGridLayout: 网格布局管理器
# QSizePolicy: 控件大小策略管理
# QInputDialog: 输入对话框，用于获取用户输入

# 3. PyQt5多媒体库 - 音视频播放功能
from PyQt5.QtMultimedia import QMediaPlayer, QMediaContent
# QMediaPlayer: 媒体播放器核心类，负责音视频播放控制
# QMediaContent: 媒体内容类，封装媒体文件路径或URL

# 4. PyQt5多媒体组件库 - 视频显示
from PyQt5.QtMultimediaWidgets import QVideoWidget
# QVideoWidget: 视频显示组件，用于在界面上显示视频画面

# 5. PyQt5核心库 - 基础功能
from PyQt5.QtCore import Qt, QUrl, QSettings, QTimer
# Qt: 核心枚举和常量，如键盘按键、对齐方式等
# QUrl: URL处理类，用于处理文件路径和网络地址
# QSettings: 应用程序设置持久化，用于保存用户偏好
# QTimer: 定时器类，用于定时执行任务和动画

# 6. PyQt5图形库 - 图形界面元素
from PyQt5.QtGui import QIcon, QKeySequence, QColor
# QIcon: 图标类，用于窗口和按钮图标
# QKeySequence: 快捷键序列，用于定义键盘快捷键
# QColor: 颜色类，用于界面颜色管理

# ============================================================================
# 总体设计思路
# ============================================================================
"""
视频播放器设计思路：

1. 设计路线：
   - 采用PyQt5框架构建跨平台桌面应用程序
   - 模块化设计：将功能分解为自定义组件（ClickableSlider、ClickableVideoWidget、FullscreenPlayer）
   - 主从架构：主播放器管理媒体播放，全屏播放器作为独立组件
   - 数据持久化：使用QSettings保存用户设置和播放历史
   - 响应式界面：支持键盘快捷键、鼠标交互和触摸操作

2. 代码难点：
   - 自定义进度条：实现点击跳转、悬停预览、拖动控制
   - 全屏切换：无缝切换视频输出，避免黑屏和闪烁
   - 智能快进：支持指数增长和阶梯式两种快进模式
   - 文件管理：视频文件扫描、去重、最近播放列表维护
   - 线程安全：定时器与UI更新的协调，避免界面卡顿
   - 错误处理：媒体加载失败、文件不存在等异常情况处理

3. 核心功能：
   - 基本播放：播放/暂停、进度控制、音量调节
   - 高级控制：智能快进/快退、播放速度调整
   - 文件管理：视频列表、最近播放、文件重命名
   - 界面定制：高亮颜色选择、全屏播放、界面布局
   - 用户体验：快捷键支持、鼠标交互、设置记忆
"""


# ============================================================================
# 1. ClickableSlider 类 - 自定义可点击进度条
# ============================================================================
"""
功能说明：
- 扩展QSlider，支持点击跳转到任意位置
- 鼠标悬停时显示时间预览
- 支持拖动和释放事件处理
- 提供时间格式化回调接口

设计要点：
1. 鼠标跟踪：启用setMouseTracking以实时获取鼠标位置
2. 点击跳转：重写mousePressEvent，计算点击位置对应的进度值
3. 悬停预览：在mouseMoveEvent中显示时间工具提示
4. 回调机制：通过_position_setter回调通知主播放器更新播放位置
"""
class ClickableSlider(QSlider):
    def __init__(self, orientation, parent=None):
        super().__init__(orientation, parent)
        self.hover_tooltip = None
        self.time_formatter = None  # 时间格式化函数
        self.setMouseTracking(True)  # 启用鼠标跟踪，用于悬停预览
        self._position_setter = None  # 位置设置回调函数
    
    def set_time_formatter(self, formatter):
        """设置时间格式化函数，用于将进度值转换为时间字符串"""
        self.time_formatter = formatter
    
    def mousePressEvent(self, event):
        """处理鼠标点击事件 - 支持点击跳转"""
        if event.button() == Qt.LeftButton:
            # 先调用父类方法，让 QSlider 处理点击（更新滑块位置）
            super().mousePressEvent(event)
            # 然后触发位置设置回调，通知播放器跳转到新位置
            if self._position_setter and self.maximum() > 0:
                self._position_setter(self.value())
        else:
            super().mousePressEvent(event)
    
    def mouseReleaseEvent(self, event):
        """处理鼠标释放事件 - 拖动结束时设置位置"""
        super().mouseReleaseEvent(event)
        if event.button() == Qt.LeftButton:
            if self._position_setter and self.maximum() > 0:
                self._position_setter(self.value())
    
    def mouseMoveEvent(self, event):
        """鼠标移动事件 - 显示悬停预览和处理拖动"""
        # 显示时间预览（仅在未拖动滑块时显示）
        if self.maximum() > 0 and self.time_formatter and not self.isSliderDown():
            x_pos = event.pos().x()
            # 计算鼠标位置对应的进度值（线性映射）
            if self.width() > 0:
                value = self.minimum() + (self.maximum() - self.minimum()) * x_pos / self.width()
                value = max(self.minimum(), min(self.maximum(), int(value)))
                
                # 格式化时间
                time_str = self.time_formatter(value)
                
                # 显示工具提示
                QToolTip.showText(event.globalPos(), time_str, self)
        
        # 调用父类方法处理拖动
        super().mouseMoveEvent(event)
    
    def leaveEvent(self, event):
        """鼠标离开事件 - 隐藏工具提示"""
        QToolTip.hideText()
        super().leaveEvent(event)
    
    def set_position_setter(self, setter):
        """设置位置设置器回调，用于通知播放器更新播放位置"""
        self._position_setter = setter



# ============================================================================
# 2. ClickableVideoWidget 类 - 可点击暂停的视频窗口
# ============================================================================
"""
功能说明：
- 扩展QVideoWidget，支持点击视频画面暂停/播放
- 提供鼠标移动回调，用于全屏模式下的控制条显示

设计要点：
1. 点击暂停：重写mousePressEvent，点击视频画面时触发播放/暂停
2. 鼠标移动检测：通过mouse_move_callback通知父组件鼠标移动事件
3. 回调机制：通过player_callback回调通知播放器切换播放状态
"""
class ClickableVideoWidget(QVideoWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.player_callback = None  # 播放/暂停回调函数
        self.mouse_move_callback = None  # 鼠标移动回调，用于全屏控制条显示
    
    def mousePressEvent(self, event):
        """处理鼠标点击事件 - 点击视频画面切换播放/暂停"""
        if event.button() == Qt.LeftButton and self.player_callback:
            self.player_callback()  # 触发暂停回调
        super().mousePressEvent(event)
    
    def mouseMoveEvent(self, event):
        """处理鼠标移动事件 - 转发给鼠标移动回调"""
        # 如果有鼠标移动回调，调用它（用于全屏模式控制条显示）
        if self.mouse_move_callback:
            self.mouse_move_callback(event)
        super().mouseMoveEvent(event)


# ============================================================================
# 3. FullscreenPlayer 类 - 全屏播放器
# ============================================================================
"""
功能说明：
- 独立的全屏播放界面，提供沉浸式观看体验
- 悬浮式控制条，支持自动隐藏/显示
- 支持键盘快捷键操作（播放/暂停、音量控制、快进/快退等）
- 智能控制条显示：鼠标靠近底部时显示，3秒无操作自动隐藏

设计要点：
1. 独立视频窗口：创建独立的ClickableVideoWidget，避免从主界面搬运导致的闪烁
2. 悬浮控制条：半透明悬浮样式，支持自适应窗口大小
3. 自动隐藏：通过QTimer实现控制条自动隐藏，提升观看体验
4. 键盘事件转发：将键盘事件转发给主窗口处理，保持快捷键一致性
5. 性能优化：设置窗口属性减少重绘，提升全屏播放流畅度
"""
class FullscreenPlayer(QWidget):
    def __init__(self, media_player, parent=None):
        super().__init__(parent)
        self.media_player = media_player  # 媒体播放器实例
        self.exit_callback = None  # 退出全屏回调函数
        
        # 1. 布局初始化 - 使用网格布局，边距为0实现全屏效果
        self.layout = QGridLayout(self)
        self.layout.setContentsMargins(0, 0, 0, 0)
        
        # 2. 创建自己独立的视频窗口 (不再从主界面搬运，避免闪烁)
        self.video_widget = ClickableVideoWidget(self)
        self.video_widget.setSizePolicy(QSizePolicy.Ignored, QSizePolicy.Ignored)  # 忽略大小策略，填满全屏
        self.video_widget.player_callback = self.toggle_play  # 点击视频切换播放/暂停
        # 绑定鼠标移动事件，用于唤醒控制条
        self.video_widget.mouse_move_callback = self.on_video_widget_mouse_move
        
        self.layout.addWidget(self.video_widget, 0, 0)  # 视频窗口占据整个网格
        
        # 3. 控制条容器 (悬浮样式) - 半透明黑色背景，圆角设计
        self.controls_container = QWidget()
        self.controls_container.setStyleSheet("""
            QWidget {
                background-color: rgba(0, 0, 0, 80);  /* 80%不透明度黑色背景 */
                border-radius: 15px;  /* 圆角设计 */
            }
        """)
        
        controls_layout = QVBoxLayout()
        controls_layout.setContentsMargins(20, 10, 20, 10)  # 内边距
        
        # 进度条 - 使用自定义的ClickableSlider
        self.progress_slider = ClickableSlider(Qt.Horizontal)
        self.progress_slider.setRange(0, 0)  # 初始范围
        self.progress_slider.set_time_formatter(self.format_time)  # 设置时间格式化函数
        self.progress_slider.set_position_setter(self.set_position_callback)  # 设置位置设置回调
        # 不连接 sliderMoved 信号，使用 ClickableSlider 的内置回调机制避免冲突
        self.progress_slider.setStyleSheet("""
            QSlider::groove:horizontal {
                height: 4px;
                background: rgba(255, 255, 255, 50);  /* 半白色轨道 */
                border-radius: 2px;
            }
            QSlider::handle:horizontal {
                width: 12px;
                height: 12px;
                background: white;  /* 白色滑块 */
                border-radius: 6px;  /* 圆形滑块 */
                margin: -4px 0;  /* 扩大点击区域 */
            }
            QSlider::handle:horizontal:hover {
                width: 14px;
                height: 14px;
                background: #0078d7;  /* 悬停时变为蓝色 */
            }
            QSlider::sub-page:horizontal {
                background: #0078d7;  /* 已播放部分为蓝色 */
                border-radius: 2px;
            }
        """)
        
        # 底部按钮布局
        btns_layout = QHBoxLayout()
        
        # 播放/暂停按钮
        self.play_btn = QPushButton()
        self.play_btn.setFixedSize(32, 32)  # 固定大小
        self.play_btn.setCursor(Qt.PointingHandCursor)  # 手型光标
        self.play_btn.setStyleSheet("background: transparent; border: none;")  # 透明背景
        self.play_btn.setIcon(self.style().standardIcon(QStyle.SP_MediaPlay))  # 播放图标
        self.play_btn.clicked.connect(self.toggle_play)  # 连接点击事件
        
        # 时间显示标签
        self.time_label = QLabel("00:00:00 / 00:00:00")
        self.time_label.setStyleSheet("color: white; font-weight: bold; font-size: 12px; background: transparent;")
        
        # 退出全屏按钮
        self.exit_btn = QPushButton("退出全屏")
        self.exit_btn.setCursor(Qt.PointingHandCursor)
        self.exit_btn.setStyleSheet("""
            QPushButton {
                background: transparent;
                color: rgba(255, 255, 255, 180);  /* 半透明白色文字 */
                border: 1px solid rgba(255, 255, 255, 50);  /* 半透明边框 */
                border-radius: 5px;
                padding: 4px 10px;
            }
            QPushButton:hover {
                background: rgba(255, 255, 255, 30);  /* 悬停时轻微背景 */
                color: white;  /* 悬停时文字变亮 */
            }
        """)
        self.exit_btn.clicked.connect(self.exit_fullscreen)  # 连接退出全屏事件
        
        # 添加按钮到布局
        btns_layout.addWidget(self.play_btn)
        btns_layout.addWidget(self.time_label)
        btns_layout.addStretch()  # 添加弹性空间
        btns_layout.addWidget(self.exit_btn)
        
        # 将进度条和按钮布局添加到控制条容器
        controls_layout.addWidget(self.progress_slider)
        controls_layout.addLayout(btns_layout)
        self.controls_container.setLayout(controls_layout)
        
        # 将控制条添加到布局（底部居中）
        self.layout.addWidget(self.controls_container, 0, 0, Qt.AlignBottom | Qt.AlignHCenter)
        self.controls_container.setFixedWidth(800)  # 初始固定宽度
        
        # 4. 计时器与设置 - 控制条自动隐藏
        self.hide_timer = QTimer()
        self.hide_timer.setSingleShot(True)  # 单次触发
        self.hide_timer.timeout.connect(self.hide_controls)  # 超时隐藏控制条
        self.controls_visible = True  # 控制条可见状态
        
        # 窗口设置
        self.setMouseTracking(True)  # 启用鼠标跟踪
        self.setWindowFlags(Qt.Window | Qt.FramelessWindowHint)  # 无边框窗口
        self.setStyleSheet("background-color: black;")  # 黑色背景
        
        # 优化：设置窗口属性，减少重绘
        self.setAttribute(Qt.WA_OpaquePaintEvent)
        self.setAttribute(Qt.WA_NoSystemBackground)
        
        # 5. 信号连接 - 连接媒体播放器信号
        self.media_player.stateChanged.connect(self.update_state)  # 播放状态变化
        self.media_player.positionChanged.connect(self.update_position)  # 播放位置变化
        self.media_player.durationChanged.connect(self.update_duration)  # 媒体时长变化


    def resizeEvent(self, event):
        if self.controls_container:
            target_width = int(self.width() * 0.8)
            self.controls_container.setFixedWidth(min(target_width, 1000))
        super().resizeEvent(event)

    # --- 逻辑控制 ---
    def toggle_play(self):
        if self.media_player.state() == QMediaPlayer.PlayingState:
            self.media_player.pause()
        else:
            self.media_player.play()
        # 不自动显示控制条

    def update_state(self, state):
        if state == QMediaPlayer.PlayingState:
            self.play_btn.setIcon(self.style().standardIcon(QStyle.SP_MediaPause))
        else:
            self.play_btn.setIcon(self.style().standardIcon(QStyle.SP_MediaPlay))

    def update_position(self, position):
        if not self.progress_slider.isSliderDown():
            self.progress_slider.setValue(position)
        dur = self.media_player.duration()
        self.time_label.setText(f"{self.format_time(position)} / {self.format_time(dur)}")

    def update_duration(self, duration):
        self.progress_slider.setRange(0, duration)

    def set_position_callback(self, position):
        self.media_player.setPosition(position)
        # 拖动进度条时显示控制条
        self.show_controls()

    def format_time(self, ms):
        seconds = (ms // 1000) % 60
        minutes = (ms // 60000)
        return f"{minutes:02d}:{seconds:02d}"

    def show_controls(self):
        self.controls_container.show()
        self.controls_visible = True
        self.hide_timer.start(3000)

    def hide_controls(self):
        self.controls_container.hide()
        self.controls_visible = False

    def on_video_widget_mouse_move(self, event):
        # 只有鼠标靠近底部时才显示控制条
        if event.pos().y() > self.video_widget.height() - 150:
            self.show_controls()

    def mouseMoveEvent(self, event):
        # 只有鼠标靠近底部时才显示控制条
        if event.pos().y() > self.height() - 150:
            self.show_controls()
        super().mouseMoveEvent(event)

    def exit_fullscreen(self):
        if self.exit_callback:
            self.exit_callback()

    def keyPressEvent(self, event):
        key = event.key()
        if key in (Qt.Key_Escape, Qt.Key_F):
            self.exit_fullscreen()
        elif key == Qt.Key_Space:
            self.toggle_play()
            # 空格键不显示控制条
        elif key in (Qt.Key_W, Qt.Key_Up, Qt.Key_PageUp):
            # 音量增加
            parent = self.parent()
            if parent and hasattr(parent, 'keyPressEvent'):
                parent.keyPressEvent(event)
            # 音量控制不自动显示控制条
        elif key in (Qt.Key_S, Qt.Key_Down, Qt.Key_PageDown):
            # 音量减少
            parent = self.parent()
            if parent and hasattr(parent, 'keyPressEvent'):
                parent.keyPressEvent(event)
            # 音量控制不自动显示控制条
        elif key in (Qt.Key_Left, Qt.Key_Right, Qt.Key_Home, Qt.Key_End, Qt.Key_A, Qt.Key_D):
            # 使用主窗口的快进/快退逻辑
            parent = self.parent()
            if parent and hasattr(parent, 'keyPressEvent'):
                parent.keyPressEvent(event)
            # 方向键不自动显示控制条
        elif key in (Qt.Key_1, Qt.Key_2, Qt.Key_3, Qt.Key_0):
            # 使用主窗口的快速跳转逻辑
            parent = self.parent()
            if parent and hasattr(parent, 'keyPressEvent'):
                parent.keyPressEvent(event)
            # 数字键不自动显示控制条
    
    def keyReleaseEvent(self, event):
        key = event.key()
        if key in (Qt.Key_Left, Qt.Key_Right, Qt.Key_Home, Qt.Key_End, Qt.Key_A, Qt.Key_D):
            # 转发到主窗口处理
            parent = self.parent()
            if parent and hasattr(parent, 'keyReleaseEvent'):
                parent.keyReleaseEvent(event)
        super().keyReleaseEvent(event)

# ============================================================================
# 4. AudioVideoPlayer 类 - 主播放器类
# ============================================================================
"""
功能说明：
- 视频播放器的主界面类，集成所有播放控制功能
- 支持视频文件管理、播放控制、快捷键操作、全屏播放等
- 提供智能快进/快退、播放速度调整、高亮颜色定制等高级功能

设计要点：
1. 智能快进系统：支持指数增长和阶梯式两种快进模式，按键时间越长速度越快
2. 文件管理：自动扫描视频文件，支持最近播放列表和文件重命名
3. 数据持久化：使用QSettings保存用户设置、播放历史和播放位置
4. 全屏切换：无缝切换全屏播放，避免黑屏和闪烁
5. 键盘快捷键：全面的快捷键支持，提升操作效率
6. 界面定制：支持高亮颜色选择，个性化界面体验
"""
class AudioVideoPlayer(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("漂亮播放器") 
        self.resize(1200, 700)  # 初始窗口大小
        
        # 1. 快进/快退系统变量
        self._key_press_times = {}  # 记录按键按下时间，用于计算按住时长
        self._active_seek_keys = set()  # 记录所有正在按下的快进/快退键（集合）
        self.playback_rate = 1.0  # 播放倍速（支持快捷切换）
        self.seek_mode = 'exponential'  # 非线性放大倍率设置：'exponential' 或 'stepped'
        self.seek_base_ms = 2000  # 基础步长（毫秒）
        self.seek_speed_ms_per_sec = 5000  # 默认快进倍速（毫秒/秒）
        self._seek_steps = [(0, 2000), (1, 5000), (3, 10000), (6, 30000)]  # 阶梯模式阈值
        
        # 2. 计时器系统
        self._seek_timer = QTimer()  # 用于连续快进/快退
        self._seek_timer.timeout.connect(self._on_seek_timer)
        self._seek_timer.setInterval(30)  # 30ms 更新一次，提高流畅度
        self._save_position_timer = QTimer()  # 用于定期保存播放位置
        self._save_position_timer.timeout.connect(self.save_playback_position)
        self._save_position_timer.setInterval(5000)  # 每5秒保存一次播放位置
        
        # 3. 文件管理变量
        self._recent_files = []  # 最近播放列表
        self.video_extensions = {'.mp4', '.avi', '.wmv', '.mkv', '.mov', '.flv', '.m4v', '.webm', 
                                 '.mpg', '.mpeg', '.m2v', '.m4v', '.3gp', '.3g2', '.f4v', '.f4p', 
                                 '.f4a', '.f4b', '.rm', '.rmvb', '.asf', '.ts', '.mts', '.m2ts'}  # 视频文件扩展名
        self.current_playing_file = None  # 当前播放文件路径
        self.video_source_dir = None  # 视频源目录（用户选择的扫描目录）
        
        # 4. 全屏播放器
        self.fullscreen_player = None
        self.is_fullscreen = False
        
        # 5. 界面配置
        self.highlight_color = QColor(0, 120, 215)  # 高亮颜色配置（默认Windows蓝色）
        self.default_volume = 50  # 默认音量
        
        # 6. 读取持久化设置（如果存在）
        try:
            settings = QSettings('PrettyPlayer', 'PrettyPlayer')  # 应用程序设置
            self.seek_mode = settings.value('seek_mode', self.seek_mode)
            self.seek_speed_ms_per_sec = int(settings.value('seek_speed', self.seek_speed_ms_per_sec))
            
            # 读取最近播放列表
            recent_str = settings.value('recent_files', '[]')
            try:
                self._recent_files = json.loads(recent_str)
            except:
                self._recent_files = []
            
            # 读取上次播放的文件和位置
            self.last_played_file = settings.value('last_played_file', '')
            self.last_played_position = int(settings.value('last_played_position', 0))
            
            # 读取高亮颜色
            highlight_color_str = settings.value('highlight_color', '')
            if highlight_color_str:
                try:
                    # 从字符串恢复颜色（格式：r,g,b）
                    r, g, b = map(int, highlight_color_str.split(','))
                    self.highlight_color = QColor(r, g, b)
                except:
                    pass
            
            # 读取视频源目录
            saved_video_dir = settings.value('video_source_dir', '')
            if saved_video_dir and os.path.isdir(saved_video_dir):
                self.video_source_dir = saved_video_dir
            
            # 读取音量设置
            self.default_volume = int(settings.value('volume', self.default_volume))
        except Exception:
            settings = None
            self.last_played_file = ''
            self.last_played_position = 0
        
        self._settings = settings  # 保存设置对象引用
        
        # 7. 初始化界面和功能
        self.init_ui()  # 初始化用户界面
        self.check_if_opened_with_file()  # 检查是否通过命令行参数打开文件
        
        # 启动后恢复上次播放位置（延迟500ms确保界面加载完成）
        QTimer.singleShot(500, self.restore_last_position)


    def init_ui(self):
        # 设置图标路径 (智能识别)
        if getattr(sys, 'frozen', False):
            base_path = os.path.dirname(sys.executable)
        else:
            base_path = os.path.dirname(os.path.abspath(__file__))
        
        # 尝试多个可能的图标文件名
        icon_names = ['nap_cn.ico', 'favicon.ico', 'player_icon.ico', 'icon.ico', 'app_icon.ico']
        icon_path = None
        for icon_name in icon_names:
            temp_path = os.path.join(base_path, icon_name)
            if os.path.isfile(temp_path):
                icon_path = temp_path
                break
        
        # 如果找到图标文件，设置窗口图标
        if icon_path:
            self.setWindowIcon(QIcon(icon_path))

        # 媒体组件
        self.mediaPlayer = QMediaPlayer(None, QMediaPlayer.VideoSurface)
        self.video_widget = ClickableVideoWidget()  # 使用可点击的视频窗口
        self.video_widget.player_callback = self.play_video  # 绑定点击暂停回调
        self.mediaPlayer.setVideoOutput(self.video_widget)
        
        # 设置默认音量
        self.mediaPlayer.setVolume(self.default_volume)

        # 按钮
        self.playBtn = QPushButton()
        self.playBtn.setEnabled(False)
        self.playBtn.setIcon(self.style().standardIcon(QStyle.SP_MediaPlay))
        self.playBtn.clicked.connect(self.play_video)

        openBtn = QPushButton("打开视频")
        openBtn.clicked.connect(self.open_file_dialog)
        
        selectFolderBtn = QPushButton("选择文件夹")
        selectFolderBtn.clicked.connect(self.select_video_folder)
        
        refreshBtn = QPushButton("刷新列表")
        refreshBtn.clicked.connect(self.refresh_video_list)

        # 模式下拉（指数 / 阶梯），并恢复上次选择
        self.modeCombo = QComboBox()
        self.modeCombo.addItem('指数', 'exponential')
        self.modeCombo.addItem('阶梯', 'stepped')
        current_index = 0 if self.seek_mode == 'exponential' else 1
        self.modeCombo.setCurrentIndex(current_index)
        self.modeCombo.setToolTip('切换快进/快退的放大倍率模式（M 快捷键）')
        self.modeCombo.currentIndexChanged.connect(self.on_mode_combo_changed)

        # 默认倍速设置（单位：毫秒）
        self.speedLabel = QLabel("倍速(ms):")
        self.speedSpinBox = QSpinBox()
        self.speedSpinBox.setRange(1000, 30000)
        self.speedSpinBox.setValue(self.seek_speed_ms_per_sec)
        self.speedSpinBox.setSingleStep(1000)
        self.speedSpinBox.setToolTip('快进/快退的基础速度（毫秒/秒）')
        self.speedSpinBox.valueChanged.connect(self.on_speed_changed)

        # --- 自定义的 ClickableSlider ---
        self.slider = ClickableSlider(Qt.Horizontal)
        self.slider.setRange(0, 0)
        self.slider.set_time_formatter(self.format_time)  # 设置时间格式化函数
        self.slider.set_position_setter(self.set_position)  # 设置位置设置器
        # 移除sliderMoved连接，使用直接回调避免冲突

        # 时间显示标签（当前时间 / 总时长）
        self.timeLabel = QLabel("00:00:00 / 00:00:00")
        self.timeLabel.setMinimumWidth(160)
        self.timeLabel.setAlignment(Qt.AlignCenter)

        self.label = QLabel("准备就绪")

        # 倍速按钮
        normalSpeedBtn = QPushButton("正常")
        normalSpeedBtn.clicked.connect(lambda: self.set_playback_rate(1.0))
        speed15Btn = QPushButton("1.5x")
        speed15Btn.clicked.connect(lambda: self.set_playback_rate(1.5))
        speed2Btn = QPushButton("2x")
        speed2Btn.clicked.connect(lambda: self.set_playback_rate(2.0))
        
        # 颜色选择按钮
        colorBtn = QPushButton("高亮颜色")
        colorBtn.clicked.connect(self.choose_highlight_color)

        # 控制条布局
        hbox = QHBoxLayout()
        hbox.addWidget(openBtn)
        hbox.addWidget(self.playBtn)
        hbox.addWidget(normalSpeedBtn)
        hbox.addWidget(speed15Btn)
        hbox.addWidget(speed2Btn)
        hbox.addWidget(colorBtn)
        hbox.addStretch()
        hbox.addWidget(self.modeCombo)
        hbox.addWidget(self.speedLabel)
        hbox.addWidget(self.speedSpinBox)
        hbox.addWidget(self.slider, 1)
        hbox.addWidget(self.timeLabel)

        # 文件列表（标签页：视频列表 + 最近播放）
        self.fileListTab = QTabWidget()
        
        # 视频列表
        self.videoListWidget = QListWidget()
        self.videoListWidget.setSelectionMode(self.videoListWidget.SingleSelection)  # 单选模式
        self.videoListWidget.setDefaultDropAction(Qt.IgnoreAction)  # 禁用自动滚动
        self.videoListWidget.setAutoScroll(False)  # 禁用自动滚动
        self.videoListWidget.setContextMenuPolicy(Qt.CustomContextMenu)  # 启用右键菜单
        self.videoListWidget.customContextMenuRequested.connect(self.show_video_list_menu)
        self.videoListWidget.itemClicked.connect(self.on_video_list_clicked)
        self.fileListTab.addTab(self.videoListWidget, "视频列表")
        
        # 最近播放
        self.recentListWidget = QListWidget()
        self.recentListWidget.setSelectionMode(self.recentListWidget.SingleSelection)  # 单选模式
        self.recentListWidget.setDefaultDropAction(Qt.IgnoreAction)  # 禁用自动滚动
        self.recentListWidget.setAutoScroll(False)  # 禁用自动滚动
        self.recentListWidget.setContextMenuPolicy(Qt.CustomContextMenu)  # 启用右键菜单
        self.recentListWidget.customContextMenuRequested.connect(self.show_recent_list_menu)
        self.recentListWidget.itemClicked.connect(self.on_recent_list_clicked)
        self.fileListTab.addTab(self.recentListWidget, "最近播放")

        # 左侧文件列表区
        leftLayout = QVBoxLayout()
        leftLayout.setContentsMargins(5, 5, 5, 5)
        leftLayout.addWidget(refreshBtn)
        leftLayout.addWidget(selectFolderBtn)
        leftLayout.addWidget(self.fileListTab)

        # 右侧视频播放区
        self.rightLayout = QVBoxLayout()
        self.rightLayout.setContentsMargins(5, 5, 5, 5)
        self.rightLayout.addWidget(self.video_widget, 1)
        self.rightLayout.addLayout(hbox)
        
        # 分割器
        splitter = QSplitter(Qt.Horizontal)
        
        leftWidget = QWidget()
        leftWidget.setLayout(leftLayout)
        
        rightWidget = QWidget()
        rightWidget.setLayout(self.rightLayout)
        
        splitter.addWidget(leftWidget)
        splitter.addWidget(rightWidget)
        splitter.setStretchFactor(0, 0)
        splitter.setStretchFactor(1, 1)
        splitter.setSizes([200, 1000])
        
        # 主布局
        mainLayout = QVBoxLayout()
        mainLayout.setContentsMargins(0, 0, 0, 0)
        mainLayout.addWidget(self.label)
        mainLayout.addWidget(splitter, 1)
        
        self.setLayout(mainLayout)

        # 确保窗口接收键盘事件，避免 QVideoWidget 抢焦点
        self.setFocusPolicy(Qt.StrongFocus)
        self.video_widget.setFocusPolicy(Qt.NoFocus)

        # 快捷键：按 M 切换模式（仍调用切换函数，让 combobox 保持同步）
        try:
            shortcut = QShortcut(QKeySequence("M"), self)
            shortcut.activated.connect(self.toggle_seek_mode)
        except Exception:
            pass

        # 连接
        self.mediaPlayer.stateChanged.connect(self.media_state_changed)
        self.mediaPlayer.positionChanged.connect(self.position_changed)
        self.mediaPlayer.durationChanged.connect(self.duration_changed)
        self.mediaPlayer.error.connect(self.handle_errors)
        
        # 初始化列表
        self.refresh_video_list()
        self.refresh_recent_list()

    def play_specific_file(self, file_path):
        self.mediaPlayer.setMedia(QMediaContent(QUrl.fromLocalFile(file_path)))
        self.playBtn.setEnabled(True)
        self.label.setText(os.path.basename(file_path))
        self.playBtn.click()
        # 记录当前播放文件
        self.current_playing_file = file_path
        # 保存当前播放文件
        self.save_current_file()
        # 启动定期保存播放位置的计时器
        self._save_position_timer.start()
        # 用 QTimer 延迟高亮，确保UI已更新
        QTimer.singleShot(100, self.highlight_current_playing)
        # 更新最近播放列表
        self.add_to_recent(file_path)

    def add_to_recent(self, file_path):
        """添加文件到最近播放列表"""
        # 移除已存在的相同文件
        if file_path in self._recent_files:
            self._recent_files.remove(file_path)
        # 插入到最前
        self._recent_files.insert(0, file_path)
        # 保留最近 20 个
        self._recent_files = self._recent_files[:20]
        # 保存
        try:
            if self._settings is not None:
                self._settings.setValue('recent_files', json.dumps(self._recent_files))
        except Exception:
            pass
        # 刷新 UI
        self.refresh_recent_list()

    def select_video_folder(self):
        """选择视频源文件夹"""
        folder = QFileDialog.getExistingDirectory(self, "选择视频文件夹")
        if folder:
            # 规范化路径，确保使用正确的路径分隔符
            folder = os.path.normpath(folder)
            self.video_source_dir = folder
            # 保存视频源目录到设置
            try:
                if self._settings is not None:
                    self._settings.setValue('video_source_dir', folder)
                    self.label.setText(f"视频源目录已保存: {folder}")
            except Exception:
                pass
            self.refresh_video_list()

    def refresh_video_list(self):
        """扫描当前目录和常见位置的视频文件"""
        # 清空列表
        self.videoListWidget.clear()
        
        # 扫描位置列表
        scan_paths = []
        
        # 如果用户指定了视频源目录，优先只扫描该目录（避免扫描整个用户目录导致重复）
        if self.video_source_dir:
            scan_paths.append(self.video_source_dir)
            # 如果指定了源目录，不扫描用户主目录（避免重复）
            # 只添加当前播放文件的目录（如果不同）
            if hasattr(self, 'mediaPlayer') and self.mediaPlayer.currentMedia().canonicalUrl().isValid():
                last_dir = os.path.dirname(self.mediaPlayer.currentMedia().canonicalUrl().toLocalFile())
                if last_dir and os.path.normpath(last_dir) != os.path.normpath(self.video_source_dir):
                    scan_paths.append(last_dir)
        else:
            # 如果没有指定源目录，才扫描默认位置
            scan_paths.extend([
                os.path.dirname(os.path.abspath(__file__)),  # 脚本目录
            ])
            # 如果有开放的视频，添加其目录
            if hasattr(self, 'mediaPlayer') and self.mediaPlayer.currentMedia().canonicalUrl().isValid():
                last_dir = os.path.dirname(self.mediaPlayer.currentMedia().canonicalUrl().toLocalFile())
                if last_dir and last_dir not in scan_paths:
                    scan_paths.insert(0, last_dir)
        
        # 使用两个集合：一个用于路径去重，一个用于文件名去重
        seen_paths = set()  # 用于路径去重（规范化路径）
        seen_filenames = set()  # 用于文件名去重（规范化文件名）
        video_files = []  # 最终文件列表
        
        for path in scan_paths:
            if os.path.isdir(path):
                try:
                    for filename in os.listdir(path):
                        filepath = os.path.join(path, filename)
                        if os.path.isfile(filepath):
                            ext = os.path.splitext(filename)[1].lower()
                            if ext in self.video_extensions:
                                # 先规范化路径，获取真实路径
                                try:
                                    real_path = os.path.realpath(filepath)
                                    normalized_path = os.path.normpath(real_path).lower()
                                except:
                                    # 如果realpath失败，使用abspath
                                    normalized_path = os.path.normpath(os.path.abspath(filepath)).lower()
                                
                                # 如果路径已经存在，跳过（同一个文件）
                                if normalized_path in seen_paths:
                                    continue
                                
                                # 规范化文件名用于去重（转小写、去除空格）
                                filename_normalized = filename.lower().strip()
                                
                                # 如果这个文件名已经添加过，直接跳过
                                if filename_normalized in seen_filenames:
                                    continue
                                
                                # 添加到集合中
                                seen_paths.add(normalized_path)
                                seen_filenames.add(filename_normalized)
                                video_files.append(filepath)
                except Exception:
                    continue
        
        # 对文件列表进行排序
        video_files = sorted(video_files)
        
        # 添加到列表（已经去重，直接添加即可，最多显示 100 个）
        for i, fpath in enumerate(video_files[:100]):
            filename = os.path.basename(fpath)
            
            # 最后一次检查：确保列表中没有重复的显示名称
            display_name_normalized = filename.lower().strip()
            is_duplicate = False
            for j in range(self.videoListWidget.count()):
                existing_item = self.videoListWidget.item(j)
                if existing_item:
                    existing_text = existing_item.text().lower().strip()
                    if existing_text == display_name_normalized:
                        is_duplicate = True
                        break
            
            if is_duplicate:
                continue
            
            item = QListWidgetItem(filename)
            item.setData(Qt.UserRole, fpath)
            # 设置工具提示显示完整路径
            item.setToolTip(f"完整路径: {fpath}")
            self.videoListWidget.addItem(item)

    def refresh_recent_list(self):
        """刷新最近播放列表"""
        self.recentListWidget.clear()
        for fpath in self._recent_files:
            if os.path.isfile(fpath):
                item = QListWidgetItem(os.path.basename(fpath))
                item.setData(Qt.UserRole, fpath)
                # 设置工具提示显示完整路径
                item.setToolTip(fpath)
                self.recentListWidget.addItem(item)

    def on_video_list_clicked(self, item):
        """视频列表项被点击"""
        # 保存当前滚动位置
        scroll_pos = self.videoListWidget.verticalScrollBar().value()
        
        fpath = item.data(Qt.UserRole)
        if fpath and os.path.isfile(fpath):
            self.play_specific_file(fpath)
        
        # 点击后立即取消选择，防止自动滚动
        self.videoListWidget.clearSelection()
        
        # 恢复滚动位置
        self.videoListWidget.verticalScrollBar().setValue(scroll_pos)

    def on_recent_list_clicked(self, item):
        """最近播放列表项被点击"""
        # 保存当前滚动位置
        scroll_pos = self.recentListWidget.verticalScrollBar().value()
        
        fpath = item.data(Qt.UserRole)
        if fpath and os.path.isfile(fpath):
            self.play_specific_file(fpath)
        else:
            # 文件已删除，从列表移除
            try:
                self._recent_files.remove(fpath)
                if self._settings is not None:
                    self._settings.setValue('recent_files', json.dumps(self._recent_files))
            except Exception:
                pass
            self.refresh_recent_list()
        
        # 点击后立即取消选择，防止自动滚动
        self.recentListWidget.clearSelection()
        
        # 恢复滚动位置
        self.recentListWidget.verticalScrollBar().setValue(scroll_pos)

    def show_video_list_menu(self, position):
        """显示视频列表右键菜单"""
        item = self.videoListWidget.itemAt(position)
        if item is None:
            return
        
        menu = QMenu(self)
        rename_action = menu.addAction("重命名视频文件")
        rename_action.triggered.connect(lambda: self.rename_video_file(item))
        menu.addSeparator()
        delete_action = menu.addAction("从列表中删除")
        delete_action.triggered.connect(lambda: self.delete_item_from_video_list(item))
        menu.exec_(self.videoListWidget.mapToGlobal(position))

    def show_recent_list_menu(self, position):
        """显示最近播放列表右键菜单"""
        item = self.recentListWidget.itemAt(position)
        if item is None:
            return
        
        menu = QMenu(self)
        rename_action = menu.addAction("重命名视频文件")
        rename_action.triggered.connect(lambda: self.rename_video_file(item, is_recent=True))
        menu.addSeparator()
        delete_action = menu.addAction("从列表中删除")
        delete_action.triggered.connect(lambda: self.delete_item_from_recent_list(item))
        menu.exec_(self.recentListWidget.mapToGlobal(position))

    def rename_video_file(self, item, is_recent=False):
        """重命名视频文件"""
        if item is None:
            return
        
        old_filepath = item.data(Qt.UserRole)
        if not old_filepath or not os.path.isfile(old_filepath):
            QMessageBox.warning(self, '错误', '文件不存在！')
            return
        
        old_filename = os.path.basename(old_filepath)
        old_dir = os.path.dirname(old_filepath)
        name_without_ext, ext = os.path.splitext(old_filename)
        
        # 弹出输入对话框
        new_name, ok = QInputDialog.getText(
            self,
            '重命名视频文件',
            f'请输入新的文件名（不含扩展名）：\n当前：{name_without_ext}',
            text=name_without_ext
        )
        
        if not ok or not new_name or new_name.strip() == '':
            return
        
        new_name = new_name.strip()
        
        # 检查文件名是否合法
        invalid_chars = ['<', '>', ':', '"', '/', '\\', '|', '?', '*']
        for char in invalid_chars:
            if char in new_name:
                QMessageBox.warning(
                    self,
                    '无效文件名',
                    f'文件名不能包含以下字符：< > : " / \\ | ? *'
                )
                return
        
        # 如果名称没有改变，直接返回
        if new_name == name_without_ext:
            return
        
        # 构建新文件路径
        new_filename = new_name + ext
        new_filepath = os.path.join(old_dir, new_filename)
        
        # 检查新文件名是否已存在
        if os.path.exists(new_filepath):
            QMessageBox.warning(
                self,
                '文件已存在',
                f'文件 "{new_filename}" 已存在，请使用其他名称。'
            )
            return
        
        # 确认重命名
        reply = QMessageBox.question(
            self,
            '确认重命名',
            f'确定要将文件重命名吗？\n\n原名称：{old_filename}\n新名称：{new_filename}',
            QMessageBox.Yes | QMessageBox.No,
            QMessageBox.No
        )
        
        if reply != QMessageBox.Yes:
            return
        
        # 如果正在播放这个文件，先完全停止并释放文件
        is_current_playing = (self.current_playing_file and 
                             os.path.normpath(os.path.abspath(old_filepath)) == 
                             os.path.normpath(os.path.abspath(self.current_playing_file)))
        
        was_playing = False
        current_position = 0
        
        if is_current_playing:
            was_playing = self.mediaPlayer.state() == QMediaPlayer.PlayingState
            current_position = self.mediaPlayer.position()
            # 完全停止播放并清空媒体，释放文件句柄
            self.mediaPlayer.stop()
            self.mediaPlayer.setMedia(QMediaContent())
            # 等待文件句柄完全释放
            import time
            time.sleep(0.3)
        
        # 执行重命名
        try:
            # 确保路径使用正确的编码
            # 在 Windows 上，使用原始字符串避免编码问题
            old_filepath_abs = os.path.abspath(old_filepath)
            new_filepath_abs = os.path.abspath(new_filepath)
            
            # 检查文件是否被占用
            if is_current_playing:
                # 等待一小段时间确保文件句柄被释放
                import time
                time.sleep(0.1)
            
            # 使用 Windows API 进行重命名（更好地处理中文）
            try:
                import ctypes
                kernel32 = ctypes.windll.kernel32
                # MoveFileW 是 Windows API，专门处理 Unicode 文件名
                result = kernel32.MoveFileW(old_filepath_abs, new_filepath_abs)
                if not result:
                    raise OSError(f"MoveFileW failed with error code: {kernel32.GetLastError()}")
            except Exception as e:
                # 如果 Windows API 失败，尝试使用 shutil
                import shutil
                shutil.move(old_filepath_abs, new_filepath_abs)
            
            # 更新列表项
            item.setText(new_filename)
            item.setData(Qt.UserRole, new_filepath)
            item.setToolTip(f"完整路径: {new_filepath}")
            
            # 更新最近播放列表中的路径
            if old_filepath in self._recent_files:
                index = self._recent_files.index(old_filepath)
                self._recent_files[index] = new_filepath
                try:
                    if self._settings is not None:
                        self._settings.setValue('recent_files', json.dumps(self._recent_files))
                except Exception:
                    pass
                # 刷新最近播放列表
                self.refresh_recent_list()
            
            # 如果是当前播放的文件，重新加载
            if is_current_playing:
                self.current_playing_file = new_filepath
                self.mediaPlayer.setMedia(QMediaContent(QUrl.fromLocalFile(new_filepath)))
                self.mediaPlayer.setPosition(current_position)
                if was_playing:
                    self.mediaPlayer.play()
                self.label.setText(new_filename)
                # 保存新的文件路径
                self.save_current_file()
            
            self.label.setText(f"文件已重命名: {new_filename}")
            
        except Exception as e:
            QMessageBox.critical(
                self,
                '重命名失败',
                f'无法重命名文件：{str(e)}'
            )

    def delete_item_from_video_list(self, item):
        """从视频列表中删除项"""
        if item is None:
            return
        
        filename = item.text()
        reply = QMessageBox.question(
            self,
            '确认删除',
            f'确定要从列表中删除 "{filename}" 吗？\n（不会删除文件本身）',
            QMessageBox.Yes | QMessageBox.No,
            QMessageBox.No
        )
        
        if reply == QMessageBox.Yes:
            # 获取当前滚动位置
            scroll_pos = self.videoListWidget.verticalScrollBar().value()
            # 删除列表项
            row = self.videoListWidget.row(item)
            self.videoListWidget.takeItem(row)
            # 恢复滚动位置
            self.videoListWidget.verticalScrollBar().setValue(scroll_pos)

    def delete_item_from_recent_list(self, item):
        """从最近播放列表中删除项"""
        if item is None:
            return
        
        filename = item.text()
        reply = QMessageBox.question(
            self,
            '确认删除',
            f'确定要从最近播放列表中删除 "{filename}" 吗？',
            QMessageBox.Yes | QMessageBox.No,
            QMessageBox.No
        )
        
        if reply == QMessageBox.Yes:
            # 获取文件路径
            fpath = item.data(Qt.UserRole)
            # 从最近播放列表中移除
            if fpath in self._recent_files:
                self._recent_files.remove(fpath)
                # 保存设置
                try:
                    if self._settings is not None:
                        self._settings.setValue('recent_files', json.dumps(self._recent_files))
                except Exception:
                    pass
            # 刷新列表
            self.refresh_recent_list()

    def save_current_file(self):
        """保存当前播放文件"""
        try:
            if self._settings is not None and self.current_playing_file:
                self._settings.setValue('last_played_file', self.current_playing_file)
        except Exception:
            pass
    
    def save_playback_position(self):
        """保存当前播放位置"""
        try:
            if self._settings is not None and self.current_playing_file:
                position = self.mediaPlayer.position()
                self._settings.setValue('last_played_position', position)
                self._settings.setValue('last_played_file', self.current_playing_file)
        except Exception:
            pass
    
    def restore_last_position(self):
        """恢复上次播放位置"""
        try:
            if (hasattr(self, 'last_played_file') and self.last_played_file and 
                os.path.isfile(self.last_played_file)):
                # 如果有命令行参数，优先使用命令行参数
                if len(sys.argv) > 1:
                    return
                
                # 播放上次的文件
                self.mediaPlayer.setMedia(QMediaContent(QUrl.fromLocalFile(self.last_played_file)))
                self.playBtn.setEnabled(True)
                self.label.setText(os.path.basename(self.last_played_file))
                self.current_playing_file = self.last_played_file
                
                # 恢复播放位置
                if self.last_played_position > 0:
                    # 连接durationChanged信号，确保在媒体加载完成后设置位置
                    def on_duration_changed(duration):
                        if duration > 0 and self.last_played_position > 0:
                            # 确保位置不超过总时长
                            pos = min(self.last_played_position, duration)
                            self.mediaPlayer.setPosition(pos)
                            self.slider.setValue(pos)
                            # 自动播放
                            self.mediaPlayer.play()
                            self.mediaPlayer.durationChanged.disconnect()
                    
                    self.mediaPlayer.durationChanged.connect(on_duration_changed)
                else:
                    # 如果没有保存的位置，直接播放
                    QTimer.singleShot(300, lambda: self.mediaPlayer.play())
                
                # 高亮当前播放的文件
                QTimer.singleShot(300, self.highlight_current_playing)
                self.label.setText(f"已恢复上次播放: {os.path.basename(self.last_played_file)}")
        except Exception:
            pass

    def check_if_opened_with_file(self):
        if len(sys.argv) > 1:
            file_path = sys.argv[1]
            self.play_specific_file(file_path)

    def open_file_dialog(self):
        fileName, _ = QFileDialog.getOpenFileName(self, "选择视频", ".", "Video Files (*.mp4 *.avi *.wmv *.mkv)")
        if fileName != '':
            self.play_specific_file(fileName)

    def play_video(self):
        if self.mediaPlayer.state() == QMediaPlayer.PlayingState:
            self.mediaPlayer.pause()
        else:
            self.mediaPlayer.play()

    def media_state_changed(self, state):
        if self.mediaPlayer.state() == QMediaPlayer.PlayingState:
            self.playBtn.setIcon(self.style().standardIcon(QStyle.SP_MediaPause))
            # 播放时保持高亮，用 QTimer 延迟
            QTimer.singleShot(50, self.highlight_current_playing)
        else:
            self.playBtn.setIcon(self.style().standardIcon(QStyle.SP_MediaPlay))

    def format_time(self, milliseconds):
        """格式化时间显示（毫秒转 hh:mm:ss）"""
        total_seconds = milliseconds // 1000
        hours = total_seconds // 3600
        minutes = (total_seconds % 3600) // 60
        seconds = total_seconds % 60
        return f"{hours:02d}:{minutes:02d}:{seconds:02d}"

    def position_changed(self, position):
        # 只有当用户没有在拖动的时候，才自动更新进度条
        # 防止用户拖动时，进度条跟用户打架
        if not self.slider.isSliderDown():
            self.slider.setValue(position)
        
        # 更新时间显示
        if not self.is_fullscreen:
            current_time = self.format_time(position)
            total_duration = self.mediaPlayer.duration()
            if total_duration > 0:
                total_time = self.format_time(total_duration)
                self.timeLabel.setText(f"{current_time} / {total_time}")
            else:
                self.timeLabel.setText(f"{current_time} / --:--:--")

    def duration_changed(self, duration):
        self.slider.setRange(0, duration)
        # 更新总时长显示
        if not self.is_fullscreen and duration > 0:
            current_pos = self.mediaPlayer.position()
            current_time = self.format_time(current_pos)
            total_time = self.format_time(duration)
            self.timeLabel.setText(f"{current_time} / {total_time}")

    def set_position(self, position):
        """设置播放位置（确保值在有效范围内）"""
        if self.mediaPlayer.duration() > 0:
            position = max(0, min(position, self.mediaPlayer.duration()))
        self.mediaPlayer.setPosition(position)
        # 更新进度条（但不触发信号，避免循环）
        if hasattr(self, 'slider') and not self.slider.isSliderDown():
            self.slider.setValue(position)
        if hasattr(self, 'progress_slider') and not self.progress_slider.isSliderDown():
            self.progress_slider.setValue(position)

    def keyPressEvent(self, event):
        key = event.key()
        
        # 音量控制：W/上键/PageUp增加音量，S/下键/PageDown减少音量
        if key in (Qt.Key_W, Qt.Key_Up, Qt.Key_PageUp):
            current_volume = self.mediaPlayer.volume()
            new_volume = min(100, current_volume + 5)
            self.mediaPlayer.setVolume(new_volume)
            self.label.setText(f"音量: {new_volume}%")
            # 保存音量设置
            try:
                if self._settings is not None:
                    self._settings.setValue('volume', new_volume)
            except Exception:
                pass
            event.accept()
            return
        elif key in (Qt.Key_S, Qt.Key_Down, Qt.Key_PageDown):
            current_volume = self.mediaPlayer.volume()
            new_volume = max(0, current_volume - 5)
            self.mediaPlayer.setVolume(new_volume)
            self.label.setText(f"音量: {new_volume}%")
            # 保存音量设置
            try:
                if self._settings is not None:
                    self._settings.setValue('volume', new_volume)
            except Exception:
                pass
            event.accept()
            return
        
        # 倍速快捷键：1 = 快进 10s, 2 = 快进 30s, 3 = 快进 60s, 0 = 快退 10s
        if key == Qt.Key_1:
            self.quick_seek(10000)  # 快进 10 秒
            event.accept()
            return
        elif key == Qt.Key_2:
            self.quick_seek(30000)  # 快进 30 秒
            event.accept()
            return
        elif key == Qt.Key_3:
            self.quick_seek(60000)  # 快进 60 秒
            event.accept()
            return
        elif key == Qt.Key_0:
            self.quick_seek(-10000)  # 快退 10 秒
            event.accept()
            return
        
        # F: 全屏播放
        if key == Qt.Key_F:
            self.toggle_fullscreen()
            event.accept()
            return
        
        # Space: 切换播放/暂停（立即生效）
        if key == Qt.Key_Space:
            if self.mediaPlayer.state() == QMediaPlayer.PlayingState:
                self.mediaPlayer.pause()
            else:
                self.mediaPlayer.play()
            event.accept()
            return

        # 对于快进/快退按键（Left/Right/Home/End/A/D），启动连续快进/快退计时器
        if key in (Qt.Key_Left, Qt.Key_Right, Qt.Key_Home, Qt.Key_End, Qt.Key_A, Qt.Key_D):
            # 不管是否重复，都添加到活跃键集合（确保一直快进）
            if key not in self._key_press_times:
                self._key_press_times[key] = time.time()
            self._active_seek_keys.add(key)
            # 立即触发一次快进
            self._on_seek_timer()
            # 启动定时器（如果未运行）
            if not self._seek_timer.isActive():
                self._seek_timer.start()
            event.accept()
            return

        super().keyPressEvent(event)

    def keyReleaseEvent(self, event):
        key = event.key()
        if key in (Qt.Key_Left, Qt.Key_Right, Qt.Key_Home, Qt.Key_End, Qt.Key_A, Qt.Key_D):
            # 从活跃键集合移除
            self._active_seek_keys.discard(key)
            # 移除按键时间记录
            self._key_press_times.pop(key, None)
            # 如果没有活跃按键，停止定时器
            if not self._active_seek_keys:
                self._seek_timer.stop()
            event.accept()
            return

        super().keyReleaseEvent(event)

    def _on_seek_timer(self):
        """计时器回调：连续快进/快退并显示倍速"""
        # 检查是否还有活跃的快进/快退键
        if not self._active_seek_keys:
            self._seek_timer.stop()
            return
        
        # 检查播放器是否有有效的媒体
        if not self.mediaPlayer.duration() or self.mediaPlayer.duration() == 0:
            return
        
        # 对所有活跃的键执行快进/快退
        for key in list(self._active_seek_keys):  # 用列表副本避免迭代时修改集合
            if key not in self._key_press_times:
                continue
            
            hold = max(0.0, time.time() - self._key_press_times[key])
            
            # 计算倍速
            if self.seek_mode == 'exponential':
                multiplier = 2 ** hold
            else:
                multiplier = 1.0
                for th, val in self._seek_steps:
                    if hold >= th:
                        multiplier = val / self.seek_base_ms
            
            # 显示倍速信息
            try:
                if not self.is_fullscreen:
                    self.label.setText(f"倍速: {multiplier:.1f}x")
            except Exception:
                pass
            
            # 计算此次位移
            seek_ms = int(self.seek_speed_ms_per_sec * 0.03 * multiplier)
            
            current = self.mediaPlayer.position()
            dur = self.mediaPlayer.duration() or 0
            
            # Key_Left/Home/A => 向后，Key_Right/End/D => 向前
            if key in (Qt.Key_Left, Qt.Key_Home, Qt.Key_A):
                new_pos = max(0, current - seek_ms)
            else:
                new_pos = min(dur, current + seek_ms)
            
            # 设置新位置
            if new_pos != current:
                self.mediaPlayer.setPosition(new_pos)
                # 更新进度条（如果不在拖动状态）
                try:
                    if hasattr(self, 'slider') and not self.slider.isSliderDown():
                        self.slider.setValue(new_pos)
                    if hasattr(self, 'fullscreen_player') and self.fullscreen_player and hasattr(self.fullscreen_player, 'progress_slider'):
                        if not self.fullscreen_player.progress_slider.isSliderDown():
                            self.fullscreen_player.progress_slider.setValue(new_pos)
                except Exception:
                    pass

    def handle_errors(self):
        self.playBtn.setEnabled(False)
        self.label.setText("错误: " + self.mediaPlayer.errorString())

    def toggle_seek_mode(self):
        # 在两种模式之间切换并更新下拉与持久化
        new_mode = 'stepped' if self.seek_mode == 'exponential' else 'exponential'
        self.seek_mode = new_mode
        # 更新 combobox 显示（避免触发多余的保存逻辑）
        try:
            idx = 0 if new_mode == 'exponential' else 1
            self.modeCombo.blockSignals(True)
            self.modeCombo.setCurrentIndex(idx)
            self.modeCombo.blockSignals(False)
        except Exception:
            pass
        # 持久化
        try:
            if self._settings is not None:
                self._settings.setValue('seek_mode', self.seek_mode)
        except Exception:
            pass
        try:
            self.label.setText(f"模式已切换: {'指数' if new_mode=='exponential' else '阶梯'}")
        except Exception:
            pass

    def on_mode_combo_changed(self, index):
        try:
            mode = self.modeCombo.itemData(index)
            if not mode:
                mode = 'exponential'
            self.seek_mode = mode
            if self._settings is not None:
                self._settings.setValue('seek_mode', mode)
            self.label.setText(f"模式: {'指数' if mode=='exponential' else '阶梯'}")
        except Exception:
            pass

    def on_speed_changed(self, value):
        """倍速设置值改变时的回调"""
        try:
            self.seek_speed_ms_per_sec = value
            if self._settings is not None:
                self._settings.setValue('seek_speed', value)
            self.label.setText(f"快进倍速设置: {value}ms/s")
        except Exception:
            pass

    def set_playback_rate(self, rate):
        """设置播放速度"""
        self.mediaPlayer.setPlaybackRate(rate)
        self.playback_rate = rate
        if rate == 1.0:
            self.label.setText("正常速度 (1.0x)")
        else:
            self.label.setText(f"播放速度: {rate}x")

    def toggle_fullscreen(self):
        """切换全屏模式"""
        if self.is_fullscreen:
            # 退出全屏
            self.exit_fullscreen()
        else:
            # 进入全屏
            self.enter_fullscreen()

    def enter_fullscreen(self):
        """进入全屏播放"""
        # 记录当前播放状态
        was_playing = self.mediaPlayer.state() == QMediaPlayer.PlayingState
        current_position = self.mediaPlayer.position()
        
        # 重用全屏播放器，避免重复创建造成卡顿
        if self.fullscreen_player is None:
            self.fullscreen_player = FullscreenPlayer(self.mediaPlayer, self)
            self.fullscreen_player.exit_callback = self.exit_fullscreen
        
        self.is_fullscreen = True
        
        # 先显示全屏窗口（避免黑屏）
        self.fullscreen_player.showFullScreen()
        
        # 切换视频输出到全屏窗口
        self.mediaPlayer.setVideoOutput(self.fullscreen_player.video_widget)
        
        # 恢复播放位置和状态（减少卡顿感）
        if current_position > 0:
            self.mediaPlayer.setPosition(current_position)
        if was_playing:
            QTimer.singleShot(50, self.mediaPlayer.play)
        
        # 显示控制条
        self.fullscreen_player.show_controls()
        
        # 隐藏主窗口（放在最后，避免闪烁）
        QTimer.singleShot(100, self.hide)
        
        # 确保全屏窗口获得焦点
        QTimer.singleShot(150, self.fullscreen_player.activateWindow)

    def exit_fullscreen(self):
        """退出全屏播放"""
        if self.fullscreen_player is None:
            return
        
        # 记录当前播放状态
        was_playing = self.mediaPlayer.state() == QMediaPlayer.PlayingState
        current_position = self.mediaPlayer.position()
        
        self.is_fullscreen = False
        
        # 先显示主窗口（避免黑屏）
        self.showNormal()
        self.activateWindow()
        
        # 切换视频输出回主窗口
        self.mediaPlayer.setVideoOutput(self.video_widget)
        
        # 恢复播放位置和状态
        if current_position > 0:
            self.mediaPlayer.setPosition(current_position)
        if was_playing:
            QTimer.singleShot(50, self.mediaPlayer.play)
        
        # 隐藏全屏窗口（放在最后）
        QTimer.singleShot(100, self.fullscreen_player.hide)
        
        # 更新主窗口的时间显示
        duration = self.mediaPlayer.duration()
        if duration > 0:
            current_time = self.format_time(current_position)
            total_time = self.format_time(duration)
            self.timeLabel.setText(f"{current_time} / {total_time}")
        self.label.setText("已退出全屏")

    def quick_seek(self, offset_ms):
        """快速跳转指定时长（毫秒）"""
        current = self.mediaPlayer.position()
        dur = self.mediaPlayer.duration() or 0
        new_pos = max(0, min(dur, current + offset_ms))
        self.mediaPlayer.setPosition(new_pos)
        
        try:
            self.slider.setValue(new_pos)
        except Exception:
            pass
        
        # 显示跳转信息
        direction = "快进" if offset_ms > 0 else "快退"
        duration_sec = abs(offset_ms) // 1000
        try:
            self.label.setText(f"{direction} {duration_sec} 秒 (当前位置: {new_pos//1000}s)")
        except Exception:
            pass

    def highlight_current_playing(self):
        """高亮显示当前播放的视频（完全禁用滚动）"""
        if not self.current_playing_file:
            return
        
        from PyQt5.QtGui import QBrush
        
        # 规范化路径以便对比
        current_file = os.path.normpath(os.path.abspath(self.current_playing_file))
        
        # 保存当前滚动位置
        video_scroll_pos = self.videoListWidget.verticalScrollBar().value()
        recent_scroll_pos = self.recentListWidget.verticalScrollBar().value()
        
        # 在视频列表中查找并高亮
        for i in range(self.videoListWidget.count()):
            item = self.videoListWidget.item(i)
            fpath = item.data(Qt.UserRole)
            if fpath:
                fpath_normalized = os.path.normpath(os.path.abspath(fpath))
                if fpath_normalized == current_file:
                    # 只设置背景色
                    item.setBackground(QBrush(self.highlight_color))
                else:
                    # 其他项恢复正常
                    item.setBackground(QBrush(QColor(255, 255, 255)))
        
        # 清除所有选择
        self.videoListWidget.clearSelection()
        
        # 在最近播放中也高亮
        for i in range(self.recentListWidget.count()):
            item = self.recentListWidget.item(i)
            fpath = item.data(Qt.UserRole)
            if fpath:
                fpath_normalized = os.path.normpath(os.path.abspath(fpath))
                if fpath_normalized == current_file:
                    item.setBackground(QBrush(self.highlight_color))
                else:
                    item.setBackground(QBrush(QColor(255, 255, 255)))
        
        # 清除所有选择
        self.recentListWidget.clearSelection()
        
        # 恢复滚动位置
        self.videoListWidget.verticalScrollBar().setValue(video_scroll_pos)
        self.recentListWidget.verticalScrollBar().setValue(recent_scroll_pos)

    def choose_highlight_color(self):
        """选择高亮颜色"""
        color = QColorDialog.getColor(self.highlight_color, self, "选择高亮颜色")
        if color.isValid():
            self.highlight_color = color
            # 保存高亮颜色到设置
            try:
                if self._settings is not None:
                    # 将颜色保存为字符串格式（r,g,b）
                    color_str = f"{color.red()},{color.green()},{color.blue()}"
                    self._settings.setValue('highlight_color', color_str)
                    self.label.setText(f"高亮颜色已保存: RGB({color.red()}, {color.green()}, {color.blue()})")
            except Exception:
                pass
            # 立即更新高亮
            self.highlight_current_playing()

    def closeEvent(self, event):
        """窗口关闭事件 - 保存播放位置"""
        # 停止保存位置计时器
        self._save_position_timer.stop()
        # 保存播放位置
        self.save_playback_position()
        event.accept()

if __name__ == '__main__':
    # 启用高分屏缩放支持
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)
    
    app = QApplication(sys.argv)
    
    # 统一设置应用字体，增加可读性
    from PyQt5.QtGui import QFont
    font = QFont("Microsoft YaHei UI", 9)
    app.setFont(font)
    
    player = AudioVideoPlayer()
    player.show()
    sys.exit(app.exec_())





