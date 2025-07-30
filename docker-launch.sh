#!/bin/bash

# MCP Tool Docker Launch Script
# Similar approach to other MCP projects

echo "MCP Tool Docker Launch"
echo "====================="

# Check if Docker is running
if ! docker info >/dev/null 2>&1; then
    echo "❌ Docker is not running. Please start Docker first."
    exit 1
fi

echo "✅ Docker is running"

# Build containers
echo "🔨 Building MCP Tool containers..."
cd docker

# Build each service
docker build -f Dockerfile.sprite-tool -t mcp-aseprite/sprite-tool:latest . || exit 1
docker build -f Dockerfile.tile-checker -t mcp-aseprite/tile-checker:latest . || exit 1
docker build -f Dockerfile.map-maker -t mcp-aseprite/map-maker:latest . || exit 1

echo "✅ All containers built successfully"

# Start services
echo "🚀 Starting MCP Tool services..."
docker-compose up -d

if [ $? -eq 0 ]; then
    echo "✅ MCP Tool services started successfully"
    echo ""
    echo "📊 Service Status:"
    docker-compose ps
    echo ""
    echo "🔗 Useful Commands:"
    echo "  View logs:     docker-compose logs -f"
    echo "  Stop services: docker-compose down"
    echo "  Restart:       docker-compose restart"
    echo "  Shell access:  docker-compose exec sprite-tool bash"
else
    echo "❌ Failed to start services"
    exit 1
fi
