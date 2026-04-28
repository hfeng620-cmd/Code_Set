@echo off
REM 清理临时文件的批处理文件
REM 可以通过任务计划程序定期执行

echo 开始清理临时文件: %date% %time%

REM 获取脚本所在目录
set SCRIPT_DIR=%~dp0

REM 执行Python脚本
python "%SCRIPT_DIR%clean_temp_files.py" --dir "%SCRIPT_DIR%.." --verbose

echo 清理完成: %date% %time%
