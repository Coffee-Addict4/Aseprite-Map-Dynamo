@echo off
REM Test script for Docker Compose setup on Windows

echo Testing MCP Aseprite Map Dynamo Docker setup...

REM Check if Docker is running
docker version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: Docker is not running or not installed.
    echo Please start Docker Desktop and try again.
    exit /b 1
)

REM Change to docker directory
cd docker

REM Check compose file syntax
echo Validating docker-compose.yml...
docker-compose config >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: docker-compose.yml has syntax errors
    cd ..
    exit /b 1
)

echo docker-compose.yml syntax is valid

REM Build all containers
echo Building containers...
docker-compose build
if %ERRORLEVEL% neq 0 (
    echo Error: Failed to build containers
    cd ..
    exit /b 1
)

echo All containers built successfully

REM Start services
echo Starting services...
docker-compose up -d
if %ERRORLEVEL% neq 0 (
    echo Error: Failed to start services
    cd ..
    exit /b 1
)

echo Services started successfully

REM Wait a bit for services to initialize
echo Waiting for services to initialize...
timeout /t 10 /nobreak >nul

REM Check service status
echo Checking service status...
docker-compose ps

REM Check health status
echo Checking health status...
for %%s in (sprite-tool tile-checker map-maker preview-tool) do (
    for /f "tokens=*" %%i in ('docker inspect --format="{{.State.Health.Status}}" mcp-%%s 2^>nul') do (
        echo %%s health: %%i
    )
)

echo.
echo Test completed. Use 'docker-compose down' to stop services.
cd ..
pause
