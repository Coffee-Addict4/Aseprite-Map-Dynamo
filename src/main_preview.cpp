#include "core/config_loader.h"
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
            std::cout << "preview_tool: healthy" << std::endl;
            return 0;
        }
        
        // Handle daemon mode
        if (argc > 1 && std::string(argv[1]) == "--daemon") {
            std::cout << "preview_tool: starting daemon mode" << std::endl;
            // Main service loop would go here
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            return 0;
        }
        
        // Default: run preview tool
        std::cout << "preview_tool: starting preview service..." << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "preview_tool error: " << e.what() << std::endl;
        return 1;
    }
}
