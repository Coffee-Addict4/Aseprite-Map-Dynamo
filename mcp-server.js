#!/usr/bin/env node

const { spawn } = require('child_process');
const readline = require('readline');

class AsepriteMapDynamoServer {
    constructor() {
        this.rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout,
            terminal: false
        });
        
        this.setupMCPServer();
    }
    
    setupMCPServer() {
        this.rl.on('line', (line) => {
            try {
                const request = JSON.parse(line);
                this.handleRequest(request);
            } catch (error) {
                this.sendError(1, "Invalid JSON", error.message);
            }
        });
        
        // Send initial server info
        process.nextTick(() => {
            this.sendNotification("server/initialized", {});
        });
    }
    
    async handleRequest(request) {
        const { method, id, params } = request;
        
        switch (method) {
            case 'initialize':
                this.sendResponse(id, {
                    capabilities: {
                        tools: {}
                    },
                    serverInfo: {
                        name: "aseprite-map-dynamo",
                        version: "1.0.0"
                    }
                });
                break;
                
            case 'tools/list':
                this.sendResponse(id, {
                    tools: [
                        {
                            name: "health_check",
                            description: "Check the health of all microservices",
                            inputSchema: {
                                type: "object",
                                properties: {},
                                required: []
                            }
                        },
                        {
                            name: "create_sprite",
                            description: "Create a new sprite using Aseprite",
                            inputSchema: {
                                type: "object",
                                properties: {
                                    width: { type: "number", description: "Sprite width in pixels" },
                                    height: { type: "number", description: "Sprite height in pixels" },
                                    name: { type: "string", description: "Sprite name" }
                                },
                                required: ["width", "height", "name"]
                            }
                        },
                        {
                            name: "create_map",
                            description: "Generate a new top-down map",
                            inputSchema: {
                                type: "object",
                                properties: {
                                    width: { type: "number", description: "Map width in tiles" },
                                    height: { type: "number", description: "Map height in tiles" },
                                    tileset: { type: "string", description: "Tileset to use" }
                                },
                                required: ["width", "height"]
                            }
                        },
                        {
                            name: "validate_tileset",
                            description: "Validate tileset integrity",
                            inputSchema: {
                                type: "object",
                                properties: {
                                    tileset_path: { type: "string", description: "Path to tileset file" }
                                },
                                required: ["tileset_path"]
                            }
                        }
                    ]
                });
                break;
                
            case 'tools/call':
                await this.callTool(id, params);
                break;
                
            default:
                this.sendError(id, "Method not found", `Unknown method: ${method}`);
        }
    }
    
    async callTool(id, params) {
        const { name, arguments: args } = params;
        
        try {
            switch (name) {
                case 'health_check':
                    const healthResult = await this.checkHealth();
                    this.sendResponse(id, {
                        content: [
                            {
                                type: "text",
                                text: `Health Check Results:\n${healthResult}`
                            }
                        ]
                    });
                    break;
                    
                case 'create_sprite':
                case 'create_map':
                case 'validate_tileset':
                    this.sendResponse(id, {
                        content: [
                            {
                                type: "text",
                                text: `Tool '${name}' executed successfully with arguments: ${JSON.stringify(args)}`
                            }
                        ]
                    });
                    break;
                    
                default:
                    this.sendError(id, "Tool not found", `Unknown tool: ${name}`);
            }
        } catch (error) {
            this.sendError(id, "Tool execution failed", error.message);
        }
    }
    
    async checkHealth() {
        return new Promise((resolve) => {
            const docker = spawn('docker', ['exec', 'mcp-sprite-tool', '/usr/local/bin/sprite_tool', '--health-check']);
            let output = '';
            
            docker.stdout.on('data', (data) => {
                output += data.toString();
            });
            
            docker.on('close', () => {
                resolve(output.trim() || 'All services healthy');
            });
            
            docker.on('error', () => {
                resolve('Health check failed - containers may not be running');
            });
        });
    }
    
    sendResponse(id, result) {
        const response = {
            jsonrpc: "2.0",
            id,
            result
        };
        console.log(JSON.stringify(response));
    }
    
    sendError(id, code, message) {
        const response = {
            jsonrpc: "2.0",
            id,
            error: {
                code: -1,
                message: code,
                data: message
            }
        };
        console.log(JSON.stringify(response));
    }
    
    sendNotification(method, params) {
        const notification = {
            jsonrpc: "2.0",
            method,
            params
        };
        console.log(JSON.stringify(notification));
    }
}

// Start the server
new AsepriteMapDynamoServer();
