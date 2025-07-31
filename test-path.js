const { spawn } = require('child_process');

function testPathValidation() {
    console.log("Testing Path Validation...");
    
    const server = spawn('node', ['mcp-server.js'], {
        cwd: 'c:\\GitHub\\Aseprite-Map-Dynamo'
    });
    
    const testCommand = {
        jsonrpc: "2.0",
        id: 1,
        method: "tools/call",
        params: {
            name: "validate_output_path",
            arguments: {
                path: "C:\\Users\\jadon\\Downloads",
                platform: "windows"
            }
        }
    };
    
    server.stdout.on('data', (data) => {
        console.log('Path Validation Response:', data.toString());
    });
    
    server.stderr.on('data', (data) => {
        console.error('Error:', data.toString());
    });
    
    setTimeout(() => {
        server.stdin.write(JSON.stringify(testCommand) + '\n');
    }, 1000);
    
    setTimeout(() => {
        server.kill();
    }, 5000);
}

testPathValidation();
