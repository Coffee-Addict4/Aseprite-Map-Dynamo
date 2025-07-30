#!/bin/bash

# MCP Tool Docker Build Script
# This script builds and tests Docker images for the MCP Tool microservices

set -e  # Exit on any error

echo "MCP Tool Docker Build Script"
echo "============================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_step() {
    echo -e "${BLUE}[STEP]${NC} $1"
}

# Check prerequisites
check_prerequisites() {
    print_step "Checking prerequisites..."
    
    # Check Docker
    if ! command -v docker &> /dev/null; then
        print_error "Docker is required but not installed"
        exit 1
    fi
    
    # Check Docker Compose
    if ! command -v docker-compose &> /dev/null; then
        print_error "Docker Compose is required but not installed"
        exit 1
    fi
    
    # Check if Docker daemon is running
    if ! docker info &> /dev/null; then
        print_error "Docker daemon is not running"
        exit 1
    fi
    
    print_info "Docker prerequisites satisfied"
}

# Create necessary directories and files
setup_environment() {
    print_step "Setting up build environment..."
    
    # Create shared assets directory
    mkdir -p assets/{input,output,logs,config}
    
    # Create placeholder binaries for Docker build
    mkdir -p docker/binaries
    
    # Create minimal placeholder binaries (will be replaced by actual builds later)
    create_placeholder_binary() {
        local binary_name=$1
        cat > "docker/binaries/${binary_name}" << 'EOF'
#!/bin/bash
echo "MCP Tool ${binary_name} - Placeholder Implementation"
echo "This is a minimal placeholder for Docker testing"

case "$1" in
    --health-check)
        echo "Health check: OK"
        exit 0
        ;;
    --daemon)
        echo "Running in daemon mode..."
        while true; do
            sleep 30
            echo "$(date): ${binary_name} daemon heartbeat"
        done
        ;;
    --help)
        echo "Usage: ${binary_name} [--health-check|--daemon|--help]"
        ;;
    *)
        echo "Processing with ${binary_name}..."
        echo "Input: $*"
        echo "Output: Success (placeholder)"
        ;;
esac
EOF
        chmod +x "docker/binaries/${binary_name}"
    }
    
    create_placeholder_binary "sprite_tool"
    create_placeholder_binary "tile_checker" 
    create_placeholder_binary "map_maker"
    create_placeholder_binary "preview_tool"
    
    print_info "Environment setup completed"
}

# Update Dockerfiles to use placeholder binaries
update_dockerfiles() {
    print_step "Updating Dockerfiles for testing..."
    
    # Update Dockerfile.sprite-tool
    cat > docker/Dockerfile.sprite-tool << 'EOF'
# MCP Tool - Sprite Processing Container
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    bash \
    coreutils \
    && rm -rf /var/lib/apt/lists/*

# Set up shared volume mount point
RUN mkdir -p /mnt/assets /mnt/assets/input /mnt/assets/output /mnt/assets/logs

# Copy sprite tool binary
COPY binaries/sprite_tool /usr/local/bin/sprite_tool
RUN chmod +x /usr/local/bin/sprite_tool

# Set environment variables
ENV MCP_CONTAINER_MODE=true
ENV SHARED_VOLUME_PATH=/mnt/assets

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD /usr/local/bin/sprite_tool --health-check || exit 1

# Set working directory
WORKDIR /app

# Default command
CMD ["/usr/local/bin/sprite_tool", "--daemon"]
EOF

    # Update Dockerfile.tile-checker
    cat > docker/Dockerfile.tile-checker << 'EOF'
# MCP Tool - Tile Validation Container
FROM alpine:3.18

# Install runtime dependencies
RUN apk add --no-cache bash

# Set up shared volume mount point
RUN mkdir -p /mnt/assets /mnt/assets/input /mnt/assets/output /mnt/assets/logs

# Copy tile checker binary
COPY binaries/tile_checker /usr/local/bin/tile_checker
RUN chmod +x /usr/local/bin/tile_checker

# Set environment variables
ENV MCP_CONTAINER_MODE=true
ENV SHARED_VOLUME_PATH=/mnt/assets

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD /usr/local/bin/tile_checker --health-check || exit 1

# Set working directory
WORKDIR /app

# Default command
CMD ["/usr/local/bin/tile_checker", "--daemon"]
EOF

    # Update Dockerfile.map-maker
    cat > docker/Dockerfile.map-maker << 'EOF'
# MCP Tool - Map Generation Container
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    bash \
    coreutils \
    && rm -rf /var/lib/apt/lists/*

# Set up shared volume mount point
RUN mkdir -p /mnt/assets /mnt/assets/input /mnt/assets/output /mnt/assets/logs

# Copy map maker binary
COPY binaries/map_maker /usr/local/bin/map_maker
RUN chmod +x /usr/local/bin/map_maker

# Set environment variables
ENV MCP_CONTAINER_MODE=true
ENV SHARED_VOLUME_PATH=/mnt/assets

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD /usr/local/bin/map_maker --health-check || exit 1

# Set working directory
WORKDIR /app

# Default command
CMD ["/usr/local/bin/map_maker", "--daemon"]
EOF

    # Create preview tool Dockerfile
    cat > docker/Dockerfile.preview-tool << 'EOF'
# MCP Tool - Preview Generation Container
FROM ubuntu:22.04

# Install runtime dependencies including ImageMagick
RUN apt-get update && apt-get install -y \
    bash \
    coreutils \
    imagemagick \
    && rm -rf /var/lib/apt/lists/*

# Set up shared volume mount point
RUN mkdir -p /mnt/assets /mnt/assets/input /mnt/assets/output /mnt/assets/logs

# Copy preview tool binary
COPY binaries/preview_tool /usr/local/bin/preview_tool
RUN chmod +x /usr/local/bin/preview_tool

# Set environment variables
ENV MCP_CONTAINER_MODE=true
ENV SHARED_VOLUME_PATH=/mnt/assets

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD /usr/local/bin/preview_tool --health-check || exit 1

# Set working directory
WORKDIR /app

# Default command
CMD ["/usr/local/bin/preview_tool", "--daemon"]
EOF

    print_info "Dockerfiles updated for testing"
}

# Build Docker images
build_images() {
    print_step "Building Docker images..."
    
    cd docker
    
    # Build individual services
    print_info "Building sprite-tool image..."
    docker build -f Dockerfile.sprite-tool -t mcp-sprite-tool:latest .
    
    print_info "Building tile-checker image..."
    docker build -f Dockerfile.tile-checker -t mcp-tile-checker:latest .
    
    print_info "Building map-maker image..."  
    docker build -f Dockerfile.map-maker -t mcp-map-maker:latest .
    
    print_info "Building preview-tool image..."
    docker build -f Dockerfile.preview-tool -t mcp-preview-tool:latest .
    
    cd ..
    
    print_info "All Docker images built successfully"
}

# Test container health
test_containers() {
    print_step "Testing container health..."
    
    cd docker
    
    # Start services
    print_info "Starting services with docker-compose..."
    docker-compose up -d
    
    # Wait for containers to start
    sleep 10
    
    # Check health status
    print_info "Checking container health..."
    docker-compose ps
    
    # Test health endpoints
    for service in sprite-tool tile-checker map-maker preview-tool; do
        print_info "Testing $service health check..."
        docker-compose exec -T mcp-$service /usr/local/bin/$service --health-check
    done
    
    # Show logs
    print_info "Recent container logs:"
    docker-compose logs --tail=5
    
    cd ..
    
    print_info "Container health tests completed"
}

# Clean up function
cleanup() {
    print_step "Cleaning up..."
    
    cd docker 2>/dev/null || true
    docker-compose down 2>/dev/null || true
    cd .. 2>/dev/null || true
    
    print_info "Cleanup completed"
}

# Main execution
main() {
    print_info "Starting MCP Tool Docker build process..."
    
    # Set up cleanup trap
    trap cleanup EXIT
    
    check_prerequisites
    setup_environment
    update_dockerfiles
    build_images
    test_containers
    
    print_info "✅ Docker build and test completed successfully!"
    print_info "🐳 All microservices are running and healthy"
    print_info ""
    print_info "Next steps:"
    print_info "1. View running containers: docker-compose ps"
    print_info "2. View logs: docker-compose logs [service-name]"
    print_info "3. Stop services: docker-compose down"
    print_info "4. Replace placeholder binaries with real implementations"
}

# Parse command line arguments
CLEANUP_ONLY=false
NO_TEST=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --cleanup-only)
            CLEANUP_ONLY=true
            shift
            ;;
        --no-test)
            NO_TEST=true
            shift
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --cleanup-only    Only clean up running containers"
            echo "  --no-test        Skip container testing"
            echo "  --help           Show this help message"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Execute based on options
if [ "$CLEANUP_ONLY" = true ]; then
    cleanup
else
    main
fi
