@echo off
REM MCP Tool Build Script for Windows
REM This script builds the MCP Tool project with proper configuration

setlocal enabledelayedexpansion

echo MCP Tool Build Script
echo ====================

REM Function to print status messages
set "INFO_COLOR=92"
set "WARN_COLOR=93"
set "ERROR_COLOR=91"

REM Check prerequisites
echo [INFO] Checking prerequisites...

REM Check Docker (required for this build approach)
docker --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Docker is required but not installed
    echo Please install Docker Desktop and try again
    exit /b 1
) else (
    echo [INFO] Docker found, proceeding with container-based build
    set "DOCKER_AVAILABLE=true"
)

REM Skip local CMake/compiler checks - we'll build inside containers

echo [INFO] Prerequisites check completed

REM Create build directory
echo [INFO] Setting up build directory...

if exist "build" (
    echo [WARN] Build directory exists, cleaning...
    rmdir /s /q "build"
)

mkdir "build"
if not exist "assets" mkdir "assets"
if not exist "assets\input" mkdir "assets\input"
if not exist "assets\output" mkdir "assets\output"
if not exist "assets\logs" mkdir "assets\logs"

echo [INFO] Build directory setup completed

REM Build Docker images directly (skip local CMake build)
echo [INFO] Building MCP Tool Docker containers...

cd "docker"

REM Build all Docker images with docker-compose for Linux containers
echo [INFO] Building all containers using docker-compose...
docker-compose build --no-cache
if %errorlevel% neq 0 (
    echo [ERROR] Docker compose build failed
    cd ..
    exit /b 1
)

echo [INFO] All containers built successfully with docker-compose

REM Test containers
echo [INFO] Testing container health checks...
echo [INFO] Containers will be tested after they are started with docker-compose

cd ..

REM Start the MCP Tool stack
echo [INFO] Starting MCP Tool services...
cd "docker"
docker-compose up -d
if %errorlevel% neq 0 (
    echo [ERROR] Failed to start services
    cd ..
    exit /b 1
) else (
    echo [INFO] MCP Tool services started successfully
    echo [INFO] You can check status with: docker-compose ps
    echo [INFO] View logs with: docker-compose logs
    echo [INFO] Stop services with: docker-compose down
)

cd ..

echo [INFO] Build and deployment completed successfully!
echo [INFO] MCP Tool is now running in containerized mode

pause
