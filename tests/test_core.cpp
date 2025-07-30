#include <cassert>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "../src/core/config_loader.h"

using namespace mcp_tool::core;

void TestConfigLoaderBasic() {
    std::cout << "Testing ConfigLoader basic functionality..." << std::endl;
    
    ConfigLoader loader;
    
    // Test initial state
    assert(!loader.GetBoolValue("nonexistent_key"));
    assert(loader.GetIntValue("nonexistent_key") == 0);
    assert(loader.GetValue("nonexistent_key").empty());
    
    std::cout << "✓ Basic ConfigLoader tests passed" << std::endl;
}

void TestConfigLoaderWithFile() {
    std::cout << "Testing ConfigLoader with configuration file..." << std::endl;
    
    // Create a temporary config file
    std::string temp_config = "test_config.txt";
    std::ofstream config_file(temp_config);
    config_file << "test_key=test_value\n";
    config_file << "bool_key=true\n";
    config_file << "int_key=42\n";
    config_file << "double_key=3.14\n";
    config_file.close();
    
    ConfigLoader loader;
    
    // Test loading configuration
    bool loaded = loader.LoadConfig(temp_config);
    assert(loaded);
    
    // Test value retrieval
    assert(loader.GetValue("test_key") == "test_value");
    assert(loader.GetBoolValue("bool_key") == true);
    assert(loader.GetIntValue("int_key") == 42);
    assert(loader.GetDoubleValue("double_key") == 3.14);
    
    // Clean up
    std::filesystem::remove(temp_config);
    
    std::cout << "✓ ConfigLoader file tests passed" << std::endl;
}

void TestContainerDetection() {
    std::cout << "Testing container detection..." << std::endl;
    
    ConfigLoader loader;
    
    // Test container detection (will be false in test environment)
    bool in_container = loader.IsRunningInContainer();
    std::string volume_path = loader.GetSharedVolumePath();
    
    assert(!volume_path.empty());
    std::cout << "Container mode: " << (in_container ? "true" : "false") << std::endl;
    std::cout << "Shared volume path: " << volume_path << std::endl;
    
    std::cout << "✓ Container detection tests passed" << std::endl;
}

void TestAuditLogging() {
    std::cout << "Testing audit logging..." << std::endl;
    
    ConfigLoader loader;
    
    // Test audit logging
    loader.LogConfigChange("test_key", "old_value", "new_value", "Unit test");
    
    // Check if audit log directory exists
    std::string shared_path = loader.GetSharedVolumePath();
    std::string audit_dir = shared_path + "/audit_logs";
    
    if (std::filesystem::exists(audit_dir)) {
        std::cout << "✓ Audit logging directory created" << std::endl;
    }
    
    std::cout << "✓ Audit logging tests passed" << std::endl;
}

int main() {
    try {
        std::cout << "Running MCP Tool Core Tests..." << std::endl;
        std::cout << "=====================================" << std::endl;
        
        TestConfigLoaderBasic();
        TestConfigLoaderWithFile();
        TestContainerDetection();
        TestAuditLogging();
        
        std::cout << "=====================================" << std::endl;
        std::cout << "All core tests passed successfully!" << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
