# PowerShell 启动脚本 - 威震天屏幕统计工具
Write-Host "正在启动威震天屏幕统计工具..." -ForegroundColor Green
Write-Host ""

# 切换到 Screen_Tracker 目录
Set-Location -Path "$PSScriptRoot\Screen_Tracker"

# 检查Python环境
$pythonwPath = "D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\pythonw.exe"
if (Test-Path $pythonwPath) {
    Write-Host "使用指定的Python环境..." -ForegroundColor Cyan
    & $pythonwPath ScreenTracker.pyw
} else {
    Write-Host "使用系统Python环境..." -ForegroundColor Cyan
    # 尝试使用 pythonw（无控制台窗口）
    try {
        & pythonw ScreenTracker.pyw
    } catch {
        Write-Host "pythonw 不可用，尝试使用 python..." -ForegroundColor Yellow
        & python ScreenTracker.pyw
    }
}

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "启动失败，请检查：" -ForegroundColor Red
    Write-Host "1. Python是否已安装" -ForegroundColor Yellow
    Write-Host "2. 依赖库是否已安装：pip install pywin32 psutil pillow pystray matplotlib" -ForegroundColor Yellow
    Write-Host "3. 文件是否完整" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "按任意键继续..." -ForegroundColor Gray
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
}