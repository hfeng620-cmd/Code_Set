@echo off
setlocal
rem ensure running under cmd.exe
if not defined __BAT_LAUNCHED (
  set __BAT_LAUNCHED=1
  start "" cmd /k "%~f0"
  exit /b
)
chcp 936 >nul
title VMware Service Manager
color 0A

echo.
echo ============================================
echo      VMware Service Manager v1.2
echo ============================================
echo.
echo Function: Auto manage VMware NAT & DHCP Services
echo Author: Cline
echo.
echo.

:menu
echo.
echo Please select operation:
echo 1. Start background monitoring (Recommended)
echo 2. Show current service status
echo 3. Manually start VMware services
echo 4. Manually stop VMware services
echo 5. Check if VMware is running
echo 6. Create desktop shortcut
echo 7. Exit
echo.

set /p choice="Enter choice (1-7): "

if "%choice%"=="1" goto start_monitor
if "%choice%"=="2" goto show_status
if "%choice%"=="3" goto start_services
if "%choice%"=="4" goto stop_services
if "%choice%"=="5" goto check_vmware
if "%choice%"=="6" goto create_shortcut
if "%choice%"=="7" goto exit_program

echo Invalid choice, please try again
goto menu

:start_monitor
echo.
echo Starting background monitoring...
echo Monitoring will automatically:
echo - Detect VMware start -> Auto start services
echo - Detect VMware stop -> Auto stop services
echo - Reduce unnecessary performance loss
echo.
echo Press Ctrl+C to stop monitoring, or close this window
echo.
python.exe "%~dp0vmware_service_manager.py"
pause
goto menu

:show_status
echo.
echo Current service status:
echo.
powershell -Command "Get-Service -Name 'VMware NAT Service' | Select-Object Name, Status, DisplayName | Format-Table -AutoSize"
echo.
powershell -Command "Get-Service -Name 'VMnetDHCP' | Select-Object Name, Status, DisplayName | Format-Table -AutoSize"
echo.
pause
goto menu

:start_services
echo.
echo Starting VMware services...
powershell -Command "Start-Service -Name 'VMware NAT Service'"
powershell -Command "Start-Service -Name 'VMnetDHCP'"
echo.
echo Services started successfully!
echo.
powershell -Command "Get-Service -Name 'VMware NAT Service','VMnetDHCP' | Select-Object Name, Status | Format-Table -AutoSize"
pause
goto menu

:stop_services
echo.
echo Stopping VMware services...
powershell -Command "Stop-Service -Name 'VMware NAT Service'"
powershell -Command "Stop-Service -Name 'VMnetDHCP'"
echo.
echo Services stopped successfully!
echo.
powershell -Command "Get-Service -Name 'VMware NAT Service','VMnetDHCP' | Select-Object Name, Status | Format-Table -AutoSize"
pause
goto menu

:check_vmware
echo.
echo Checking VMware running status...
tasklist | findstr /i vmware.exe >nul
if %errorlevel% equ 0 (
    echo [OK] VMware is running
) else (
    echo [X] VMware is not running
)
echo.
pause
goto menu

:create_shortcut
echo.
echo Creating desktop shortcut...
set "desktop=%USERPROFILE%\Desktop"
set "shortcut=%desktop%\VMware_Service_Manager.bat"

echo @echo off > "%shortcut%"
echo chcp 936 ^>nul >> "%shortcut%"
echo cd /d "%~dp0" >> "%shortcut%"
echo start "VMware Service Manager" "%~dp0VMware_Service_Manager_EN.bat" >> "%shortcut%"

echo Shortcut created on desktop: VMware_Service_Manager.bat
echo.
echo Usage:
echo 1. Double-click "VMware_Service_Manager.bat" on desktop
echo 2. Select desired function
echo 3. Recommended: Option 1 for background monitoring
echo.
pause
goto menu

:exit_program
echo.
echo Thank you for using VMware Service Manager!
echo.
pause
exit /b 0