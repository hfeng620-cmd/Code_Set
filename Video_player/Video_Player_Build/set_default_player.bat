@echo off
chcp 65001 >nul
echo ==========================================
echo       设置漂亮播放器为默认视频播放器
echo ==========================================

echo.
echo 方法1：通过Windows设置
echo 1. 右键点击任意视频文件（如.mp4文件）
echo 2. 选择"打开方式" -> "选择其他应用"
echo 3. 点击"更多应用"
echo 4. 滚动到底部，点击"在这台电脑上查找其他应用"
echo 5. 浏览到：D:\VS_Code\Code_Set\dist\漂亮\漂亮.exe
echo 6. 选中"始终使用此应用打开.mp4文件"
echo 7. 点击"确定"

echo.
echo 方法2：通过命令设置（需要管理员权限）
echo 注意：以下命令需要以管理员身份运行
echo.
echo 设置.mp4文件关联：
echo assoc .mp4=漂亮播放器.mp4
echo ftype 漂亮播放器.mp4="D:\VS_Code\Code_Set\dist\漂亮\漂亮.exe" "%%1"

echo.
echo 设置其他视频格式：
echo assoc .avi=漂亮播放器.avi
echo ftype 漂亮播放器.avi="D:\VS_Code\Code_Set\dist\漂亮\漂亮.exe" "%%1"
echo assoc .mkv=漂亮播放器.mkv
echo ftype 漂亮播放器.mkv="D:\VS_Code\Code_Set\dist\漂亮\漂亮.exe" "%%1"
echo assoc .wmv=漂亮播放器.wmv
echo ftype 漂亮播放器.wmv="D:\VS_Code\Code_Set\dist\漂亮\漂亮.exe" "%%1"
echo assoc .mov=漂亮播放器.mov
echo ftype 漂亮播放器.mov="D:\VS_Code\Code_Set\dist\漂亮\漂亮.exe" "%%1"

echo.
echo 要应用这些设置，请以管理员身份运行此批处理文件。
echo 或者手动复制上述命令到管理员命令提示符中执行。

echo.
echo ==========================================
echo       完成！
echo ==========================================
pause