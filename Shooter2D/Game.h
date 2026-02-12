#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include <random>

// Entity base class
struct Entity {
    float x, y;
    float width, height;
    float vx, vy;
    bool active;
    
    Entity(float px, float py, float w, float h) 
        : x(px), y(py), width(w), height(h), vx(0), vy(0), active(true) {}
    
    bool collidesWith(const Entity& other) const {
        return active && other.active &&
               x < other.x + other.width &&
               x + width > other.x &&
               y < other.y + other.height &&
               y + height > other.y;
    }
};

// Bullet class
struct Bullet : public Entity {
    bool fromPlayer;
    
    Bullet(float px, float py, float velocityY, bool player)
        : Entity(px, py, 4, 10), fromPlayer(player) {
        vy = velocityY;
    }
};

// Enemy class
struct Enemy : public Entity {
    float shootTimer;
    int health;
    
    Enemy(float px, float py)
        : Entity(px, py, 40, 40), shootTimer(0), health(2) {
        vy = 30.0f;
    }
};

// Player class
struct Player : public Entity {
    int health;
    float shootCooldown;
    
    Player(float px, float py)
        : Entity(px, py, 40, 40), health(100), shootCooldown(0) {}
};

// Particle class for explosions
struct Particle : public Entity {
    float life;
    Uint8 r, g, b;
    
    Particle(float px, float py, float vx_, float vy_, Uint8 red, Uint8 green, Uint8 blue)
        : Entity(px, py, 3, 3), life(1.0f) {
        vx = vx_;
        vy = vy_;
        r = red;
        g = green;
        b = blue;
    }
};

// Main Game class
class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    bool gameOver;
    
    Player player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Particle> particles;
    
    int score;
    float enemySpawnTimer;
    float gameTime;
    
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    
    std::mt19937 rng;
    
    void handleInput();
    void update(float deltaTime);
    void render();
    void spawnEnemy();
    void spawnParticles(float x, float y, Uint8 r, Uint8 g, Uint8 b, int count);
    void checkCollisions();
    void renderText(const char* text, int x, int y, int size);
    
public:
    Game();
    ~Game();
    bool init();
    void run();
    void cleanup();
};

#endif
