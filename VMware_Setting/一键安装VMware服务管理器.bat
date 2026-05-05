@echo off
setlocal
rem ensure running under cmd.exe
if not defined __BAT_LAUNCHED (
  set __BAT_LAUNCHED=1
  start "" cmd /k "%~f0"
  exit /b
)
chcp 936 >nul
title VMware服务管理器一键安装
echo.
echo +==================================================+
echo ^|          VMware服务管理器一键安装程序            |
echo +==================================================+
echo.
echo 此程序将为您：
echo 1. 创建桌面快捷方式
echo 2. 启动后台监控服务
echo 3. 创建启动菜单项
echo.
echo 请确保已安装Python和psutil库
echo.

set /p continue="是否继续？(Y/N): "

if /i "%continue%" neq "Y" (
    echo 安装已取消
    pause
    exit /b 0
)

echo.
echo 步骤1: 创建桌面快捷方式...
python.exe "%~dp0vmware_service_manager.py" --install
if %errorlevel% neq 0 (
    echo 创建快捷方式失败
    pause
    exit /b 1
)

echo.
echo 步骤2: 启动后台监控...
start /B pythonw.exe "%~dp0vmware_service_manager.py"
echo 后台监控已启动

echo.
echo 步骤3: 创建启动文件夹快捷方式（可选）...
set /p create_startup="是否创建开机自启动？(Y/N): "
if /i "%create_startup%"=="Y" (
    powershell -Command "$StartupPath = [Environment]::GetFolderPath('Startup'); $ShortcutPath = Join-Path $StartupPath 'VMware服务管理器.lnk'; $ScriptPath = '%cd%\vmware_service_manager.py'; $WScriptShell = New-Object -ComObject WScript.Shell; $Shortcut = $WScriptShell.CreateShortcut($ShortcutPath); $Shortcut.TargetPath = 'pythonw.exe'; $Shortcut.Arguments = '`"$ScriptPath`"'; $Shortcut.WorkingDirectory = '%cd%'; $Shortcut.Description = 'VMware服务自动管理器'; $Shortcut.IconLocation = 'shell32.dll,21'; $Shortcut.Save()"
    echo 开机自启动已设置
)

echo.
echo +==================================================+
echo ^|                  安装完成！                      |
echo +==================================================+
echo.
echo 已创建以下项目：
echo OK 桌面快捷方式：VMware服务管理器.lnk
echo OK 后台监控服务（已启动）
if /i "%create_startup%"=="Y" (
    echo OK 开机自启动项
)
echo.
echo 使用方法：
echo 1. 双击桌面快捷方式启动/停止监控
echo 2. 或运行 vmware_service_launcher.bat 进行更多操作
echo 3. 监控会自动管理VMware NAT和DHCP服务
echo.
echo 监控功能：
echo - 检测VMware启动 -> 自动启动服务
echo - 检测VMware关闭 -> 自动停止服务
echo - 减少不必要的性能损耗
echo.
pause