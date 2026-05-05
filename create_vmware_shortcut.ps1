# VMware Service Manager Shortcut Creator
# Create desktop shortcut for VMware Service Manager

$DesktopPath = [Environment]::GetFolderPath("Desktop")
$ShortcutPath = Join-Path $DesktopPath "VMwareServiceManager.lnk"
$ScriptPath = Join-Path $PSScriptRoot "vmware_service_manager.py"
$IconPath = "shell32.dll,21"  # Use system icon

Write-Host "Creating VMware Service Manager shortcut..." -ForegroundColor Green

# Create shortcut object
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut($ShortcutPath)

# Set shortcut properties
$Shortcut.TargetPath = "pythonw.exe"
$Shortcut.Arguments = "`"$ScriptPath`""
$Shortcut.WorkingDirectory = $PSScriptRoot
$Shortcut.Description = "VMware NAT and DHCP Service Auto Manager"
$Shortcut.IconLocation = $IconPath

# Save shortcut
$Shortcut.Save()

Write-Host "Shortcut created at: $ShortcutPath" -ForegroundColor Green
Write-Host ""
Write-Host "Usage:" -ForegroundColor Yellow
Write-Host "1. Double-click shortcut to start background monitoring" -ForegroundColor Cyan
Write-Host "2. Monitor will automatically detect VMware status and manage services" -ForegroundColor Cyan
Write-Host "3. To stop monitoring, end pythonw.exe process in Task Manager" -ForegroundColor Cyan
Write-Host ""
Write-Host "Or run batch file: vmware_service_launcher.bat" -ForegroundColor Yellow

# Keep window open
Read-Host "Press Enter to exit"
