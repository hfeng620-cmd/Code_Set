@echo off
chcp 65001 >nul
echo 屏幕使用时长统计工具 - 跨平台版本
echo ==================================
echo.

REM 检查Python
python --version >nul 2>&1
if errorlevel 1 (
    echo 错误: Python未安装或不在PATH中
    echo 请先安装Python 3.6或更高版本
    pause
    exit /b 1
)

REM 检查是否在正确目录
if not exist "ScreenTracker_cross_platform.pyw" (
    echo 错误: 请在Screen_Tracker目录中运行此脚本
    echo 当前目录: %cd%
    pause
    exit /b 1
)

REM 检查依赖
echo 检查依赖...
python -c "import psutil" >nul 2>&1
if errorlevel 1 (
    echo 警告: psutil未安装，尝试安装...
    pip install psutil
)

python -c "import pystray" >nul 2>&1
if errorlevel 1 (
    echo 警告: pystray未安装，尝试安装...
    pip install pystray
)

python -c "import matplotlib" >nul 2>&1
if errorlevel 1 (
    echo 警告: matplotlib未安装，尝试安装...
    pip install matplotlib
)

python -c "import win32gui" >nul 2>&1
if errorlevel 1 (
    echo 警告: pywin32未安装，尝试安装...
    pip install pywin32
)

echo.
echo 启动屏幕统计工具...
echo 程序将在后台运行，请检查系统托盘（右下角）
echo 右键点击托盘图标可以查看菜单
echo.

REM 运行程序
python ScreenTracker_cross_platform.pyw

echo.
echo 程序已停止
echo 数据保存在: screen_data.db
echo 可以使用查询脚本查看数据: python screen_data_query.py
pause