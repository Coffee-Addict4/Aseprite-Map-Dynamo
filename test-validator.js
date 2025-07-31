// Test the SecurePathValidator independently
const { SecurePathValidator } = require('./mcp-server.js');

const validator = new SecurePathValidator();

// Test cases
const testPaths = [
    'C:\\Users\\jadon\\Downloads\\output.png',
    'C:\\Users\\jadon\\Downloads\\',
    'D:\\Projects\\maps\\tileset.png',
    '/home/user/downloads/map.png',
    'invalid:path<>',
    'C:\\CON\\file.txt',
    ''
];

testPaths.forEach(path => {
    console.log(`\n--- Testing: ${path} ---`);
    const result = validator.validatePath(path, 'windows');
    console.log('Valid:', result.isValid);
    if (!result.isValid) {
        console.log('Errors:', result.errors);
    }
    console.log('Components:', result.components);
});
