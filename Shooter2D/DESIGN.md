# Game Design Document - 2D Space Shooter

## Overview
A vertical scrolling space shooter where the player controls a spaceship, shoots enemies, and tries to survive as long as possible.

## Core Mechanics

### Player Controls
- **Movement**: WASD or Arrow Keys (8-directional movement)
- **Shooting**: SPACE bar (automatic fire with cooldown)
- **Speed**: 300 pixels/second
- **Shoot Cooldown**: 0.2 seconds

### Player Stats
- **Health**: 100 HP
- **Size**: 40x40 pixels
- **Color**: Light blue (100, 200, 255)
- **Position**: Bottom center of screen

### Enemies
- **Size**: 40x40 pixels
- **Color**: Red (255, 50, 50)
- **Health**: 2 HP
- **Speed**: 30 pixels/second (downward)
- **Shoot Rate**: Every 2 seconds
- **Points**: 10 per kill

### Bullets
- **Player Bullets**: Yellow (255, 255, 100), travel upward at 500 px/s
- **Enemy Bullets**: Red (255, 50, 50), travel downward at 300 px/s
- **Size**: 4x10 pixels
- **Damage**: 10 HP to player, 1 HP to enemies

### Collision Damage
- **Enemy Bullet Hit**: -10 HP
- **Enemy Collision**: -20 HP (destroys enemy)

## Game Progression

### Difficulty Scaling
Enemy spawn rate increases over time:
- Starting spawn rate: 2 seconds
- Minimum spawn rate: 0.5 seconds
- Formula: max(0.5, 2.0 - gameTime * 0.05)

### Score System
- Enemy destroyed: +10 points
- No points for survival time (pure kill-based scoring)

## Visual Effects

### Particles
Particles spawn on various events:
- **Hit enemy** (not killed): 5 red/orange particles
- **Enemy destroyed**: 20 orange particles
- **Player hit by bullet**: 8 red particles
- **Player hit by enemy**: 15 red particles
- **Player death**: 30 blue particles

Particle properties:
- Random velocity in all directions (50-200 px/s)
- Fade out over 0.5 seconds
- Size: 3x3 pixels

### Background
- Dark blue space (10, 10, 30)
- 100 scrolling white stars for parallax effect

### HUD
- **Health Bar**: Top-left corner
  - Background: Dark gray (50, 50, 50)
  - Fill: Green to red gradient based on health
  - Size: 200x20 pixels
- **Score**: Below health bar, simple text rendering

## Game States

### Playing
- Player can move and shoot
- Enemies spawn and attack
- Collisions are detected
- Score increments

### Game Over
- Triggered when player health ≤ 0
- Shows "GAME OVER" message
- Displays final score
- Press R to restart

## Technical Implementation

### Architecture
- **Entity-based design**: Base Entity class with position, size, velocity
- **Component pattern**: Separate systems for rendering, physics, collisions
- **Event system**: Particles spawn based on game events

### Update Loop
1. Handle input
2. Update player position
3. Update all bullets
4. Update all enemies (movement + shooting)
5. Update particles
6. Check all collisions
7. Remove inactive entities
8. Spawn new enemies if needed

### Collision Detection
Simple AABB (Axis-Aligned Bounding Box) collision:
- Player bullets vs enemies
- Enemy bullets vs player
- Enemies vs player

### Performance Optimizations
- Inactive entities are removed each frame
- Delta time capping (max 0.05s) prevents spiral of death
- Simple rendering (no textures, just colored rectangles)

## Code Structure

### Files
- **main.cpp**: Entry point, initializes game
- **Game.h**: Class declarations and structures
- **Game.cpp**: Game logic implementation

### Key Classes
- **Entity**: Base class (position, size, velocity, collision)
- **Player**: Player ship with health and shoot cooldown
- **Enemy**: Enemy ship with health and shoot timer
- **Bullet**: Projectile with direction flag
- **Particle**: Visual effect with lifetime
- **Game**: Main game manager

## Future Enhancements (Not Implemented)
- Power-ups (faster shooting, shields, multi-shot)
- Boss enemies every N points
- Multiple enemy types
- Sound effects and music
- Different weapons
- Local high score saving
- Multiple difficulty levels
- Player lives system
- Better graphics/sprites

## Development Notes
- Uses SDL2 for cross-platform compatibility
- No external dependencies beyond SDL2
- Simple collision detection (no complex physics)
- Particle system for visual feedback
- Pure C++14 (no C++17/20 features for wider compatibility)
