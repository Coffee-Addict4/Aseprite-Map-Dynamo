cmake_minimum_required(VERSION 3.16)
project(MCP-Tool VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(src)

# Core library
set(CORE_SOURCES
    src/core/config_loader.cpp
    src/core/task_router.cpp
    src/core/guideline_enforcer.cpp
)

add_library(mcp_core ${CORE_SOURCES})
target_link_libraries(mcp_core Threads::Threads)

# Tileset library
set(TILESET_SOURCES
    src/tileset/sprite_tool.cpp
    src/tileset/tile_checker.cpp
    src/tileset/packer_tool.cpp
    src/tileset/meta_editor.cpp
)

add_library(mcp_tileset ${TILESET_SOURCES})
target_link_libraries(mcp_tileset mcp_core)

# Map library
set(MAP_SOURCES
    src/map/map_tool.cpp
    src/map/zone_tool.cpp
    src/map/map_maker.cpp
    src/map/preview_tool.cpp
)

add_library(mcp_map ${MAP_SOURCES})
target_link_libraries(mcp_map mcp_core)

# Executable for sprite tool
add_executable(sprite_tool src/tileset/sprite_tool_main.cpp)
target_link_libraries(sprite_tool mcp_tileset)

# Executable for tile checker
add_executable(tile_checker src/tileset/tile_checker_main.cpp)
target_link_libraries(tile_checker mcp_tileset)

# Executable for map maker
add_executable(map_maker src/map/map_maker_main.cpp)
target_link_libraries(map_maker mcp_map)

# Executable for preview tool
add_executable(preview_tool src/map/preview_tool_main.cpp)
target_link_libraries(preview_tool mcp_map)

# Main MCP tool executable
add_executable(mcp_tool src/main.cpp)
target_link_libraries(mcp_tool mcp_core mcp_tileset mcp_map)

# Install targets
install(TARGETS sprite_tool tile_checker map_maker preview_tool mcp_tool
    RUNTIME DESTINATION bin
)

# Install configuration files
install(FILES 
    config/strict-guidelines.json
    config/adaptive-rules.json
    DESTINATION etc/mcp-tool
)

# Testing
enable_testing()

# Test executables
add_executable(test_core tests/test_core.cpp)
target_link_libraries(test_core mcp_core)
add_test(NAME CoreTests COMMAND test_core)

add_executable(test_tileset tests/test_tileset.cpp)
target_link_libraries(test_tileset mcp_tileset)
add_test(NAME TilesetTests COMMAND test_tileset)

add_executable(test_map tests/test_map.cpp)
target_link_libraries(test_map mcp_map)
add_test(NAME MapTests COMMAND test_map)

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Packaging
set(CPACK_PACKAGE_NAME "MCP-Tool")
set(CPACK_PACKAGE_VENDOR "Aseprite-Map-Dynamo")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MCP Tool for game development with Aseprite")

include(CPack)
