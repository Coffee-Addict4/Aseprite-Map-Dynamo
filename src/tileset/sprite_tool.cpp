#include "sprite_tool.h"
#include <iostream>

using namespace mcp_tool::tileset;

SpriteTool::SpriteTool() {
    std::cout << "SpriteTool initialized" << std::endl;
}

bool SpriteTool::ExportSprite(const std::string& /* input_path */, const std::string& /* output_path */, 
                             const ExportSettings& /* settings */) {
    // Stub implementation
    return true;
}

bool SpriteTool::BatchExportSprites(const std::vector<std::string>& /* input_paths */, 
                                   const std::string& /* output_dir */, 
                                   const ExportSettings& /* settings */) {
    // Stub implementation
    return true;
}

bool SpriteTool::ValidateSprite(const std::string& /* sprite_path */) {
    // Stub implementation
    return true;
}
