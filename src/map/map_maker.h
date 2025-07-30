#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>

namespace mcp_tool {
namespace map {

struct LayerDefinition {
    std::string name;
    std::string type; // "tile", "object", "image"
    int z_order;
    bool visible;
    double opacity;
    std::map<std::string, std::string> properties;
    std::string tileset_reference;
};

struct GameplayLogic {
    std::string logic_type; // "collision", "trigger", "spawn", "interactive"
    std::map<std::string, std::string> parameters;
    std::vector<std::pair<int, int>> affected_tiles; // tile coordinates
    std::string script_reference;
};

struct TerrainDefinition {
    std::string terrain_type;
    std::vector<int> tile_ids;
    std::map<std::string, double> properties; // "movement_cost", "height", etc.
    std::vector<std::string> compatible_terrains;
};

struct MapMetadata {
    std::string map_name;
    int width;
    int height;
    int tile_width;
    int tile_height;
    std::string orientation; // "orthogonal", "isometric", "staggered"
    std::vector<LayerDefinition> layers;
    std::vector<TerrainDefinition> terrains;
    std::vector<GameplayLogic> gameplay_elements;
    std::string tool_signature;
    std::string version;
    std::map<std::string, std::string> transformation_deltas;
    std::chrono::system_clock::time_point created_at;
};

class MapMaker {
public:
    MapMaker();
    ~MapMaker() = default;

    // Map creation
    bool CreateMap(const std::string& tileset_path, const MapMetadata& metadata, 
                  const std::string& output_path);
    bool CreateLayeredMap(const std::vector<std::string>& tileset_paths, 
                         const MapMetadata& metadata, const std::string& output_path);

    // Terrain and gameplay logic integration
    bool AddTerrainLayer(MapMetadata& metadata, const TerrainDefinition& terrain);
    bool AddGameplayLogic(MapMetadata& metadata, const GameplayLogic& logic);
    bool ValidateGameplayLogic(const MapMetadata& metadata);

    // Layer management
    bool AddLayer(MapMetadata& metadata, const LayerDefinition& layer);
    bool RemoveLayer(MapMetadata& metadata, const std::string& layer_name);
    bool ReorderLayers(MapMetadata& metadata, const std::vector<std::string>& layer_order);

    // Semantic tagging
    bool AddSemanticTags(MapMetadata& metadata, const std::map<std::string, std::string>& tags);
    bool ValidateSemanticConsistency(const MapMetadata& metadata);

    // Export formats
    bool ExportToTiledJson(const MapMetadata& metadata, const std::string& output_path);
    bool ExportToTiledTmx(const MapMetadata& metadata, const std::string& output_path);
    bool ExportToCustomFormat(const MapMetadata& metadata, const std::string& format, 
                             const std::string& output_path);

    // Transformation tracking and rollback
    void StartTransaction(const std::string& transaction_id);
    void CommitTransaction(const std::string& transaction_id);
    void RollbackTransaction(const std::string& transaction_id);
    bool CanRollback(const std::string& transaction_id) const;

    // Tool signatures and validation
    bool EmbedToolSignature(MapMetadata& metadata);
    bool ValidateToolSignatures(const MapMetadata& metadata);
    bool EmbedDataTypes(MapMetadata& metadata);

    // Audit and compliance
    void SetSharedVolumePath(const std::string& path);
    std::vector<std::string> GetTransformationLog() const;

private:
    std::string shared_volume_path_;
    std::string tool_signature_;
    std::string tool_version_;
    std::map<std::string, MapMetadata> transaction_snapshots_;
    std::vector<std::string> transformation_log_;

    // Map generation helpers
    bool GenerateTileData(const MapMetadata& metadata, std::vector<std::vector<int>>& tile_data);
    bool ApplyTerrainRules(const std::vector<TerrainDefinition>& terrains, 
                          std::vector<std::vector<int>>& tile_data);
    bool IntegrateGameplayElements(const std::vector<GameplayLogic>& elements, 
                                  MapMetadata& metadata);

    // Export format helpers
    std::string GenerateJsonMapData(const MapMetadata& metadata);
    std::string GenerateTmxMapData(const MapMetadata& metadata);
    bool WriteJsonFile(const std::string& json_data, const std::string& output_path);
    bool WriteTmxFile(const std::string& tmx_data, const std::string& output_path);

    // Validation helpers
    bool ValidateLayerConsistency(const std::vector<LayerDefinition>& layers);
    bool ValidateTerrainCompatibility(const std::vector<TerrainDefinition>& terrains);
    bool ValidateTilesetReferences(const std::vector<LayerDefinition>& layers, 
                                  const std::vector<std::string>& available_tilesets);

    // Transformation tracking
    void RecordTransformation(const std::string& operation, const std::string& before_state, 
                             const std::string& after_state, const std::string& reason);
    std::string SerializeMetadata(const MapMetadata& metadata) const;
    MapMetadata DeserializeMetadata(const std::string& serialized_data) const;

    // Utility functions
    std::string GenerateToolSignature();
    std::string CalculateMetadataHash(const MapMetadata& metadata) const;
    void WriteAuditLog(const std::string& entry);
};

} // namespace map
} // namespace mcp_tool
