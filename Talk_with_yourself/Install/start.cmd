@echo off
setlocal

cd /d "%~dp0"

where node >nul 2>nul
if errorlevel 1 (
  echo Node.js was not found in PATH.
  echo Install Node.js first, then run this script again.
  pause
  exit /b 1
)

if not exist "server.js" (
  echo server.js was not found in:
  echo %CD%
  pause
  exit /b 1
)

if "%PORT%"=="" set "PORT=8787"

echo Starting Talk with yourself on http://localhost:%PORT%
echo Press Ctrl+C to stop the server.
echo.

node server.js
set "EXIT_CODE=%ERRORLEVEL%"

echo.
if not "%EXIT_CODE%"=="0" echo Server exited with code %EXIT_CODE%.
pause
exit /b %EXIT_CODE%
