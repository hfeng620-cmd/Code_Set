#!/bin/bash
# Linux 开机自启动设置脚本
# 最简单的方法：创建 .desktop 文件

echo "=== Linux 开机自启动设置 ==="
echo ""

# 检查是否在正确目录
if [ ! -f "ScreenTracker_cross_platform.pyw" ]; then
    echo "错误: 请在 Screen_Tracker 目录中运行此脚本"
    echo "当前目录: $(pwd)"
    exit 1
fi

# 获取当前用户名
USERNAME=$(whoami)
echo "当前用户: $USERNAME"

# 创建 autostart 目录
AUTOSTART_DIR="$HOME/.config/autostart"
echo "创建目录: $AUTOSTART_DIR"
mkdir -p "$AUTOSTART_DIR"

# 获取 Python 路径
PYTHON_PATH=$(which python3)
if [ -z "$PYTHON_PATH" ]; then
    PYTHON_PATH=$(which python)
fi

if [ -z "$PYTHON_PATH" ]; then
    echo "错误: 未找到 Python，请先安装 Python3"
    exit 1
fi

echo "Python 路径: $PYTHON_PATH"

# 获取程序绝对路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROGRAM_PATH="$SCRIPT_DIR/ScreenTracker_cross_platform.pyw"
ICON_PATH="$SCRIPT_DIR/icon.png"

echo "程序路径: $PROGRAM_PATH"
echo "图标路径: $ICON_PATH"

# 创建 .desktop 文件
DESKTOP_FILE="$AUTOSTART_DIR/screen_tracker.desktop"
echo "创建文件: $DESKTOP_FILE"

cat > "$DESKTOP_FILE" << EOF
[Desktop Entry]
Type=Application
Name=屏幕统计工具
Comment=跨平台屏幕使用时长统计
Exec=$PYTHON_PATH "$PROGRAM_PATH"
Icon=$ICON_PATH
Terminal=false
StartupNotify=false
X-GNOME-Autostart-enabled=true
EOF

# 设置权限
chmod +x "$DESKTOP_FILE"

echo ""
echo "✅ 开机自启动设置完成！"
echo ""
echo "文件位置: $DESKTOP_FILE"
echo "文件内容:"
echo "----------------------------------------"
cat "$DESKTOP_FILE"
echo "----------------------------------------"
echo ""
echo "下次登录时程序会自动启动。"
echo ""
echo "如果要立即测试，可以运行:"
echo "  $PYTHON_PATH \"$PROGRAM_PATH\""
echo ""
echo "取消开机自启:"
echo "  rm \"$DESKTOP_FILE\""
echo ""
echo "通过程序菜单取消:"
echo "  1. 运行程序"
echo "  2. 右键点击系统托盘图标"
echo "  3. 选择 '❌ 取消自启'"
echo ""
echo "重启系统后生效。"