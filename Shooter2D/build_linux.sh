#!/bin/bash

echo "Building 2D Space Shooter for Linux..."
echo ""

# Check if SDL2 is installed
if ! pkg-config --exists sdl2; then
    echo "Error: SDL2 development libraries not found!"
    echo "Please install SDL2:"
    echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev"
    echo "  Fedora: sudo dnf install SDL2-devel"
    echo "  Arch: sudo pacman -S sdl2"
    exit 1
fi

# Build using Makefile
echo "Compiling..."
make clean
make

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful! Run ./SpaceShooter to play!"
    echo ""
else
    echo "Build failed!"
    exit 1
fi
