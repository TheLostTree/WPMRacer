function generateProgmemWords(){
    const fs = require("fs")
    let list = fs.readFileSync("list.txt", "utf-8").split("\n");


    console.log(`#pragma region Words`)
    let names = []
    for(let i = 0; i < list.length; i++){
        let item = list[i]
        let name = `wordlist_${i}`;
        console.log(`const char ${name}[] PROGMEM = "${item}";`);
        names.push(name);
    }

    console.log(`PGM_P const wordlist[] PROGMEM = {`);
    for(let name of names){
        console.log(`\t${name},`)
    }
    console.log(`};`)

    console.log(`#pragma endregion`)

}


function testWordSelection(){
    const fs = require("fs")
    let list = fs.readFileSync("list.txt", "utf-8").split("\n");

    let accum = "";

    while(accum.length < 50 * 5){
        // 50 words at 5 letters per word
        accum += list[Math.floor(Math.random()*list.length)]
        accum += " "
    }
    accum.trim()
    console.log(accum)
    
}
 

// ps2 keyboard scancodes are not the same as ascii i fear
function generateKeyboardMap(){
    let _unshifted = [
    [0x0e, '`'],
    [0x15, 'q'],
    [0x16, '1'],
    [0x1a, 'z'],
    [0x1b, 's'],
    [0x1c, 'a'],
    [0x1d, 'w'],
    [0x1e, '2'],
    [0x21, 'c'],
    [0x22, 'x'],
    [0x23, 'd'],
    [0x24, 'e'],
    [0x25, '4'],
    [0x26, '3'],
    [0x29, ' '],
    [0x2a, 'v'],
    [0x2b, 'f'],
    [0x2c, 't'],
    [0x2d, 'r'],
    [0x2e, '5'],
    [0x31, 'n'],
    [0x32, 'b'],
    [0x33, 'h'],
    [0x34, 'g'],
    [0x35, 'y'],
    [0x36, '6'],
    [0x39, ','],
    [0x3a, 'm'],
    [0x3b, 'j'],
    [0x3c, 'u'],
    [0x3d, '7'],
    [0x3e, '8'],
    [0x41, ','],
    [0x42, 'k'],
    [0x43, 'i'],
    [0x44, 'o'],
    [0x45, '0'],
    [0x46, '9'],
    [0x49, '.'],
    [0x4a, '/'],
    [0x4b, 'l'],
    [0x4c, ';'],
    [0x4d, 'p'],
    [0x4e, '-'],
    [0x52, '`'],
    [0x54, '['],
    [0x55, '='],
    [0x5a, 13],
    [0x5b, ']'],
    [0x5d, '/'],
    [0x61, '<'],
    [0x66, 8],
    [0x69, '1'],
    [0x6b, '4'],
    [0x6c, '7'],
    [0x70, '0'],
    [0x71, ','],
    [0x72, '2'],
    [0x73, '5'],
    [0x74, '6'],
    [0x75, '8'],
    [0x79, '+'],
    [0x7a, '3'],
    [0x7b, '-'],
    [0x7c, '*'],
    [0x7d, '9'],
    [0, 0]]; // scancode, char

    let _shifted = [
    [0x0e, '`'],
    [0x15, 'Q'],
    [0x16, '!'],
    [0x1a, 'Z'],
    [0x1b, 'S'],
    [0x1c, 'A'],
    [0x1d, 'W'],
    [0x1e, '@'],
    [0x21, 'C'],
    [0x22, 'X'],
    [0x23, 'D'],
    [0x24, 'E'],
    [0x25, '$'],
    [0x26, '#'],
    [0x29, ' '],
    [0x2a, 'V'],
    [0x2b, 'F'],
    [0x2c, 'T'],
    [0x2d, 'R'],
    [0x2e, '%'],
    [0x31, 'N'],
    [0x32, 'B'],
    [0x33, 'H'],
    [0x34, 'G'],
    [0x35, 'Y'],
    [0x36, '^'],
    [0x39, 'L'],
    [0x3a, 'M'],
    [0x3b, 'J'],
    [0x3c, 'U'],
    [0x3d, '&'],
    [0x3e, '*'],
    [0x41, '<'],
    [0x42, 'K'],
    [0x43, 'I'],
    [0x44, 'O'],
    [0x45, ')'],
    [0x46, '('],
    [0x49, '>'],
    [0x4a, '?'],
    [0x4b, 'L'],
    [0x4c, ':'],
    [0x4d, 'P'],
    [0x4e, '_'],
    [0x52, '"'],
    [0x54, '['],
    [0x55, '+'],
    [0x5a, 13],
    [0x5b, ']'],
    [0x5d, '|'],
    [0x61, '>'],
    [0x66, 8],
    [0x69, '1'],
    [0x6b, '4'],
    [0x6c, '7'],
    [0x70, '0'],
    [0x71, ','],
    [0x72, '2'],
    [0x73, '5'],
    [0x74, '6'],
    [0x75, '8'],
    [0x79, '+'],
    [0x7a, '3'],
    [0x7b, '-'],
    [0x7c, '*'],
    [0x7d, '9'],
    [0, 0]]; // scancode char

    let total_size = 0
    for(let i = 0; i < _unshifted.length; i++){
        total_size += 2; // scancode, char
    }
    for(let i = 0; i < _shifted.length; i++){
        total_size += 2; // scancode, char
    }
    
    //instead do arr[scancode] = struct {char, shifted_char}, check size

    let alt_size = 0
    let max_scancode = 0
    for(let i = 0; i < _unshifted.length; i++){
        let scancode = _unshifted[i][0];
        if(scancode > max_scancode) max_scancode = scancode;
    }
    for(let i = 0; i < _shifted.length; i++){
        let scancode = _shifted[i][0];
        if(scancode > max_scancode) max_scancode = scancode;
    }
    alt_size = (max_scancode + 1) * 2;
    console.log(`// Total size method 1: ${total_size} bytes`);
    console.log(`// Total size method 2: ${alt_size} bytes`);
    // choose method 2
    // build array
    let arr = []
    for(let i = 0; i <= max_scancode; i++){
        arr.push([0,0]); // unshifted, shifted
    }
    for(let i = 0; i < _unshifted.length; i++){
        let scancode = _unshifted[i][0];
        let char = _unshifted[i][1];
        arr[scancode][0] = char;
    }
    for(let i = 0; i < _shifted.length; i++){
        let scancode = _shifted[i][0];
        let char = _shifted[i][1];
        arr[scancode][1] = char;
    }

    // print array
    console.log(`#define INVALID_KEYCODE {0, 0}`);

    console.log(`uint8_t keycodes[${arr.length}][2] = {`);
    for(let i = 0; i < arr.length; i++){
        let unshifted_char = arr[i][0];
        let shifted_char = arr[i][1];
        if(typeof unshifted_char === "string"){
            unshifted_char = `'${unshifted_char}'`;
        }
        if(typeof shifted_char === "string"){
            shifted_char = `'${shifted_char}'`;
        }
        if(unshifted_char === 0 && shifted_char === 0){
            console.log(`    INVALID_KEYCODE, // 0x${i.toString(16).padStart(2,'0')}`);
        } else {
            console.log(`    { ${unshifted_char}, ${shifted_char} }, // 0x${i.toString(16).padStart(2,'0')}`);
        }
    }
    console.log(`};`);
}

generateKeyboardMap();