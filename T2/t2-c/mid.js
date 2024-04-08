import { readFileSync } from 'fs';
const wf = readFileSync('./t2-c/mancalaResult.wasm')
const u8f = new Uint8Array(wf);
const wm = new WebAssembly.Module(u8f);

const wi = new WebAssembly.Instance(wm, 
{
    env:{
        mancalaResult: function(flag, seq, size) {
            console.log(size);
        }
    }
})

function mancalaResult(flag, seq, size) {
    const fptr = wi.exports.mancalaResult;
    const mem = new Int32Array(wi.exports.memory.buffer);
    seq.forEach((v, i) => {
        mem[i] = v;
    });
    const res = fptr(flag, mem, size);
    return res;
}
export {mancalaResult};