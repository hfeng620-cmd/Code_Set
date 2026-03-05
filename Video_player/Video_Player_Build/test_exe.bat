@echo off
chcp 65001 >nul
echo 测试视频播放器可执行文件
echo ========================================
echo.

REM 检查可执行文件是否存在
if not exist "dist\漂亮视频播放器.exe" (
    echo ❌ 错误: 找不到可执行文件 "dist\漂亮视频播放器.exe"
    echo.
    echo 请先运行 pack_video_player.py 打包程序
    pause
    exit /b 1
)

echo ✅ 找到可执行文件: dist\漂亮视频播放器.exe
echo 📏 文件大小: 
for %%F in ("dist\漂亮视频播放器.exe") do echo     %%~zF 字节 (约 %%~zF/1048576 MB)
echo.

echo 🚀 启动视频播放器...
echo 注意: 首次启动可能需要几秒钟时间
echo.

REM 运行可执行文件
start "" "dist\漂亮视频播放器.exe"

echo.
echo ✅ 视频播放器已启动！
echo.
echo 📝 使用说明:
echo   1. 程序窗口会自动打开
echo   2. 点击"打开视频"按钮选择视频文件
echo   3. 按空格键播放/暂停
echo   4. 按 F 键进入/退出全屏
echo   5. 按 ESC 键退出程序
echo.
pause