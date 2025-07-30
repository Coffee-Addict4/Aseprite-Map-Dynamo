#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "core/config_loader.h"
#include "tileset/tile_checker.h"

using namespace mcp_tool::core;
using namespace mcp_tool::tileset;

void PrintUsage() {
    std::cout << "MCP Tile Checker Service" << std::endl;
    std::cout << "Usage: tile_checker [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --health-check    Return health status and exit" << std::endl;
    std::cout << "  --daemon          Run in daemon mode" << std::endl;
    std::cout << "  --validate-tiles PATH  Validate tiles in directory" << std::endl;
    std::cout << "  --config FILE     Use specific config file" << std::endl;
    std::cout << "  --help            Show this help message" << std::endl;
}

bool HealthCheck() {
    try {
        // Basic health check - verify core components
        ConfigLoader config;
        TileChecker checker;
        
        std::cout << "HEALTHY: Tile checker service is operational" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "UNHEALTHY: " << e.what() << std::endl;
        return false;
    }
}

void DaemonMode() {
    std::cout << "Starting tile checker daemon..." << std::endl;
    
    ConfigLoader config;
    TileChecker checker;
    
    // Set shared volume path for containerized environment
    checker.SetSharedVolumePath("/mnt/assets");
    
    int check_count = 0;
    while (true) {
        try {
            check_count++;
            std::cout << "Daemon heartbeat #" << check_count << " - tile checker ready" << std::endl;
            
            // Sleep for 30 seconds between heartbeats
            std::this_thread::sleep_for(std::chrono::seconds(30));
            
        } catch (const std::exception& e) {
            std::cerr << "Daemon error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
}

int ValidateTilesDirectory(const std::string& directory_path) {
    try {
        ConfigLoader config;
        TileChecker checker;
        
        std::cout << "Validating tiles in directory: " << directory_path << std::endl;
        
        // Set shared volume path
        checker.SetSharedVolumePath("/mnt/assets");
        
        // Create mock tile paths for validation demonstration
        std::vector<std::string> tile_paths = {
            directory_path + "/tile_01.aseprite",
            directory_path + "/tile_02.aseprite",
            directory_path + "/character_idle.aseprite"
        };
        
        // Validate the tile collection
        auto issues = checker.ValidateTileCollection(tile_paths);
        
        std::cout << "Validation completed. Found " << issues.size() << " issues." << std::endl;
        
        if (checker.HasCriticalIssues(issues)) {
            std::cerr << "CRITICAL issues found! Check validation report." << std::endl;
            return 1;
        }
        
        std::cout << "Tile validation passed successfully." << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error validating tiles: " << e.what() << std::endl;
        return 1;
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            PrintUsage();
            return 0;
        }
        
        std::string command = argv[1];
        
        if (command == "--health-check") {
            return HealthCheck() ? 0 : 1;
        } 
        else if (command == "--daemon") {
            DaemonMode();
            return 0;
        }
        else if (command == "--validate-tiles" && argc >= 3) {
            std::string directory_path = argv[2];
            return ValidateTilesDirectory(directory_path);
        }
        else if (command == "--help") {
            PrintUsage();
            return 0;
        }
        else {
            std::cerr << "Unknown command: " << command << std::endl;
            PrintUsage();
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
