# 2D Space Shooter Game

A classic 2D space shooter game written in C++ using SDL2. Shoot enemies, dodge bullets, and survive as long as possible!

## Features

- **Classic arcade gameplay** - Vertical scrolling shooter
- **Enemy AI** - Enemies that move and shoot back at you
- **Particle effects** - Explosions and visual feedback
- **Score tracking** - Compete for high scores
- **Health system** - Take damage and try to survive
- **Progressive difficulty** - Game gets harder over time
- **Simple controls** - Easy to learn, hard to master

## Game Controls

- **WASD** or **Arrow Keys** - Move your ship
- **SPACE** - Shoot
- **R** - Restart (when game over)
- **Close window** or **ESC** - Quit game

## Gameplay

- Destroy red enemy ships for points (10 points each)
- Avoid enemy bullets (red) and collisions
- Your bullets are yellow
- Health bar is shown in the top-left corner
- Survive as long as possible and beat your high score!

## Building the Game

### Windows

#### Prerequisites
You need ONE of the following:
1. **Visual Studio 2019 or later** with C++ tools, OR
2. **MinGW-w64** (recommended for simpler setup)

#### Option 1: MinGW (Recommended)
1. Install MinGW-w64 from: https://www.mingw-w64.org/
2. Download SDL2 development libraries for MinGW:
   - Go to https://www.libsdl.org/download-2.0.php
   - Download "SDL2-devel-2.x.x-mingw.tar.gz"
   - Extract it and copy the SDL2 folder to the game directory
3. Run `build_mingw.bat`

#### Option 2: Visual Studio
1. Install Visual Studio with C++ support
2. Download SDL2 development libraries:
   - Go to https://www.libsdl.org/download-2.0.php
   - Download "SDL2-devel-2.x.x-VC.zip"
   - Extract it and rename to SDL2, place in game directory
3. Open "Developer Command Prompt for VS"
4. Navigate to the game directory
5. Run `build_windows.bat`

### Linux

#### Prerequisites
Install SDL2 development libraries:

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev g++ make
```

**Fedora:**
```bash
sudo dnf install SDL2-devel gcc-c++ make
```

**Arch Linux:**
```bash
sudo pacman -S sdl2 gcc make
```

#### Building
```bash
./build_linux.sh
```

Or manually with make:
```bash
make
```

#### Running
```bash
./SpaceShooter
```

## File Structure

```
2d_shooter_game/
├── main.cpp              # Entry point
├── Game.h                # Game class header
├── Game.cpp              # Game implementation
├── CMakeLists.txt        # CMake build configuration
├── Makefile              # Linux Makefile
├── build_windows.bat     # Windows build script (Visual Studio)
├── build_mingw.bat       # Windows build script (MinGW)
├── build_linux.sh        # Linux build script
├── README.md             # This file
└── SDL2/                 # SDL2 libraries (you need to download)
```

## Troubleshooting

### Windows
- **"SDL2 not found"**: Make sure SDL2 folder is in the game directory with include/ and lib/ subdirectories
- **"Cannot find SDL2.dll"**: The DLL should be copied automatically. If not, manually copy SDL2/lib/x64/SDL2.dll to the same folder as the .exe
- **MinGW not found**: Make sure MinGW bin directory is in your PATH

### Linux
- **"SDL2 not found"**: Install SDL2 development package (see Prerequisites)
- **Permission denied**: Run `chmod +x build_linux.sh` and `chmod +x SpaceShooter`

## Technical Details

- **Language**: C++14
- **Library**: SDL2 (Simple DirectMedia Layer)
- **Graphics**: 2D rendering with SDL2
- **Platform**: Windows and Linux compatible
- **Dependencies**: SDL2 only (included for Windows builds)

## Tips for High Scores

1. Keep moving - Don't stay in one place
2. Shoot constantly - More bullets = more hits
3. Watch for enemy bullet patterns
4. Destroy enemies quickly before they shoot
5. Use the entire screen - Don't corner yourself

## Credits

Created as a demonstration of C++ game development with SDL2.

## License

Free to use, modify, and distribute.

Enjoy the game!
