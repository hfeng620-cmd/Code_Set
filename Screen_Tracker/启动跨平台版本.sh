#!/bin/bash
# 屏幕使用时长统计工具 - 跨平台版本启动脚本
# 适用于 Linux 系统

echo "屏幕使用时长统计工具 - 跨平台版本"
echo "=================================="

# 检查 Python 版本
python_version=$(python3 --version 2>&1 | awk '{print $2}')
echo "Python 版本: $python_version"

# 检查是否在 Screen_Tracker 目录
if [ ! -f "ScreenTracker_cross_platform.pyw" ]; then
    echo "错误: 请在 Screen_Tracker 目录中运行此脚本"
    echo "当前目录: $(pwd)"
    exit 1
fi

# 检查依赖
echo "检查依赖..."

# 检查并安装依赖（优先使用系统包管理器）
if ! python3 -c "import psutil" 2>/dev/null; then
    echo "警告: psutil 未安装"
    echo "建议使用系统包管理器安装："
    echo "  Arch Linux: sudo pacman -S python-psutil"
    echo "  Ubuntu/Debian: sudo apt install python3-psutil"
    echo "或者使用虚拟环境："
    echo "  python3 -m venv venv && source venv/bin/activate && pip install psutil"
    echo "或者使用 pipx："
    echo "  pipx install psutil"
    echo "或者强制安装（不推荐）："
    echo "  pip install --break-system-packages psutil"
    exit 1
fi

if ! python3 -c "import pystray" 2>/dev/null; then
    echo "警告: pystray 未安装"
    echo "建议使用系统包管理器安装："
    echo "  Arch Linux: sudo pacman -S python-pystray"
    echo "  Ubuntu/Debian: sudo apt install python3-pystray"
    echo "或者使用虚拟环境："
    echo "  python3 -m venv venv && source venv/bin/activate && pip install pystray"
    echo "或者使用 pipx："
    echo "  pipx install pystray"
    echo "或者强制安装（不推荐）："
    echo "  pip install --break-system-packages pystray"
    exit 1
fi

if ! python3 -c "import matplotlib" 2>/dev/null; then
    echo "警告: matplotlib 未安装"
    echo "建议使用系统包管理器安装："
    echo "  Arch Linux: sudo pacman -S python-matplotlib"
    echo "  Ubuntu/Debian: sudo apt install python3-matplotlib"
    echo "或者使用虚拟环境："
    echo "  python3 -m venv venv && source venv/bin/activate && pip install matplotlib"
    echo "或者使用 pipx："
    echo "  pipx install matplotlib"
    echo "或者强制安装（不推荐）："
    echo "  pip install --break-system-packages matplotlib"
    exit 1
fi

if ! python3 -c "import PIL" 2>/dev/null; then
    echo "警告: PIL (Pillow) 未安装"
    echo "建议使用系统包管理器安装："
    echo "  Arch Linux: sudo pacman -S python-pillow"
    echo "  Ubuntu/Debian: sudo apt install python3-pil"
    echo "或者使用虚拟环境："
    echo "  python3 -m venv venv && source venv/bin/activate && pip install pillow"
    echo "或者使用 pipx："
    echo "  pipx install pillow"
    echo "或者强制安装（不推荐）："
    echo "  pip install --break-system-packages pillow"
    exit 1
fi

# 检查系统工具
echo "检查系统工具..."
if ! command -v xdotool &> /dev/null; then
    echo "警告: xdotool 未安装，活动窗口检测功能可能受限"
    echo "在 Arch Linux 上可以使用: sudo pacman -S xdotool"
    echo "在 Ubuntu/Debian 上可以使用: sudo apt install xdotool"
fi

if ! command -v xprintidle &> /dev/null; then
    echo "警告: xprintidle 未安装，空闲时间检测功能可能受限"
    echo "在 Arch Linux 上可以使用: sudo pacman -S xprintidle"
    echo "在 Ubuntu/Debian 上可以使用: sudo apt install xprintidle"
fi

# 启动程序
echo ""
echo "启动屏幕统计工具..."
echo "按 Ctrl+C 停止程序"
echo "程序将在后台运行，请检查系统托盘"
echo ""

# 运行程序
python3 ScreenTracker_cross_platform.pyw

# 如果程序退出，显示提示
echo ""
echo "程序已停止"
echo "数据保存在: screen_data.db"
echo "可以使用查询脚本查看数据: python screen_data_query.py"