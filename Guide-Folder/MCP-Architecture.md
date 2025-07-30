# MCP Tool Architecture Plan (C++ Implementation)

## Prerequisites
- Review and fetch all Aseprite documentation links listed in Aseprite-Docs.yaml.
- Adhere to project guidelines (see Guidelines file) for automation control, API integration, Docker usage, and C++ best practices.

## Objectives
- Create a tile set compatible with Aseprite, ensuring all assets are tagged, anchored, and structured for model and tool consumption.
- Generate top-down maps that are not only visually correct but also semantically rich, supporting gameplay logic, rollback, and validation.

## Core C++ Modules
- config_loader: Reads YAML or plain text config files for pipeline and asset configuration.
- task_router: Orchestrates pipeline calls, manages thread execution, and ensures thread safety using std::thread or a lightweight dispatcher.
- guideline_enforcer: Validates all pipeline steps against immutable and adaptive guidelines (see Guidelines file and strict-guidelines.json).

## Tileset Pipeline
- sprite_tool: Wraps Aseprite CLI via system call or subprocess to export frames, tags, and metadata. Ensures .json metadata is always exported alongside .png.
- tile_checker: Verifies frame size, tag integrity, anchor coordinates, and palette consistency. Flags issues for review, never auto-corrects immutable violations.
- packer_tool (optional): Uses TexturePacker or similar "Safe tools" to pack tiles into atlases if required, maintaining audit logs for all transformations.
- meta_editor: Defines tag rules, grid boundaries, and semantic labels; outputs JSON or TXT metadata for downstream use and model validation.

## Map Pipeline
- map_tool: Reads and parses Tiled export formats (.json or .tmx) for map structure. Validates map data against guidelines and metadata.
- zone_tool: Handles LDtk-style procedural structure and zone logic (if enabled), ensuring procedural content respects design rules and is auditable.
- map_maker: Builds layered maps with terrain, gameplay logic, and semantic tags. Embeds tool signatures, data types, and transformation deltas for rollback and validation.
- preview_tool: Stitches images for preview using native image libraries or system calls to ImageMagick. Generates visual and metadata-based previews for model and human review.

## Integration Notes
- Use the C++ standard library and STL-first approach for all modules.
- Ensure thread safety for all modules that perform concurrent operations.
- Minimize external dependencies; prefer system calls for non-native tools (Aseprite CLI, TexturePacker, ImageMagick).
- All outputs (PNG, TXT/YAML, JSON) must include tool signatures, versioning, and audit logs for traceability.
- Model feedback loops: All model-suggested changes are flagged, reviewed, and tracked using semantic diffs. No core architecture or immutable rule is overwritten without explicit approval.


## Aseprite API Overview

Aseprite provides a powerful scripting API (Lua-based) and a command-line interface (CLI) for automation and integration. Key features and components include:

### Scripting API (Lua)
- **Access to Sprites:** Manipulate sprites, layers, frames, cels, palettes, and tags programmatically.
- **Automation:** Batch process assets, extract metadata, and perform custom operations.
- **Dialogs:** Create custom dialogs for user input.
- **Plugin Support:** Extend Aseprite with custom scripts and plugins.

#### Main Namespaces & Objects
- `app`: Main entry point for accessing the current sprite, files, commands, clipboard, and OS functions.
- `Sprite`, `Layer`, `Frame`, `Cel`, `Tag`, `Palette`, `Slice`, `Tilemap`: Core objects for asset manipulation.
- `Dialog`: For building custom UI dialogs.
- `json`: For reading/writing JSON metadata.

#### Example Operations
- Open, create, and save sprites.
- Export frames, tags, and slices as images or metadata.
- Modify pixel data, palettes, and animation timing.
- Integrate with external tools via system calls.

### Command-Line Interface (CLI)
- Automate exports, batch processing, and format conversions.
- Example: `aseprite -b file.ase --sheet output.png --data output.json`

### References
- [Aseprite API Documentation](https://www.aseprite.org/api/)
- [Aseprite Scripting Guide](https://www.aseprite.org/docs/scripting/)
- [Aseprite CLI Guide](https://www.aseprite.org/docs/cli/)

## References
- See prerequisites in Aseprite-Docs.yaml for Aseprite documentation and API links.
- See Guidelines file for protected rules and best practices.
- All architecture and pipeline changes must be reviewed against the latest guidelines and audit logs.
