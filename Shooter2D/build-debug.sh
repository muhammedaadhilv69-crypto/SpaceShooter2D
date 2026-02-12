#!/bin/bash
# GitHub Codespaces - Cross Compile to Windows .exe
# Run this script in your Codespace terminal

echo "=========================================="
echo "Cross-Compiling 2D Shooter for Windows"
echo "=========================================="
echo ""

# Step 1: Install MinGW cross-compiler
echo "[1/4] Installing MinGW-w64 cross-compiler..."
sudo apt-get update -qq
sudo apt-get install -y mingw-w64 wget unzip

if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "ERROR: MinGW not installed properly!"
    exit 1
fi
echo "✓ MinGW installed"

# Step 2: Download SDL2 for MinGW
echo "[2/4] Downloading SDL2 for Windows..."
SDL2_VERSION="2.28.5"

# Remove old SDL2 if exists
rm -rf SDL2 SDL2-*.tar.gz

wget https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-mingw.tar.gz

if [ ! -f "SDL2-devel-${SDL2_VERSION}-mingw.tar.gz" ]; then
    echo "ERROR: Failed to download SDL2!"
    exit 1
fi

tar -xzf SDL2-devel-${SDL2_VERSION}-mingw.tar.gz
mv SDL2-${SDL2_VERSION}/x86_64-w64-mingw32 SDL2
rm -rf SDL2-${SDL2_VERSION} SDL2-devel-${SDL2_VERSION}-mingw.tar.gz

if [ ! -d "SDL2" ]; then
    echo "ERROR: SDL2 directory not created!"
    exit 1
fi
echo "✓ SDL2 downloaded and extracted"

# Verify SDL2 structure
if [ ! -f "SDL2/include/SDL2/SDL.h" ]; then
    echo "ERROR: SDL2 headers not found!"
    echo "Checking SDL2 directory structure..."
    ls -la SDL2/
    ls -la SDL2/include/ 2>/dev/null || echo "No include directory"
    exit 1
fi
echo "✓ SDL2 headers verified"

# Step 3: Cross-compile
echo "[3/4] Cross-compiling to Windows .exe..."

# Verify source files exist
if [ ! -f "main.cpp" ] || [ ! -f "Game.cpp" ] || [ ! -f "Game.h" ]; then
    echo "ERROR: Source files missing!"
    echo "Current directory contents:"
    ls -la
    exit 1
fi
echo "✓ Source files found"

# Compile with verbose output
echo "Compiling..."
x86_64-w64-mingw32-g++ -std=c++14 -O2 \
    -I./SDL2/include/SDL2 \
    -L./SDL2/lib \
    main.cpp Game.cpp \
    -o SpaceShooter.exe \
    -lmingw32 -lSDL2main -lSDL2 \
    -static-libgcc -static-libstdc++ \
    -mwindows

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Compilation failed!"
    echo ""
    echo "Trying compilation with error output..."
    x86_64-w64-mingw32-g++ -std=c++14 -O2 \
        -I./SDL2/include/SDL2 \
        -L./SDL2/lib \
        main.cpp Game.cpp \
        -o SpaceShooter.exe \
        -lmingw32 -lSDL2main -lSDL2 \
        -static-libgcc -static-libstdc++
    exit 1
fi

# Verify .exe was created
if [ ! -f "SpaceShooter.exe" ]; then
    echo "ERROR: SpaceShooter.exe was not created!"
    echo "Directory contents:"
    ls -la
    exit 1
fi
echo "✓ Compilation successful"

# Step 4: Copy SDL2.dll
echo "[4/4] Copying SDL2.dll..."
cp SDL2/bin/SDL2.dll .

if [ ! -f "SDL2.dll" ]; then
    echo "ERROR: SDL2.dll not copied!"
    exit 1
fi
echo "✓ SDL2.dll copied"

echo ""
echo "=========================================="
echo "✓ BUILD SUCCESSFUL!"
echo "=========================================="
echo ""
echo "Files created:"
ls -lh SpaceShooter.exe SDL2.dll
echo ""
echo "Download both files and run SpaceShooter.exe on your Windows PC!"
echo ""
echo "Game Controls:"
echo "  WASD/Arrows - Move"
echo "  SPACE - Shoot"
echo "  R - Restart"
echo "  ESC - Quit"
echo ""