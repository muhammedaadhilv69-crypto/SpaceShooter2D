#!/bin/bash
# Package script for creating distributable zip file

echo "Creating distribution package..."

# Create dist directory
mkdir -p dist/2d_shooter_game

# Copy source files
cp main.cpp dist/2d_shooter_game/
cp Game.h dist/2d_shooter_game/
cp Game.cpp dist/2d_shooter_game/

# Copy build files
cp CMakeLists.txt dist/2d_shooter_game/
cp Makefile dist/2d_shooter_game/
cp build_windows.bat dist/2d_shooter_game/
cp build_mingw.bat dist/2d_shooter_game/
cp build_linux.sh dist/2d_shooter_game/

# Copy documentation
cp README.md dist/2d_shooter_game/
cp SETUP.txt dist/2d_shooter_game/
cp DESIGN.md dist/2d_shooter_game/

# Create placeholder for SDL2 (user must download)
mkdir -p dist/2d_shooter_game/SDL2
echo "DOWNLOAD SDL2 LIBRARIES AND PLACE HERE" > dist/2d_shooter_game/SDL2/README.txt
echo "" >> dist/2d_shooter_game/SDL2/README.txt
echo "Windows: https://www.libsdl.org/download-2.0.php" >> dist/2d_shooter_game/SDL2/README.txt
echo "Download SDL2-devel-2.x.x-mingw.tar.gz or SDL2-devel-2.x.x-VC.zip" >> dist/2d_shooter_game/SDL2/README.txt
echo "" >> dist/2d_shooter_game/SDL2/README.txt
echo "Extract and place the contents so you have:" >> dist/2d_shooter_game/SDL2/README.txt
echo "  SDL2/include/SDL2/" >> dist/2d_shooter_game/SDL2/README.txt
echo "  SDL2/lib/" >> dist/2d_shooter_game/SDL2/README.txt

# Create zip file
cd dist
zip -r 2d_shooter_game.zip 2d_shooter_game/
cd ..

echo "Package created: dist/2d_shooter_game.zip"
echo "Done!"
