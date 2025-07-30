cmake_minimum_required(VERSION 3.16)
project(MCP_AsepriteTool VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard (compatible with Ubuntu 22.04 default gcc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Docker-compatible compile flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall -fPIC")

# Default build type for containers
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find required packages (available in Ubuntu 22.04)
find_package(Threads REQUIRED)

# Define target service (overridden in Dockerfiles)
if(NOT DEFINED TARGET_SERVICE)
    set(TARGET_SERVICE "all")
endif()

# Include directories
include_directories(src)
include_directories(src/core)
include_directories(src/tileset)
include_directories(src/map)

# Define common source files
set(CORE_SOURCES
    src/core/config_loader.cpp
)

# Define service-specific sources
set(SPRITE_TOOL_SOURCES
    ${CORE_SOURCES}
    src/tileset/sprite_tool.cpp
    src/main_sprite.cpp
)

set(TILE_CHECKER_SOURCES
    ${CORE_SOURCES}
    src/tileset/tile_checker.cpp
    src/main_tile.cpp
)

set(MAP_MAKER_SOURCES
    ${CORE_SOURCES}
    src/map/map_maker.cpp
    src/main_map.cpp
)

set(PREVIEW_TOOL_SOURCES
    ${CORE_SOURCES}
    src/main_preview.cpp
)

# Create executables based on target service
if(TARGET_SERVICE STREQUAL "sprite-tool" OR TARGET_SERVICE STREQUAL "all")
    add_executable(sprite_tool ${SPRITE_TOOL_SOURCES})
    target_link_libraries(sprite_tool ${CMAKE_THREAD_LIBS_INIT})
    target_compile_definitions(sprite_tool PRIVATE SERVICE_NAME="sprite-tool")
endif()

if(TARGET_SERVICE STREQUAL "tile-checker" OR TARGET_SERVICE STREQUAL "all")
    add_executable(tile_checker ${TILE_CHECKER_SOURCES})
    target_link_libraries(tile_checker ${CMAKE_THREAD_LIBS_INIT})
    target_compile_definitions(tile_checker PRIVATE SERVICE_NAME="tile-checker")
endif()

if(TARGET_SERVICE STREQUAL "map-maker" OR TARGET_SERVICE STREQUAL "all")
    add_executable(map_maker ${MAP_MAKER_SOURCES})
    target_link_libraries(map_maker ${CMAKE_THREAD_LIBS_INIT})
    target_compile_definitions(map_maker PRIVATE SERVICE_NAME="map-maker")
endif()

if(TARGET_SERVICE STREQUAL "preview-tool" OR TARGET_SERVICE STREQUAL "all")
    add_executable(preview_tool ${PREVIEW_TOOL_SOURCES})
    target_link_libraries(preview_tool ${CMAKE_THREAD_LIBS_INIT})
    target_compile_definitions(preview_tool PRIVATE SERVICE_NAME="preview-tool")
endif()

# Container-friendly install targets
if(TARGET_SERVICE STREQUAL "sprite-tool" OR TARGET_SERVICE STREQUAL "all")
    install(TARGETS sprite_tool DESTINATION /usr/local/bin)
endif()

if(TARGET_SERVICE STREQUAL "tile-checker" OR TARGET_SERVICE STREQUAL "all")
    install(TARGETS tile_checker DESTINATION /usr/local/bin)
endif()

if(TARGET_SERVICE STREQUAL "map-maker" OR TARGET_SERVICE STREQUAL "all")
    install(TARGETS map_maker DESTINATION /usr/local/bin)
endif()

if(TARGET_SERVICE STREQUAL "preview-tool" OR TARGET_SERVICE STREQUAL "all")
    install(TARGETS preview_tool DESTINATION /usr/local/bin)
endif()

# Print build information for Docker debugging
message(STATUS "=== Docker Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Target service: ${TARGET_SERVICE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=================================")
