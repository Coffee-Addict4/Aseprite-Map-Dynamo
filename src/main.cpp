#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "core/config_loader.h"
#include "core/task_router.h"
#include "tileset/sprite_tool.h"
#include "tileset/tile_checker.h"
#include "map/map_maker.h"

using namespace mcp_tool;

void PrintUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] command [args...]" << std::endl;
    std::cout << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  tileset <input_path> <output_path>  - Process tileset from Aseprite files" << std::endl;
    std::cout << "  map <tileset_path> <config_path> <output_path>  - Generate map from tileset" << std::endl;
    std::cout << "  validate <sprite_path>  - Validate sprite or tileset" << std::endl;
    std::cout << "  daemon  - Run as daemon for Docker orchestration" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --config <path>     - Configuration file path" << std::endl;
    std::cout << "  --guidelines <path> - Strict guidelines file path" << std::endl;
    std::cout << "  --adaptive <path>   - Adaptive rules file path" << std::endl;
    std::cout << "  --help              - Show this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            PrintUsage(argv[0]);
            return 1;
        }

        // Parse command line arguments
        std::string config_path = "config/strict-guidelines.json";
        std::string guidelines_path = "config/strict-guidelines.json";
        std::string adaptive_path = "config/adaptive-rules.json";
        std::string command;
        std::vector<std::string> args;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "--help") {
                PrintUsage(argv[0]);
                return 0;
            } else if (arg == "--config" && i + 1 < argc) {
                config_path = argv[++i];
            } else if (arg == "--guidelines" && i + 1 < argc) {
                guidelines_path = argv[++i];
            } else if (arg == "--adaptive" && i + 1 < argc) {
                adaptive_path = argv[++i];
            } else if (command.empty()) {
                command = arg;
            } else {
                args.push_back(arg);
            }
        }

        // Initialize configuration
        auto config_loader = std::make_unique<core::ConfigLoader>();
        
        if (!config_loader->LoadStrictGuidelines(guidelines_path)) {
            std::cerr << "Failed to load strict guidelines from: " << guidelines_path << std::endl;
            return 1;
        }
        
        if (!config_loader->LoadAdaptiveRules(adaptive_path)) {
            std::cerr << "Failed to load adaptive rules from: " << adaptive_path << std::endl;
            return 1;
        }

        if (!config_loader->ValidateConfig()) {
            std::cerr << "Configuration validation failed" << std::endl;
            return 1;
        }

        // Initialize task router
        auto task_router = std::make_unique<core::TaskRouter>();
        task_router->Start();

        // Execute command
        if (command == "tileset") {
            if (args.size() < 2) {
                std::cerr << "Tileset command requires input_path and output_path" << std::endl;
                return 1;
            }
            
            std::cout << "Processing tileset from " << args[0] << " to " << args[1] << std::endl;
            bool success = task_router->ExecuteTilesetPipeline(args[0], args[1]);
            
            if (success) {
                std::cout << "Tileset processing completed successfully" << std::endl;
            } else {
                std::cerr << "Tileset processing failed" << std::endl;
                return 1;
            }
            
        } else if (command == "map") {
            if (args.size() < 3) {
                std::cerr << "Map command requires tileset_path, config_path, and output_path" << std::endl;
                return 1;
            }
            
            std::cout << "Generating map from tileset " << args[0] << std::endl;
            bool success = task_router->ExecuteMapPipeline(args[0], args[1], args[2]);
            
            if (success) {
                std::cout << "Map generation completed successfully" << std::endl;
            } else {
                std::cerr << "Map generation failed" << std::endl;
                return 1;
            }
            
        } else if (command == "validate") {
            if (args.empty()) {
                std::cerr << "Validate command requires sprite_path" << std::endl;
                return 1;
            }
            
            auto tile_checker = std::make_unique<tileset::TileChecker>();
            tile_checker->SetSharedVolumePath(config_loader->GetSharedVolumePath());
            
            auto issues = tile_checker->ValidateTileset(args[0]);
            
            std::cout << "Validation completed. Found " << issues.size() << " issues." << std::endl;
            
            bool has_critical = tile_checker->HasCriticalIssues(issues);
            if (has_critical) {
                std::cerr << "Critical issues found!" << std::endl;
                return 1;
            }
            
        } else if (command == "daemon") {
            std::cout << "Running in daemon mode..." << std::endl;
            
            // Start health monitoring
            task_router->StartHealthMonitoring();
            
            // Keep running until stopped
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                // Could add signal handling here for graceful shutdown
            }
            
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
            PrintUsage(argv[0]);
            return 1;
        }

        // Wait for all tasks to complete
        task_router->WaitForAllTasks();
        task_router->Stop();

        std::cout << "MCP Tool execution completed." << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
