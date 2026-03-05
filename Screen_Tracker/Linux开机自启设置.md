# Linux 开机自启动设置指南

## 方法一：通过程序菜单设置（最简单）

### 步骤：
1. 运行程序
   ```bash
   cd "/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker"
   python ScreenTracker_cross_platform.pyw
   ```

2. 右键点击系统托盘图标
3. 选择菜单中的 "✅ 开机自启"
4. 程序会自动创建自启动文件

### 原理：
程序会在 `~/.config/autostart/` 目录下创建 `screen_tracker.desktop` 文件。

## 方法二：手动创建 .desktop 文件

### 步骤：
1. 创建 autostart 目录（如果不存在）
   ```bash
   mkdir -p ~/.config/autostart
   ```

2. 创建 .desktop 文件
   ```bash
   nano ~/.config/autostart/screen_tracker.desktop
   ```

3. 添加以下内容：
   ```ini
   [Desktop Entry]
   Type=Application
   Name=屏幕统计工具
   Comment=跨平台屏幕使用时长统计
   Exec=/usr/bin/python3 "/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker/ScreenTracker_cross_platform.pyw"
   Icon=/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker/icon.png
   Terminal=false
   StartupNotify=false
   X-GNOME-Autostart-enabled=true
   ```

4. 保存并退出（Ctrl+X，然后按 Y，再按 Enter）

5. 给文件添加执行权限
   ```bash
   chmod +x ~/.config/autostart/screen_tracker.desktop
   ```

## 方法三：使用 systemd 服务（更稳定）

### 步骤：
1. 创建 systemd 服务文件
   ```bash
   sudo nano /etc/systemd/system/screen-tracker.service
   ```

2. 添加以下内容：
   ```ini
   [Unit]
   Description=Screen Usage Tracker
   After=graphical.target network.target
   Wants=graphical.target

   [Service]
   Type=simple
   User=tian  # 替换为你的用户名
   Environment=DISPLAY=:0
   Environment=XAUTHORITY=/home/tian/.Xauthority  # 替换为你的用户名
   WorkingDirectory=/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker
   ExecStart=/usr/bin/python3 /run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker/ScreenTracker_cross_platform.pyw
   Restart=on-failure
   RestartSec=10

   [Install]
   WantedBy=graphical.target
   ```

3. 重新加载 systemd 配置
   ```bash
   sudo systemctl daemon-reload
   ```

4. 启用服务（开机自启）
   ```bash
   sudo systemctl enable screen-tracker.service
   ```

5. 启动服务
   ```bash
   sudo systemctl start screen-tracker.service
   ```

6. 检查服务状态
   ```bash
   sudo systemctl status screen-tracker.service
   ```

## 方法四：添加到 ~/.xinitrc（适用于启动器）

### 步骤：
1. 编辑 ~/.xinitrc 文件
   ```bash
   nano ~/.xinitrc
   ```

2. 在文件末尾添加：
   ```bash
   # 启动屏幕统计工具
   python3 "/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker/ScreenTracker_cross_platform.pyw" &
   ```

3. 保存并退出

## 方法五：添加到 ~/.bashrc 或 ~/.zshrc（终端启动）

### 步骤：
1. 编辑 shell 配置文件
   ```bash
   nano ~/.bashrc  # 或 ~/.zshrc
   ```

2. 在文件末尾添加：
   ```bash
   # 启动屏幕统计工具（如果不在运行）
   if ! pgrep -f "ScreenTracker_cross_platform" > /dev/null; then
       python3 "/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker/ScreenTracker_cross_platform.pyw" &
   fi
   ```

3. 保存并重新加载配置
   ```bash
   source ~/.bashrc
   ```

## 验证自启动是否生效

### 检查方法：
1. 查看 .desktop 文件
   ```bash
   ls -la ~/.config/autostart/
   cat ~/.config/autostart/screen_tracker.desktop
   ```

2. 重启后检查进程
   ```bash
   ps aux | grep ScreenTracker
   ```

3. 检查日志
   ```bash
   journalctl -u screen-tracker.service  # 如果使用systemd
   ```

## 取消开机自启

### 方法一：通过程序菜单
1. 运行程序
2. 右键点击系统托盘图标
3. 选择 "❌ 取消自启"

### 方法二：删除 .desktop 文件
```bash
rm ~/.config/autostart/screen_tracker.desktop
```

### 方法三：禁用 systemd 服务
```bash
sudo systemctl disable screen-tracker.service
sudo systemctl stop screen-tracker.service
```

## 桌面环境特定设置

### GNOME
```bash
# 安装必要的库
sudo pacman -S libappindicator-gtk3

# 检查GNOME扩展
gnome-extensions list
```

### KDE Plasma
```bash
# 通常不需要额外配置
# 系统托盘支持较好
```

### XFCE
```bash
# 确保有通知守护进程
sudo pacman -S xfce4-notifyd
```

### LXQt / LXDE
```bash
# 可能需要安装系统托盘组件
sudo pacman -S lxqt-panel
```

## 故障排除

### 问题1：程序启动但没有系统托盘图标
```bash
# 检查桌面环境
echo $XDG_CURRENT_DESKTOP

# 检查必要的库
pacman -Qs libappindicator
```

### 问题2：程序启动但立即退出
```bash
# 检查日志
journalctl -xe | grep screen-tracker

# 手动运行查看错误
python3 ScreenTracker_cross_platform.pyw
```

### 问题3：自启动文件不生效
```bash
# 检查文件权限
ls -la ~/.config/autostart/

# 检查文件内容
cat ~/.config/autostart/screen_tracker.desktop

# 检查路径是否正确
which python3
```

### 问题4：systemd服务启动失败
```bash
# 查看详细日志
sudo journalctl -u screen-tracker.service -f

# 检查服务文件语法
sudo systemctl cat screen-tracker.service
```

## 推荐方案

### 对于普通用户：方法一（程序菜单设置）
最简单，无需命令行操作。

### 对于高级用户：方法三（systemd服务）
更稳定，可以自动重启，有完整的日志。

### 对于多用户系统：方法二（.desktop文件）
每个用户可以独立设置。

## 注意事项

1. **路径问题**：确保所有路径都是绝对路径
2. **用户权限**：程序以当前用户身份运行
3. **显示环境**：需要设置 DISPLAY 和 XAUTHORITY 环境变量
4. **Python版本**：确保使用正确的 Python 解释器
5. **依赖检查**：确保所有依赖都已安装

## 快速设置命令

```bash
# 一键设置（方法二）
mkdir -p ~/.config/autostart
cat > ~/.config/autostart/screen_tracker.desktop << 'EOF'
[Desktop Entry]
Type=Application
Name=屏幕统计工具
Comment=跨平台屏幕使用时长统计
Exec=/usr/bin/python3 "/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker/ScreenTracker_cross_platform.pyw"
Icon=/run/media/tian/软件/VS_Code/Code_Set/Screen_Tracker/icon.png
Terminal=false
StartupNotify=false
X-GNOME-Autostart-enabled=true
EOF
chmod +x ~/.config/autostart/screen_tracker.desktop
```

现在重启系统，程序应该会自动启动！