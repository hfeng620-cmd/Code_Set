@echo off
setlocal
rem ensure running under cmd.exe
if not defined __BAT_LAUNCHED (
  set __BAT_LAUNCHED=1
  start "" cmd /k "%~f0"
  exit /b
)
chcp 936 >nul
echo VMware服务管理器启动器
echo ============================
echo.
echo 请选择操作：
echo 1. 启动监控（后台运行）
echo 2. 启动监控（控制台窗口）
echo 3. 创建桌面快捷方式
echo 4. 检查当前状态
echo 5. 停止所有VMware服务
echo 6. 退出
echo.

set /p choice="请输入选择 (1-6): "

if "%choice%"=="1" (
    echo 正在启动后台监控...
    start /B pythonw.exe "%~dp0vmware_service_manager.py"
    echo 监控已在后台启动
    pause
    goto :eof
)

if "%choice%"=="2" (
    echo 正在启动控制台监控...
    python.exe "%~dp0vmware_service_manager.py"
    pause
    goto :eof
)

if "%choice%"=="3" (
    echo 正在创建桌面快捷方式...
    python.exe "%~dp0vmware_service_manager.py" --install
    pause
    goto :eof
)

if "%choice%"=="4" (
    echo 检查当前状态...
    python.exe "%~dp0vmware_service_manager.py" --once
    pause
    goto :eof
)

if "%choice%"=="5" (
    echo 正在停止VMware服务...
    powershell -Command "Stop-Service -Name 'VMware NAT Service'"
    powershell -Command "Stop-Service -Name 'VMnetDHCP'"
    echo 服务已停止
    pause
    goto :eof
)

if "%choice%"=="6" (
    exit /b 0
)

echo 无效的选择
pause