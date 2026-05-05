# Linux 系统运行指南

## 方法一：使用启动脚本（推荐）

### 1. 给脚本添加执行权限
```bash
chmod +x 启动跨平台版本.sh
```

### 2. 运行脚本
```bash
./启动跨平台版本.sh
```

这个脚本会自动：
- 检查Python版本
- 检查依赖包
- 检查系统工具
- 启动程序

## 方法二：直接运行Python脚本

### 1. 确保有Python 3
```bash
python3 --version
# 或
python --version
```

### 2. 安装依赖
```bash
# 运行依赖安装脚本
python3 install_dependencies.py

# 或者手动安装
pip install psutil pillow matplotlib pystray
```

### 3. 安装系统工具（Arch Linux）
```bash
sudo pacman -S xdotool xprintidle wmctrl

# 对于GNOME桌面环境
sudo pacman -S libappindicator-gtk3
```

### 4. 运行程序
```bash
python3 ScreenTracker_cross_platform.pyw
```

## 方法三：作为后台服务运行

### 1. 创建systemd服务（可选）
创建文件 `/etc/systemd/system/screen-tracker.service`：
```ini
[Unit]
Description=Screen Usage Tracker
After=network.target

[Service]
Type=simple
User=你的用户名
WorkingDirectory=/path/to/Screen_Tracker
ExecStart=/usr/bin/python3 /path/to/Screen_Tracker/ScreenTracker_cross_platform.pyw
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

### 2. 启用服务
```bash
sudo systemctl daemon-reload
sudo systemctl enable screen-tracker
sudo systemctl start screen-tracker
```

## 常见问题解决

### 1. 权限问题
```bash
# 如果提示权限不足
chmod +x *.sh
chmod +x *.pyw
```

### 2. Python找不到
```bash
# 检查Python安装
which python3
which python

# 如果使用python命令而不是python3
ln -s /usr/bin/python3 /usr/local/bin/python  # 可选
```

### 3. 依赖安装失败
```bash
# 使用系统包管理器安装
sudo pacman -S python-psutil python-pillow python-matplotlib

# 或者使用pip用户安装
pip install --user psutil pillow matplotlib pystray
```

### 4. 系统托盘不显示
```bash
# 检查桌面环境
echo $XDG_CURRENT_DESKTOP

# 安装必要的库
# GNOME:
sudo pacman -S libappindicator-gtk3

# KDE Plasma通常不需要额外配置
```

### 5. 程序在后台运行
如果系统托盘不支持，程序会在终端后台运行：
- 按 `Ctrl+C` 停止程序
- 使用 `ps aux | grep ScreenTracker` 查看进程
- 使用 `kill [PID]` 停止进程

## 测试运行

### 简单测试
```bash
# 进入目录
cd /run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker

# 测试Python环境
python3 -c "import sys; print(f'Python {sys.version}')"

# 测试基本功能
python3 -c "import psutil; print('psutil OK')"
python3 -c "import pystray; print('pystray OK')"
```

### 完整测试
```bash
# 运行安装脚本
python3 install_dependencies.py

# 运行程序（测试模式）
python3 ScreenTracker_cross_platform.pyw &
# 程序会在后台运行，检查系统托盘

# 查看日志
tail -f /tmp/screen_tracker.log  # 如果程序有日志输出
```

## 桌面快捷方式

### 创建.desktop文件
创建 `~/.local/share/applications/screen-tracker.desktop`：
```ini
[Desktop Entry]
Type=Application
Name=屏幕统计工具
Comment=跨平台屏幕使用时长统计
Exec=/usr/bin/python3 /path/to/Screen_Tracker/ScreenTracker_cross_platform.pyw
Icon=/path/to/Screen_Tracker/icon.png
Terminal=false
Categories=Utility;
```

### 添加到应用程序菜单
```bash
# 更新应用程序数据库
update-desktop-database ~/.local/share/applications
```

## 数据文件位置

- 数据库：`screen_data.db`（程序目录下）
- 锁文件：`app.lock`（防止多实例运行）
- 日志：程序输出到终端或系统日志

## 停止程序

### 如果程序在终端前台运行
按 `Ctrl+C`

### 如果程序在后台运行
```bash
# 查找进程ID
ps aux | grep ScreenTracker

# 停止进程
kill [PID]

# 或者使用pkill
pkill -f ScreenTracker
```

### 如果程序卡住
```bash
# 强制停止
pkill -9 -f ScreenTracker

# 删除锁文件
rm -f app.lock
```

## 更新程序

### 从Git更新（如果有）
```bash
cd /run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker
git pull origin main  # 如果有使用git
```

### 手动更新
1. 备份 `screen_data.db`
2. 下载新版本文件
3. 替换旧文件
4. 恢复数据库

## 获取帮助

如果遇到问题：
1. 查看 `跨平台使用说明.md`
2. 运行 `install_dependencies.py` 检查依赖
3. 查看程序输出的错误信息
4. 检查系统日志：`journalctl -f`

---

**提示**：建议先在终端测试运行，确认一切正常后再设置为开机自启。