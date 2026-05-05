@echo off
chcp 65001 >nul
echo 正在启动威震天屏幕统计工具...
echo.

cd /d "%~dp0Screen_Tracker"

REM 检查Python环境
if exist "D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\pythonw.exe" (
    echo 使用指定的Python环境...
    "D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\pythonw.exe" ScreenTracker.pyw
) else (
    echo 使用系统Python环境...
    pythonw ScreenTracker.pyw
)

if %errorlevel% neq 0 (
    echo.
    echo 启动失败，请检查：
    echo 1. Python是否已安装
    echo 2. 依赖库是否已安装：pip install pywin32 psutil pillow pystray matplotlib
    echo 3. 文件是否完整
    pause
)