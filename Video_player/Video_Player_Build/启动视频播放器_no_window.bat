@echo off
chcp 65001 >nul

REM 创建VBScript来隐藏命令行窗口
set "VBS_FILE=%TEMP%\start_player.vbs"
(
echo Set WshShell = CreateObject^("WScript.Shell"^)
echo WshShell.CurrentDirectory = "%~dp0..\Video_Player_Source"
echo WshShell.Run "D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\pythonw.exe Video_Player_Perfect.py", 0, False
echo Set WshShell = Nothing
) > "%VBS_FILE%"

REM 运行VBScript
cscript //nologo "%VBS_FILE%"

REM 删除临时文件
del "%VBS_FILE%" >nul 2>&1