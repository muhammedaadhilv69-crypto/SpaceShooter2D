#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;
    
    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }
    
    std::cout << "=== 2D SPACE SHOOTER ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD or Arrow Keys - Move" << std::endl;
    std::cout << "  SPACE - Shoot" << std::endl;
    std::cout << "  R - Restart (when game over)" << std::endl;
    std::cout << "========================" << std::endl;
    
    game.run();
    game.cleanup();
    
    return 0;
}
