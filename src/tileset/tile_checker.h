#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../tileset/sprite_tool.h"

namespace mcp_tool {
namespace tileset {

enum class ValidationSeverity {
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

struct ValidationIssue {
    ValidationSeverity severity;
    std::string category;
    std::string description;
    std::string file_path;
    int frame_number;
    std::string suggested_fix;
    bool is_auto_fixable;
};

struct TileValidationRules {
    int expected_tile_width;
    int expected_tile_height;
    bool enforce_power_of_two;
    bool require_consistent_palette;
    bool validate_anchor_points;
    bool check_tag_naming_convention;
    std::vector<std::string> required_tags;
    std::vector<std::string> forbidden_tags;
    int max_frame_count;
    bool require_metadata_export;
};

class TileChecker {
public:
    TileChecker();
    ~TileChecker() = default;

    // Validation configuration
    void SetValidationRules(const TileValidationRules& rules);
    TileValidationRules GetValidationRules() const;

    // Core validation functions
    std::vector<ValidationIssue> ValidateTileset(const std::string& tileset_path);
    std::vector<ValidationIssue> ValidateIndividualTile(const std::string& tile_path);
    std::vector<ValidationIssue> ValidateTileCollection(const std::vector<std::string>& tile_paths);

    // Specific validation checks
    std::vector<ValidationIssue> CheckFrameSize(const std::string& sprite_path);
    std::vector<ValidationIssue> CheckTagIntegrity(const std::string& sprite_path);
    std::vector<ValidationIssue> CheckAnchorCoordinates(const std::string& sprite_path);
    std::vector<ValidationIssue> CheckPaletteConsistency(const std::string& sprite_path);
    std::vector<ValidationIssue> CheckNamingConventions(const std::string& sprite_path);

    // Batch validation
    bool ValidateAndReport(const std::vector<std::string>& sprite_paths, 
                          const std::string& report_output_path);
    
    // Issue reporting and analysis
    void GenerateValidationReport(const std::vector<ValidationIssue>& issues, 
                                 const std::string& output_path);
    bool HasCriticalIssues(const std::vector<ValidationIssue>& issues) const;
    std::vector<ValidationIssue> FilterIssuesBySeverity(const std::vector<ValidationIssue>& issues, 
                                                       ValidationSeverity min_severity) const;

    // Auto-fix capabilities (only for non-immutable violations)
    bool CanAutoFix(const ValidationIssue& issue) const;
    bool ApplyAutoFix(const ValidationIssue& issue);
    std::vector<ValidationIssue> GetAutoFixableIssues(const std::vector<ValidationIssue>& issues) const;

    // Audit and compliance
    void SetSharedVolumePath(const std::string& path);
    void LogValidationResults(const std::vector<ValidationIssue>& issues, const std::string& context);

private:
    TileValidationRules validation_rules_;
    std::string shared_volume_path_;
    std::vector<std::string> validation_log_;
    
    // Internal validation helpers
    bool CheckDimensions(const SpriteMetadata& metadata, std::vector<ValidationIssue>& issues);
    bool CheckPowerOfTwo(int dimension) const;
    bool ValidateTagNaming(const std::vector<std::string>& tags, std::vector<ValidationIssue>& issues);
    bool ValidateRequiredTags(const std::vector<std::string>& tags, std::vector<ValidationIssue>& issues);
    bool ValidatePaletteHash(const std::string& current_hash, const std::string& reference_hash, 
                            std::vector<ValidationIssue>& issues);

    // Issue creation helpers
    ValidationIssue CreateIssue(ValidationSeverity severity, const std::string& category, 
                               const std::string& description, const std::string& file_path,
                               int frame_number = -1, const std::string& suggested_fix = "",
                               bool is_auto_fixable = false);

    // Reporting helpers
    std::string SeverityToString(ValidationSeverity severity) const;
    std::string GenerateHtmlReport(const std::vector<ValidationIssue>& issues) const;
    std::string GenerateJsonReport(const std::vector<ValidationIssue>& issues) const;
    
    // Audit logging
    void WriteAuditLog(const std::string& entry);
    void LogIssue(const ValidationIssue& issue);
};

} // namespace tileset
} // namespace mcp_tool
