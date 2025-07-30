#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <exception>

namespace mcp_tool {
namespace core {

class ConfigLoaderException : public std::exception {
public:
    explicit ConfigLoaderException(const std::string& message) : message_(message) {}
    const char* what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};

class ConfigLoader {
public:
    ConfigLoader();
    ~ConfigLoader() = default;

    // Load configuration from YAML or plain text files
    bool LoadConfig(const std::string& config_path);
    bool LoadStrictGuidelines(const std::string& guidelines_path);
    bool LoadAdaptiveRules(const std::string& adaptive_rules_path);

    // Validate configuration against schema and guidelines
    bool ValidateConfig() const;
    bool ValidateAgainstGuidelines() const;

    // Configuration accessors
    std::string GetValue(const std::string& key) const;
    bool GetBoolValue(const std::string& key) const;
    int GetIntValue(const std::string& key) const;
    double GetDoubleValue(const std::string& key) const;

    // Container context support
    bool IsRunningInContainer() const;
    std::string GetSharedVolumePath() const;

    // Audit logging
    void LogConfigChange(const std::string& key, const std::string& old_value, 
                        const std::string& new_value, const std::string& reason);

private:
    std::unordered_map<std::string, std::string> config_values_;
    std::unordered_map<std::string, std::string> strict_guidelines_;
    std::unordered_map<std::string, std::string> adaptive_rules_;
    
    bool is_loaded_;
    bool is_validated_;
    std::string shared_volume_path_;

    // Internal validation helpers
    bool ValidateStrictRule(const std::string& rule, const std::string& value) const;
    bool ValidateAdaptiveRule(const std::string& rule, const std::string& value) const;
    void WriteAuditLog(const std::string& entry) const;
};

} // namespace core
} // namespace mcp_tool
