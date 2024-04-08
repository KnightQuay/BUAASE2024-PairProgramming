import { readFileSync } from 'fs';
const wf = readFileSync('./t3-1-c/mancalaBoard.wasm')
const u8f = new Uint8Array(wf);
const wm = new WebAssembly.Module(u8f);

const wi = new WebAssembly.Instance(wm, 
{
    env:{
        mancalaBoard: function(flag, seq, size) {
            console.log(size);
        }
    }
})

function mancalaBoard(flag, seq, size) {
    const fptr = wi.exports.mancalaBoard;
    const mem = new Int32Array(wi.exports.memory.buffer);
    seq.forEach((v, i) => {
        mem[i] = v;
    });
    const res = fptr(flag, mem, size);
    const jsArray = [];
    for (let i = 0; i < 15; i++) {
        jsArray.push(mem[res / 4 + i]);
    }
    // console.log("this is the board res!");
    // console.log(jsArray);
    return jsArray;
}
export {mancalaBoard};