# MCP Tool Docker Launch Script for Windows
# Similar approach to other MCP projects

Write-Host "MCP Tool Docker Launch" -ForegroundColor Green
Write-Host "=====================" -ForegroundColor Green

# Check if Docker is running
try {
    docker info | Out-Null
    Write-Host "✅ Docker is running" -ForegroundColor Green
}
catch {
    Write-Host "❌ Docker is not running. Please start Docker first." -ForegroundColor Red
    exit 1
}

# Build containers
Write-Host "🔨 Building MCP Tool containers..." -ForegroundColor Yellow
Set-Location docker

# Build each service
Write-Host "Building sprite-tool..." -ForegroundColor Cyan
docker build -f Dockerfile.sprite-tool -t mcp-aseprite/sprite-tool:latest .
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "Building tile-checker..." -ForegroundColor Cyan
docker build -f Dockerfile.tile-checker -t mcp-aseprite/tile-checker:latest .
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "Building map-maker..." -ForegroundColor Cyan
docker build -f Dockerfile.map-maker -t mcp-aseprite/map-maker:latest .
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "✅ All containers built successfully" -ForegroundColor Green

# Start services
Write-Host "🚀 Starting MCP Tool services..." -ForegroundColor Yellow
docker-compose up -d

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ MCP Tool services started successfully" -ForegroundColor Green
    Write-Host ""
    Write-Host "📊 Service Status:" -ForegroundColor Cyan
    docker-compose ps
    Write-Host ""
    Write-Host "🔗 Useful Commands:" -ForegroundColor Cyan
    Write-Host "  View logs:     docker-compose logs -f"
    Write-Host "  Stop services: docker-compose down"
    Write-Host "  Restart:       docker-compose restart"
    Write-Host "  Shell access:  docker-compose exec sprite-tool bash"
} else {
    Write-Host "❌ Failed to start services" -ForegroundColor Red
    exit 1
}
