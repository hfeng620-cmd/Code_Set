@echo off
chcp 65001 >nul
echo 正在启动漂亮视频播放器...
echo.

REM 切换到脚本目录
cd /d "%~dp0..\Video_Player_Source"

REM 使用指定的Python环境
set "PYTHONW=D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\pythonw.exe"

if exist "%PYTHONW%" (
    echo 使用Python环境: %PYTHONW%
    echo 运行脚本: Video_Player_Perfect.py
    echo.
    "%PYTHONW%" "Video_Player_Perfect.py"
) else (
    echo 错误: 找不到Python环境 %PYTHONW%
    echo.
    echo 请检查Python环境路径
    pause
    exit /b 1
)

echo.
echo 视频播放器已启动
pause