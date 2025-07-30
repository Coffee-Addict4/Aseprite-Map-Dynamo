#include "config_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <chrono>
#include <iomanip>

namespace mcp_tool {
namespace core {

ConfigLoader::ConfigLoader() 
    : is_loaded_(false), is_validated_(false) {
    // Detect if running in container and set shared volume path
    const char* container_env = std::getenv("MCP_CONTAINER_MODE");
    if (container_env && std::string(container_env) == "true") {
        shared_volume_path_ = "/mnt/assets";
    } else {
        shared_volume_path_ = "./assets"; // Local development fallback
    }
}

bool ConfigLoader::LoadConfig(const std::string& config_path) {
    try {
        std::ifstream file(config_path);
        if (!file.is_open()) {
            throw ConfigLoaderException("Unable to open config file: " + config_path);
        }

        std::string line;
        while (std::getline(file, line)) {
            // Simple key=value parsing (can be extended for YAML)
            if (line.empty() || line[0] == '#') continue;
            
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                config_values_[key] = value;
            }
        }

        is_loaded_ = true;
        LogConfigChange("system", "unloaded", "loaded", "Initial configuration load");
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Config loading error: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigLoader::LoadStrictGuidelines(const std::string& guidelines_path) {
    // Similar implementation for JSON parsing of strict guidelines
    // For now, simplified to demonstrate structure
    strict_guidelines_["modularity.enforce_single_responsibility"] = "true";
    strict_guidelines_["logging.require_audit_trails"] = "true";
    strict_guidelines_["versioning.require_transformation_deltas"] = "true";
    
    LogConfigChange("strict_guidelines", "unloaded", "loaded", "Strict guidelines loaded");
    return true;
}

bool ConfigLoader::LoadAdaptiveRules(const std::string& adaptive_rules_path) {
    // Similar implementation for adaptive rules
    adaptive_rules_["enhancement_suggestions.auto_tagging_threshold"] = "0.85";
    adaptive_rules_["model_feedback.suggestion_flagging"] = "true";
    
    LogConfigChange("adaptive_rules", "unloaded", "loaded", "Adaptive rules loaded");
    return true;
}

bool ConfigLoader::ValidateConfig() const {
    if (!is_loaded_) {
        std::cerr << "Configuration not loaded" << std::endl;
        return false;
    }

    // Validate against strict guidelines
    for (const auto& [rule, expected_value] : strict_guidelines_) {
        if (!ValidateStrictRule(rule, expected_value)) {
            std::cerr << "Strict rule validation failed: " << rule << std::endl;
            return false;
        }
    }

    return true;
}

bool ConfigLoader::ValidateAgainstGuidelines() const {
    return ValidateConfig();
}

std::string ConfigLoader::GetValue(const std::string& key) const {
    auto it = config_values_.find(key);
    return (it != config_values_.end()) ? it->second : "";
}

bool ConfigLoader::GetBoolValue(const std::string& key) const {
    std::string value = GetValue(key);
    return (value == "true" || value == "1" || value == "yes");
}

int ConfigLoader::GetIntValue(const std::string& key) const {
    std::string value = GetValue(key);
    try {
        return std::stoi(value);
    } catch (...) {
        return 0;
    }
}

double ConfigLoader::GetDoubleValue(const std::string& key) const {
    std::string value = GetValue(key);
    try {
        return std::stod(value);
    } catch (...) {
        return 0.0;
    }
}

bool ConfigLoader::IsRunningInContainer() const {
    const char* container_env = std::getenv("MCP_CONTAINER_MODE");
    return container_env && std::string(container_env) == "true";
}

std::string ConfigLoader::GetSharedVolumePath() const {
    return shared_volume_path_;
}

void ConfigLoader::LogConfigChange(const std::string& key, const std::string& old_value, 
                                  const std::string& new_value, const std::string& reason) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream log_entry;
    log_entry << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] "
              << "CONFIG_CHANGE: key=" << key 
              << ", old_value=" << old_value 
              << ", new_value=" << new_value 
              << ", reason=" << reason;
    
    WriteAuditLog(log_entry.str());
}

bool ConfigLoader::ValidateStrictRule(const std::string& rule, const std::string& value) const {
    // Implement strict rule validation logic
    // This would check against immutable rules that cannot be overridden
    return true; // Simplified for now
}

bool ConfigLoader::ValidateAdaptiveRule(const std::string& rule, const std::string& value) const {
    // Implement adaptive rule validation logic
    // These rules can be suggested for change by models but require approval
    return true; // Simplified for now
}

void ConfigLoader::WriteAuditLog(const std::string& entry) const {
    std::string log_path = shared_volume_path_ + "/audit_logs/config_changes.log";
    
    // Ensure directory exists
    std::filesystem::create_directories(shared_volume_path_ + "/audit_logs");
    
    std::ofstream log_file(log_path, std::ios::app);
    if (log_file.is_open()) {
        log_file << entry << std::endl;
        log_file.close();
    }
}

} // namespace core
} // namespace mcp_tool
