// Debug path validation
const illegalChars = /[<>:"|?*\x00-\x1f]/;
const testPath = "C:\\Users\\jadon\\Downloads";

console.log("Testing path:", testPath);
console.log("Contains illegal chars:", illegalChars.test(testPath));

// Check each character
for (let i = 0; i < testPath.length; i++) {
    const char = testPath[i];
    const charCode = char.charCodeAt(0);
    if (illegalChars.test(char)) {
        console.log(`Illegal character found at position ${i}: '${char}' (code: ${charCode})`);
    }
}

console.log("Breakdown:");
console.log("< test:", /[<]/.test(testPath));
console.log("> test:", /[>]/.test(testPath));  
console.log(": test:", /[:]/.test(testPath));
console.log('"' + " test:", /["]/.test(testPath));
console.log("| test:", /[|]/.test(testPath));
console.log("? test:", /[?]/.test(testPath));
console.log("* test:", /[*]/.test(testPath));
console.log("control chars test:", /[\x00-\x1f]/.test(testPath));
