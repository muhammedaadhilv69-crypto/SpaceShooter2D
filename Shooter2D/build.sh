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

# Step 2: Download SDL2 for MinGW
echo "[2/4] Downloading SDL2 for Windows..."
SDL2_VERSION="2.28.5"
wget -q https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-mingw.tar.gz
tar -xzf SDL2-devel-${SDL2_VERSION}-mingw.tar.gz
mv SDL2-${SDL2_VERSION}/x86_64-w64-mingw32 SDL2
rm -rf SDL2-${SDL2_VERSION} SDL2-devel-${SDL2_VERSION}-mingw.tar.gz

# Step 3: Cross-compile
echo "[3/4] Cross-compiling to Windows .exe..."
x86_64-w64-mingw32-g++ -std=c++14 -O2 \
    -I./SDL2/include/SDL2 \
    -L./SDL2/lib \
    main.cpp Game.cpp \
    -o SpaceShooter.exe \
    -lmingw32 -lSDL2main -lSDL2 \
    -static-libgcc -static-libstdc++ \
    -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic \
    -mwindows

# Step 4: Copy SDL2.dll
echo "[4/4] Copying SDL2.dll..."
cp SDL2/bin/SDL2.dll .

echo ""
echo "=========================================="
echo "✓ BUILD SUCCESSFUL!"
echo "=========================================="
echo ""
echo "Files created:"
echo "  - SpaceShooter.exe (the game)"
echo "  - SDL2.dll (required library)"
echo ""
echo "Download both files and run SpaceShooter.exe on your Windows PC!"
echo ""
echo "Game Controls:"
echo "  WASD/Arrows - Move"
echo "  SPACE - Shoot"
echo "  R - Restart"
echo "  ESC - Quit"
echo ""