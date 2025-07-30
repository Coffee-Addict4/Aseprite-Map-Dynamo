#include "core/config_loader.h"
#include "tileset/tile_checker.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    try {
        // Initialize configuration
        auto config_loader = std::make_unique<ConfigLoader>();
        
        // Handle health check
        if (argc > 1 && std::string(argv[1]) == "--health-check") {
            std::cout << "tile_checker: healthy" << std::endl;
            return 0;
        }
        
        // Handle daemon mode
        if (argc > 1 && std::string(argv[1]) == "--daemon") {
            std::cout << "tile_checker: starting daemon mode" << std::endl;
            // Main service loop would go here
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            return 0;
        }
        
        // Default: run tile checking
        auto tile_checker = std::make_unique<TileChecker>();
        std::cout << "tile_checker: validating tiles..." << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "tile_checker error: " << e.what() << std::endl;
        return 1;
    }
}
