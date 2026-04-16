@echo off
echo Building Mnemosyne application...
echo.

if not exist "build" mkdir build
cd build

cmake ..

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

cmake --build . --config Release

if %errorlevel% neq 0 (
    echo Build failed!
    cd ..
    exit /b 1
)

echo.
echo Build completed successfully!
echo.
echo To run the application, execute: build\Release\Mnemosyne.exe
cd ..
