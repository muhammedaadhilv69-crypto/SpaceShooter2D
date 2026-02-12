@echo off
echo Building 2D Space Shooter for Windows...
echo.

REM Check if Visual Studio is available
where cl.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: Visual Studio C++ compiler not found!
    echo Please run this from a Visual Studio Developer Command Prompt
    echo or install MinGW-w64 and use build_mingw.bat instead
    pause
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

REM Run CMake
cmake .. -G "Visual Studio 16 2019" -A x64
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

REM Build the project
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo Build successful! Executable is in build\Release\SpaceShooter.exe
echo.
cd ..
pause
