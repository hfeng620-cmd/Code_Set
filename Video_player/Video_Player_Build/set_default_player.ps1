# 设置漂亮播放器为默认视频播放器
# 需要以管理员身份运行

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "   设置漂亮播放器为默认视频播放器" -ForegroundColor Yellow
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""

# 检查是否以管理员身份运行
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "错误：此脚本需要以管理员身份运行！" -ForegroundColor Red
    Write-Host "请右键点击此脚本，选择'以管理员身份运行'" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "按任意键退出..." -ForegroundColor Gray
    $null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
    exit 1
}

# 应用程序路径
$appPath = "D:\VS_Code\Code_Set\dist\漂亮\漂亮.exe"
if (-not (Test-Path $appPath)) {
    Write-Host "错误：找不到应用程序：$appPath" -ForegroundColor Red
    Write-Host "请确保已成功创建exe文件" -ForegroundColor Yellow
    exit 1
}

Write-Host "找到应用程序：$appPath" -ForegroundColor Green

# 视频文件扩展名列表
$videoExtensions = @('.mp4', '.avi', '.mkv', '.wmv', '.mov', '.flv', '.m4v', '.webm')

Write-Host ""
Write-Host "正在设置文件关联..." -ForegroundColor Cyan

foreach ($ext in $videoExtensions) {
    # 设置文件类型关联
    $progId = "漂亮播放器$ext"
    
    # 创建 ProgID
    $regPath = "HKLM:\SOFTWARE\Classes\$progId"
    New-Item -Path $regPath -Force | Out-Null
    New-ItemProperty -Path $regPath -Name "FriendlyTypeName" -Value "漂亮播放器视频文件" -Force | Out-Null
    
    # 设置默认图标
    $iconPath = $regPath + "\DefaultIcon"
    New-Item -Path $iconPath -Force | Out-Null
    New-ItemProperty -Path $iconPath -Name "(Default)" -Value "`"$appPath`",0" -Force | Out-Null
    
    # 设置打开命令
    $shellPath = $regPath + "\shell\open\command"
    New-Item -Path $shellPath -Force | Out-Null
    New-ItemProperty -Path $shellPath -Name "(Default)" -Value "`"$appPath`" `"%1`"" -Force | Out-Null
    
    # 关联扩展名到 ProgID
    $extPath = "HKLM:\SOFTWARE\Classes\$ext"
    New-Item -Path $extPath -Force | Out-Null
    New-ItemProperty -Path $extPath -Name "(Default)" -Value $progId -Force | Out-Null
    
    Write-Host "已设置 $ext 文件关联" -ForegroundColor Green
}

# 刷新文件关联缓存
Write-Host ""
Write-Host "正在刷新文件关联缓存..." -ForegroundColor Cyan
try {
    # 方法1：使用 SHChangeNotify
    Add-Type -TypeDefinition @"
        using System;
        using System.Runtime.InteropServices;
        
        public class Shell32 {
            [DllImport("shell32.dll")]
            public static extern void SHChangeNotify(
                uint wEventId,
                uint uFlags,
                IntPtr dwItem1,
                IntPtr dwItem2);
        }
"@
    [Shell32]::SHChangeNotify(0x8000000, 0, [IntPtr]::Zero, [IntPtr]::Zero)
} catch {
    Write-Host "警告：无法刷新文件关联缓存，可能需要重启资源管理器" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "   文件关联设置完成！" -ForegroundColor Green
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "现在可以尝试双击视频文件，应该会用漂亮播放器打开。" -ForegroundColor Yellow
Write-Host ""
Write-Host "如果文件关联没有立即生效，可以尝试："
Write-Host "1. 重启资源管理器（任务管理器 -> 找到Windows资源管理器 -> 右键重启）"
Write-Host "2. 或者重启电脑"
Write-Host ""
Write-Host "按任意键退出..." -ForegroundColor Gray
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')