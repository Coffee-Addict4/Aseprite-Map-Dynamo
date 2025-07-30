#include "core/config_loader.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace mcp_tool::core;

int main(int argc, char* argv[]) {
    try {
        // Initialize configuration
        auto config_loader = std::make_unique<ConfigLoader>();
        
        // Handle health check
        if (argc > 1 && std::string(argv[1]) == "--health-check") {
            std::cout << "preview_tool: healthy" << std::endl;
            return 0;
        }
        
        // Handle MCP server mode
        if (argc > 1 && std::string(argv[1]) == "--mcp-server") {
            std::cout << "{\"jsonrpc\":\"2.0\",\"method\":\"server/initialized\",\"params\":{}}" << std::endl;
            
            // MCP server main loop - read JSON-RPC from stdin, respond on stdout
            std::string line;
            while (std::getline(std::cin, line)) {
                if (line.find("initialize") != std::string::npos) {
                    std::cout << "{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{\"capabilities\":{\"tools\":{}}," 
                              << "\"serverInfo\":{\"name\":\"aseprite-map-dynamo\",\"version\":\"1.0.0\"}}}" << std::endl;
                } else if (line.find("tools/list") != std::string::npos) {
                    std::cout << "{\"jsonrpc\":\"2.0\",\"id\":2,\"result\":{\"tools\":[" 
                              << "{\"name\":\"health_check\",\"description\":\"Check service health\"}," 
                              << "{\"name\":\"create_map\",\"description\":\"Generate a new map\"}," 
                              << "{\"name\":\"preview_map\",\"description\":\"Preview generated map\"}" 
                              << "]}}" << std::endl;
                }
                std::cout.flush();
            }
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
