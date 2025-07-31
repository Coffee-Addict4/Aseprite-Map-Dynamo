const { spawn } = require('child_process');

function testMCPServer() {
    console.log("Testing MCP Server...");
    
    // Start the MCP server process
    const server = spawn('node', ['mcp-server.js'], {
        cwd: 'c:\\GitHub\\Aseprite-Map-Dynamo'
    });
    
    // Test dual-format map generation
    const testCommand = {
        jsonrpc: "2.0",
        id: 1,
        method: "tools/call",
        params: {
            name: "generate_topdown_map",
            arguments: {
                mapWidth: 24,
                mapHeight: 24,
                biome: "forest",
                outputDirectory: "C:\\Users\\jadon\\Downloads",
                baseFilename: "test_dual_map"
            }
        }
    };
    
    server.stdout.on('data', (data) => {
        console.log('MCP Response:', data.toString());
    });
    
    server.stderr.on('data', (data) => {
        console.error('MCP Error:', data.toString());
    });
    
    // Send the test command
    setTimeout(() => {
        server.stdin.write(JSON.stringify(testCommand) + '\n');
    }, 1000);
    
    // Cleanup after 10 seconds
    setTimeout(() => {
        server.kill();
        console.log("Test completed.");
    }, 10000);
}

testMCPServer();
