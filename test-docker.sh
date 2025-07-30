#!/bin/bash
# Test script for Docker Compose setup

echo "Testing MCP Aseprite Map Dynamo Docker setup..."

# Check if docker-compose is available
if ! command -v docker-compose &> /dev/null; then
    echo "Error: docker-compose not found"
    exit 1
fi

# Change to docker directory
cd docker

# Check compose file syntax
echo "Validating docker-compose.yml..."
docker-compose config > /dev/null
if [ $? -ne 0 ]; then
    echo "Error: docker-compose.yml has syntax errors"
    exit 1
fi

echo "docker-compose.yml syntax is valid"

# Build all containers
echo "Building containers..."
docker-compose build
if [ $? -ne 0 ]; then
    echo "Error: Failed to build containers"
    exit 1
fi

echo "All containers built successfully"

# Start services
echo "Starting services..."
docker-compose up -d
if [ $? -ne 0 ]; then
    echo "Error: Failed to start services"
    exit 1
fi

echo "Services started successfully"

# Wait a bit for services to initialize
sleep 10

# Check service status
echo "Checking service status..."
docker-compose ps

# Check health status
echo "Checking health status..."
for service in sprite-tool tile-checker map-maker preview-tool; do
    status=$(docker inspect --format='{{.State.Health.Status}}' mcp-$service 2>/dev/null || echo "unknown")
    echo "$service health: $status"
done

echo "Test completed. Use 'docker-compose down' to stop services."
