@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0SetAppVolume.ps1" -Group game -Action togglemute
