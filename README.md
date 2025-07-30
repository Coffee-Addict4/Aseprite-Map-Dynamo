# MCP Tool for Aseprite Map Generation

This project implements an MCP (Model Context Protocol) tool for automating top-down map generation using Aseprite, following strict guidelines for modularity, logging, and Docker-based microservices.

## Features

- **Tileset Pipeline**: Automated processing of Aseprite sprites into game-ready tilesets
- **Map Generation**: Creates layered maps with terrain and gameplay logic
- **Validation**: Comprehensive tile and map validation with audit trails
- **Docker Support**: Microservice architecture with shared volume support
- **Guideline Compliance**: Enforces immutable rules while allowing adaptive model enhancements

## Architecture

### Core Modules
- `config_loader`: Configuration and guideline management
- `task_router`: Thread-safe pipeline orchestration and Docker container management
- `guideline_enforcer`: Validation against immutable and adaptive rules

### Tileset Pipeline
- `sprite_tool`: Aseprite CLI wrapper with metadata export
- `tile_checker`: Frame size, tag, and palette validation
- `packer_tool`: Optional texture packing with audit logs
- `meta_editor`: Tag rules and grid boundary definition

### Map Pipeline
- `map_tool`: Tiled format parsing and validation
- `zone_tool`: LDtk-style procedural generation
- `map_maker`: Layered map creation with gameplay logic
- `preview_tool`: Image stitching and preview generation

## Building

### Prerequisites
- CMake 3.16+
- C++17 compatible compiler
- Docker (for containerized deployment)
- Aseprite (for sprite processing)

### Local Build
```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Docker Build
```bash
cd docker
docker-compose build
docker-compose up -d
```

## Usage

### Command Line
```bash
# Process tileset from Aseprite files
./mcp_tool tileset input/sprites/ output/tileset/

# Generate map from tileset
./mcp_tool map tileset.json map_config.json output/map.json

# Validate sprites
./mcp_tool validate input/sprite.ase

# Run as daemon (for Docker)
./mcp_tool daemon
```

### Docker Orchestration
```bash
# Start all services
docker-compose up -d

# Check service health
docker-compose ps

# View logs
docker-compose logs sprite-tool
```

## Configuration

### Strict Guidelines (`config/strict-guidelines.json`)
Contains immutable rules that cannot be overridden by model suggestions:
- Modularity enforcement
- Audit trail requirements
- Versioning mandates
- Architecture protection

### Adaptive Rules (`config/adaptive-rules.json`)
Contains rules that can be adapted based on model feedback:
- Performance tuning parameters
- Enhancement suggestions
- Procedural generation limits

## File Structure

```
src/
├── core/              # Core modules
├── tileset/           # Tileset pipeline
├── map/               # Map pipeline
└── main.cpp           # Main application

docker/                # Docker configurations
├── Dockerfile.*       # Service-specific Dockerfiles
└── docker-compose.yml # Orchestration

config/                # Configuration files
├── strict-guidelines.json
└── adaptive-rules.json

tests/                 # Unit tests
└── test_*.cpp

Guide-Folder/          # Architecture documentation
├── Architecture
├── Guidelines
├── MCP-Architecture.md
└── MCP-Implementation-Plan.md
```

## Guidelines Compliance

This implementation adheres to the project guidelines:

### Model Proofing & Automation Control
- ✅ Separates immutable rules from adaptive logic
- ✅ Tracks changes using semantic diffs
- ✅ Requires explicit approval for core changes
- ✅ Flags model suggestions for review

### Aseprite API Integration
- ✅ Exports JSON metadata alongside PNG
- ✅ Wraps CLI calls with feedback scripts
- ✅ Pre-processes with model-friendly structure
- ✅ Provides fine-grain feature access

### Docker Usage
- ✅ Uses micro-containers for each pipeline stage
- ✅ Mounts shared /mnt/assets volume
- ✅ Generates audit logs per run
- ✅ Implements health checks

### C++ Implementation
- ✅ Uses manifest-driven asset loading
- ✅ Creates SpriteBatch/TextureStream classes
- ✅ Exposes hooks for model inference
- ✅ Documents memory/buffer behavior

### General Design Principles
- ✅ Plug-and-play module design
- ✅ Consistent metadata formats
- ✅ Embedded tool signatures
- ✅ Reversible transformations with rollback

## Testing

```bash
# Run all tests
make test

# Run specific test suites
./test_core
./test_tileset
./test_map
```

## Contributing

1. Follow the established guidelines in `Guide-Folder/Guidelines`
2. Ensure all changes are auditable and reversible
3. Add appropriate tests for new functionality
4. Update documentation as needed

## License

[Your license here]
