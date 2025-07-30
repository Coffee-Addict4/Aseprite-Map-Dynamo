#include "sprite_tool.h"
#include <iostream>

SpriteTool::SpriteTool() {
    std::cout << "SpriteTool initialized" << std::endl;
}

SpriteTool::~SpriteTool() = default;

bool SpriteTool::ExportSprite(const std::string& /* inputPath */, const std::string& /* outputPath */) {
    // Stub implementation
    return true;
}

std::vector<std::string> SpriteTool::BatchExportSprites(const std::vector<std::string>& /* inputPaths */, const std::string& /* outputDir */) {
    // Stub implementation
    return {};
}

bool SpriteTool::ValidateSprite(const std::string& /* spritePath */) {
    // Stub implementation
    return true;
}
