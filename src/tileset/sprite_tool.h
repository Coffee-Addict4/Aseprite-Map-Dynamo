#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace mcp_tool {
namespace tileset {

struct SpriteMetadata {
    std::string filename;
    int width;
    int height;
    int frame_count;
    std::vector<std::string> tags;
    std::vector<std::string> layers;
    std::map<std::string, std::string> custom_properties;
    std::string palette_hash;
    std::string tool_signature;
    std::string version;
    std::chrono::system_clock::time_point created_at;
};

struct ExportSettings {
    bool export_json_metadata;
    bool export_individual_frames;
    bool export_sprite_sheet;
    std::string output_format; // "png", "jpg", etc.
    int scale_factor;
    bool preserve_palette;
    bool include_hidden_layers;
    std::string tag_filter; // Export only specific tags
};

class SpriteTool {
public:
    SpriteTool();
    ~SpriteTool() = default;

    // Aseprite CLI wrapping
    bool ExportSprite(const std::string& input_path, const std::string& output_path, 
                     const ExportSettings& settings);
    bool ExportFrames(const std::string& input_path, const std::string& output_dir, 
                     const ExportSettings& settings);
    bool ExportSpriteSheet(const std::string& input_path, const std::string& output_path, 
                          const ExportSettings& settings);
    
    // Metadata extraction
    SpriteMetadata ExtractMetadata(const std::string& sprite_path);
    bool ExportMetadataAsJson(const SpriteMetadata& metadata, const std::string& output_path);
    
    // Batch processing
    bool BatchExportSprites(const std::vector<std::string>& input_paths, 
                           const std::string& output_dir, 
                           const ExportSettings& settings);
    
    // Validation and integrity checks
    bool ValidateSprite(const std::string& sprite_path);
    bool ValidateFrameTiming(const std::string& sprite_path);
    bool ValidateTags(const std::string& sprite_path);
    
    // Audit and logging
    void SetSharedVolumePath(const std::string& path);
    std::vector<std::string> GetProcessingLog() const;

private:
    std::string shared_volume_path_;
    std::vector<std::string> processing_log_;
    std::string tool_signature_;
    std::string tool_version_;

    // Aseprite CLI helpers
    bool ExecuteAsepriteCommand(const std::vector<std::string>& args, std::string& output);
    std::vector<std::string> BuildExportCommand(const std::string& input_path, 
                                               const std::string& output_path, 
                                               const ExportSettings& settings);
    
    // Metadata parsing helpers
    bool ParseAsepriteInfo(const std::string& sprite_path, SpriteMetadata& metadata);
    bool ExtractTagsFromSprite(const std::string& sprite_path, std::vector<std::string>& tags);
    bool ExtractLayersFromSprite(const std::string& sprite_path, std::vector<std::string>& layers);
    
    // Validation helpers
    bool CheckFrameConsistency(const std::string& sprite_path);
    bool CheckPaletteConsistency(const std::string& sprite_path);
    bool CheckTagIntegrity(const std::string& sprite_path);
    
    // Logging and audit
    void LogOperation(const std::string& operation, const std::string& input_path, 
                     const std::string& output_path, bool success, const std::string& details = "");
    void WriteAuditLog(const std::string& entry);
    std::string GenerateToolSignature();
    std::string CalculatePaletteHash(const std::string& sprite_path);
};

} // namespace tileset
} // namespace mcp_tool
