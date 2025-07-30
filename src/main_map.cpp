#include "core/config_loader.h"
#include "map/map_maker.h"
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
            std::cout << "map_maker: healthy" << std::endl;
            return 0;
        }
        
        // Handle daemon mode
        if (argc > 1 && std::string(argv[1]) == "--daemon") {
            std::cout << "map_maker: starting daemon mode" << std::endl;
            // Main service loop would go here
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            return 0;
        }
        
        // Default: run map generation
        auto map_maker = std::make_unique<MapMaker>();
        std::cout << "map_maker: generating maps..." << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "map_maker error: " << e.what() << std::endl;
        return 1;
    }
}
