@echo off
:: 解决中文乱码问题
chcp 65001 >nul

echo ==========================================
echo       正在准备打包：漂亮播放器...
echo ==========================================

:: 1. 强制切换到 D 盘 (根据你的路径)
d:
cd "D:\VS Code\Code_Set"

:: 2. 清理旧文件
echo 正在清理旧文件...
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist
del /q *.spec

:: 3. 开始打包
:: -n "漂亮" : 软件改名叫漂亮
:: -i "favicon.ico" : 使用新图标
echo 正在调用 PyInstaller 开始打包...
pyinstaller -D -w -n "漂亮" -i "favicon.ico" Video_Player.py

:: 4. 自动复制新图标到生成的文件夹里
echo 正在复制图标文件...
copy "favicon.ico" "dist\漂亮\"

echo ==========================================
echo               打包完成！
echo      请去 dist 文件夹查看你的程序
echo ==========================================
pause