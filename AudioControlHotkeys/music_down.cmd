@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0SetAppVolume.ps1" -Group music -Action down -Step 2
