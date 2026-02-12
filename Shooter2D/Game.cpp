#include "Game.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Game::Game() 
    : window(nullptr), renderer(nullptr), running(false), gameOver(false),
      player(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT - 80),
      score(0), enemySpawnTimer(0), gameTime(0),
      rng(std::random_device{}()) {
}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }
    
    window = SDL_CreateWindow("2D Space Shooter",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH, SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return false;
    }
    
    running = true;
    return true;
}

void Game::cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void Game::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
        }
        if (gameOver && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
            // Restart game
            gameOver = false;
            player.health = 100;
            player.x = SCREEN_WIDTH / 2 - 20;
            player.y = SCREEN_HEIGHT - 80;
            bullets.clear();
            enemies.clear();
            particles.clear();
            score = 0;
            gameTime = 0;
            enemySpawnTimer = 0;
        }
    }
    
    if (gameOver) return;
    
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    
    // Player movement
    player.vx = 0;
    player.vy = 0;
    
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
        player.vx = -300;
    }
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
        player.vx = 300;
    }
    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
        player.vy = -300;
    }
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
        player.vy = 300;
    }
    
    // Shooting
    if (keyState[SDL_SCANCODE_SPACE] && player.shootCooldown <= 0) {
        bullets.emplace_back(player.x + player.width / 2 - 2, player.y, -500, true);
        player.shootCooldown = 0.2f;
    }
}

void Game::update(float deltaTime) {
    if (gameOver) return;
    
    gameTime += deltaTime;
    player.shootCooldown -= deltaTime;
    enemySpawnTimer -= deltaTime;
    
    // Spawn enemies
    if (enemySpawnTimer <= 0) {
        spawnEnemy();
        enemySpawnTimer = std::max(0.5f, 2.0f - gameTime * 0.05f); // Increase difficulty over time
    }
    
    // Update player
    player.x += player.vx * deltaTime;
    player.y += player.vy * deltaTime;
    
    // Keep player in bounds
    player.x = std::max(0.0f, std::min(player.x, SCREEN_WIDTH - player.width));
    player.y = std::max(0.0f, std::min(player.y, SCREEN_HEIGHT - player.height));
    
    // Update bullets
    for (auto& bullet : bullets) {
        if (!bullet.active) continue;
        
        bullet.y += bullet.vy * deltaTime;
        
        // Deactivate bullets that go off screen
        if (bullet.y < -20 || bullet.y > SCREEN_HEIGHT + 20) {
            bullet.active = false;
        }
    }
    
    // Update enemies
    for (auto& enemy : enemies) {
        if (!enemy.active) continue;
        
        enemy.y += enemy.vy * deltaTime;
        enemy.shootTimer += deltaTime;
        
        // Enemy shooting
        if (enemy.shootTimer >= 2.0f) {
            bullets.emplace_back(enemy.x + enemy.width / 2 - 2, enemy.y + enemy.height, 300, false);
            enemy.shootTimer = 0;
        }
        
        // Deactivate enemies that go off screen
        if (enemy.y > SCREEN_HEIGHT + 50) {
            enemy.active = false;
        }
    }
    
    // Update particles
    for (auto& particle : particles) {
        if (!particle.active) continue;
        
        particle.x += particle.vx * deltaTime;
        particle.y += particle.vy * deltaTime;
        particle.life -= deltaTime * 2;
        
        if (particle.life <= 0) {
            particle.active = false;
        }
    }
    
    // Check collisions
    checkCollisions();
    
    // Check if player is dead
    if (player.health <= 0) {
        gameOver = true;
        spawnParticles(player.x + player.width / 2, player.y + player.height / 2, 100, 200, 255, 30);
    }
    
    // Clean up inactive objects
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                  [](const Bullet& b) { return !b.active; }), bullets.end());
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                  [](const Enemy& e) { return !e.active; }), enemies.end());
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                    [](const Particle& p) { return !p.active; }), particles.end());
}

void Game::checkCollisions() {
    // Player bullets vs enemies
    for (auto& bullet : bullets) {
        if (!bullet.active || !bullet.fromPlayer) continue;
        
        for (auto& enemy : enemies) {
            if (!enemy.active) continue;
            
            if (bullet.collidesWith(enemy)) {
                bullet.active = false;
                enemy.health--;
                
                spawnParticles(enemy.x + enemy.width / 2, enemy.y + enemy.height / 2, 
                             255, 100, 100, 5);
                
                if (enemy.health <= 0) {
                    enemy.active = false;
                    score += 10;
                    spawnParticles(enemy.x + enemy.width / 2, enemy.y + enemy.height / 2, 
                                 255, 150, 0, 20);
                }
                break;
            }
        }
    }
    
    // Enemy bullets vs player
    for (auto& bullet : bullets) {
        if (!bullet.active || bullet.fromPlayer) continue;
        
        if (bullet.collidesWith(player)) {
            bullet.active = false;
            player.health -= 10;
            spawnParticles(player.x + player.width / 2, player.y + player.height / 2, 
                         255, 50, 50, 8);
        }
    }
    
    // Enemies vs player
    for (auto& enemy : enemies) {
        if (!enemy.active) continue;
        
        if (enemy.collidesWith(player)) {
            enemy.active = false;
            player.health -= 20;
            spawnParticles(player.x + player.width / 2, player.y + player.height / 2, 
                         255, 0, 0, 15);
        }
    }
}

void Game::spawnEnemy() {
    std::uniform_real_distribution<float> dist(20, SCREEN_WIDTH - 60);
    float x = dist(rng);
    enemies.emplace_back(x, -50);
}

void Game::spawnParticles(float x, float y, Uint8 r, Uint8 g, Uint8 b, int count) {
    std::uniform_real_distribution<float> angleDist(0, 2 * M_PI);
    std::uniform_real_distribution<float> speedDist(50, 200);
    
    for (int i = 0; i < count; i++) {
        float angle = angleDist(rng);
        float speed = speedDist(rng);
        float vx = cos(angle) * speed;
        float vy = sin(angle) * speed;
        particles.emplace_back(x, y, vx, vy, r, g, b);
    }
}

void Game::render() {
    // Clear screen (dark blue background)
    SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);
    SDL_RenderClear(renderer);
    
    if (!gameOver) {
        // Draw stars background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < 100; i++) {
            int sx = (i * 37) % SCREEN_WIDTH;
            int sy = (i * 73 + (int)(gameTime * 20)) % SCREEN_HEIGHT;
            SDL_RenderDrawPoint(renderer, sx, sy);
        }
        
        // Draw player
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect playerRect = {(int)player.x, (int)player.y, (int)player.width, (int)player.height};
        SDL_RenderFillRect(renderer, &playerRect);
        
        // Draw player cockpit
        SDL_SetRenderDrawColor(renderer, 150, 220, 255, 255);
        SDL_Rect cockpit = {(int)player.x + 15, (int)player.y + 5, 10, 10};
        SDL_RenderFillRect(renderer, &cockpit);
        
        // Draw bullets
        for (const auto& bullet : bullets) {
            if (!bullet.active) continue;
            
            if (bullet.fromPlayer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            }
            
            SDL_Rect bulletRect = {(int)bullet.x, (int)bullet.y, (int)bullet.width, (int)bullet.height};
            SDL_RenderFillRect(renderer, &bulletRect);
        }
        
        // Draw enemies
        for (const auto& enemy : enemies) {
            if (!enemy.active) continue;
            
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            SDL_Rect enemyRect = {(int)enemy.x, (int)enemy.y, (int)enemy.width, (int)enemy.height};
            SDL_RenderFillRect(renderer, &enemyRect);
            
            // Enemy eyes
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
            SDL_Rect eye1 = {(int)enemy.x + 10, (int)enemy.y + 15, 8, 8};
            SDL_Rect eye2 = {(int)enemy.x + 22, (int)enemy.y + 15, 8, 8};
            SDL_RenderFillRect(renderer, &eye1);
            SDL_RenderFillRect(renderer, &eye2);
        }
        
        // Draw particles
        for (const auto& particle : particles) {
            if (!particle.active) continue;
            
            Uint8 alpha = (Uint8)(particle.life * 255);
            SDL_SetRenderDrawColor(renderer, particle.r, particle.g, particle.b, alpha);
            SDL_Rect particleRect = {(int)particle.x, (int)particle.y, (int)particle.width, (int)particle.height};
            SDL_RenderFillRect(renderer, &particleRect);
        }
        
        // Draw HUD
        // Health bar
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect healthBg = {10, 10, 200, 20};
        SDL_RenderFillRect(renderer, &healthBg);
        
        int healthWidth = (player.health * 200) / 100;
        Uint8 healthR = 255 - (player.health * 155 / 100);
        Uint8 healthG = (player.health * 255 / 100);
        SDL_SetRenderDrawColor(renderer, healthR, healthG, 0, 255);
        SDL_Rect healthBar = {10, 10, healthWidth, 20};
        SDL_RenderFillRect(renderer, &healthBar);
        
        // Score (simple pixel text)
        renderText(("SCORE: " + std::to_string(score)).c_str(), 10, 40, 2);
        
    } else {
        // Game over screen
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        renderText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, 4);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        renderText(("FINAL SCORE: " + std::to_string(score)).c_str(), 
                  SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2, 2);
        
        renderText("PRESS R TO RESTART", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 40, 2);
    }
    
    SDL_RenderPresent(renderer);
}

void Game::renderText(const char* text, int x, int y, int size) {
    // Simple bitmap font rendering using rectangles
    int cursorX = x;
    int cursorY = y;
    
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        
        if (c == ' ') {
            cursorX += 6 * size;
            continue;
        }
        
        if (c >= '0' && c <= '9') {
            // Draw digit
            int digit = c - '0';
            // Simple pattern for numbers
            SDL_Rect rect = {cursorX, cursorY, 4 * size, 6 * size};
            SDL_RenderFillRect(renderer, &rect);
        } else {
            // Draw letter (simplified)
            SDL_Rect rect = {cursorX, cursorY, 4 * size, 6 * size};
            SDL_RenderFillRect(renderer, &rect);
        }
        
        cursorX += 6 * size;
    }
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Cap delta time to prevent spiral of death
        if (deltaTime > 0.05f) {
            deltaTime = 0.05f;
        }
        
        handleInput();
        update(deltaTime);
        render();
        
        SDL_Delay(1); // Small delay to prevent 100% CPU usage
    }
}
