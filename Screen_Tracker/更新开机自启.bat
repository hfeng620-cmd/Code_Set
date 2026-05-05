@echo off
chcp 65001 >nul
echo 更新威震天屏幕统计开机自启动路径...
echo.

REM 删除旧的注册表项
reg delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v "威震天屏幕统计" /f 2>nul

REM 设置新的注册表项
set "SCRIPT_PATH=%~dp0ScreenTracker.pyw"
set "PYTHON_PATH=D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\pythonw.exe"

if exist "%PYTHON_PATH%" (
    reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v "威震天屏幕统计" /t REG_SZ /d "\"%PYTHON_PATH%\" \"%SCRIPT_PATH%\"" /f
    echo 已更新开机自启动路径为新的 Screen_Tracker 文件夹
) else (
    echo 错误：找不到 Python 环境 %PYTHON_PATH%
    echo 请检查 Python 环境路径
)

echo.
echo 按任意键退出...
pause >nul