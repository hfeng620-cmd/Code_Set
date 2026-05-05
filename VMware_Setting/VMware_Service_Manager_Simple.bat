@echo off
setlocal
chcp 936 >nul
title VMware服务管理器 - 简单版
color 0A

echo.
echo ================================================
echo   VMware服务管理器 - 简单版
echo ================================================
echo 功能: 自动管理VMware NAT和DHCP服务
echo 作者: Cline
echo 版本: 1.1
echo.

:menu
echo.
echo 请选择操作:
echo 1. 启动后台监控(推荐)
echo 2. 显示当前服务状态
echo 3. 手动启动VMware服务
echo 4. 手动停止VMware服务
echo 5. 检查VMware是否运行
echo 6. 创建桌面快捷方式
echo 7. 退出
echo.

set /p choice=请输入选择(1-7): 

if "%choice%"=="1" goto start_monitor
if "%choice%"=="2" goto show_status
if "%choice%"=="3" goto start_services
if "%choice%"=="4" goto stop_services
if "%choice%"=="5" goto check_vmware
if "%choice%"=="6" goto create_shortcut
if "%choice%"=="7" goto exit_program

echo 无效的选择,请重新输入
goto menu

:start_monitor
echo.
echo 正在启动后台监控...
echo 按Ctrl+C停止监控,或关闭此窗口
echo.
python.exe "%~dp0vmware_service_manager.py"
pause
goto menu

:show_status
echo.
echo 当前服务状态:
echo.
powershell -Command "Get-Service -Name 'VMware NAT Service' | Select-Object Name, Status, DisplayName | Format-Table -AutoSize"
powershell -Command "Get-Service -Name 'VMnetDHCP' | Select-Object Name, Status, DisplayName | Format-Table -AutoSize"
echo.
pause
goto menu

:start_services
echo.
echo 正在启动VMware服务...
powershell -Command "Start-Service -Name 'VMware NAT Service'"
powershell -Command "Start-Service -Name 'VMnetDHCP'"
echo 服务启动完成!
echo.
powershell -Command "Get-Service -Name 'VMware NAT Service','VMnetDHCP' | Select-Object Name, Status | Format-Table -AutoSize"
pause
goto menu

:stop_services
echo.
echo 正在停止VMware服务...
powershell -Command "Stop-Service -Name 'VMware NAT Service'"
powershell -Command "Stop-Service -Name 'VMnetDHCP'"
echo 服务停止完成!
echo.
powershell -Command "Get-Service -Name 'VMware NAT Service','VMnetDHCP' | Select-Object Name, Status | Format-Table -AutoSize"
pause
goto menu

:check_vmware
echo.
echo 检查VMware运行状态...
tasklist | findstr /i vmware.exe >nul
if %errorlevel% equ 0 (
  echo VMware正在运行
) else (
  echo VMware未运行
)
echo.
pause
goto menu

:create_shortcut
echo.
echo 正在创建桌面快捷方式...
set "desktop=%USERPROFILE%\Desktop"
set "shortcut=%desktop%\VMware服务管理.bat"

echo @echo off > "%shortcut%"
echo chcp 936 ^>nul >> "%shortcut%"
echo cd /d "%~dp0" >> "%shortcut%"
echo start "VMware服务管理器" "%~dp0VMware_Service_Manager_Simple.bat" >> "%shortcut%"

echo 快捷方式已创建到桌面: VMware服务管理.bat
echo.
pause
goto menu

:exit_program
echo.
echo 感谢使用VMware服务管理器!
echo.
pause
exit /b 0