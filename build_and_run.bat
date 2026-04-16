@echo off
call build.bat
if %errorlevel% neq 0 exit /b %errorlevel%

echo Running Mnemosyne...
build\Release\Mnemosyne.exe
