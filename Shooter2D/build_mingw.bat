@echo off
echo Building 2D Space Shooter with MinGW...
echo.

REM Check if g++ is available
where g++.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: MinGW g++ compiler not found!
    echo Please install MinGW-w64 and add it to your PATH
    pause
    exit /b 1
)

REM Compile the game
echo Compiling...
g++ -std=c++14 -O2 -I./SDL2/include -L./SDL2/lib/x64 main.cpp Game.cpp -o SpaceShooter.exe -lmingw32 -lSDL2main -lSDL2 -mwindows

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

REM Copy SDL2.dll
copy SDL2\lib\x64\SDL2.dll . >nul

echo.
echo Build successful! Run SpaceShooter.exe to play!
echo.
pause
