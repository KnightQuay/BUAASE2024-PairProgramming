import { readFileSync } from 'fs';
const wf = readFileSync('./t3-2-c/mancalaOperator.wasm')
const u8f = new Uint8Array(wf);
const wm = new WebAssembly.Module(u8f);

const wi = new WebAssembly.Instance(wm, 
{
    env:{
        mancalaOperator: function(flag, seq, size) {
            console.log(size);
        }
    }
})

function mancalaOperator(flag, seq, size) {
    const fptr = wi.exports.mancalaOperator;
    const mem = new Int32Array(wi.exports.memory.buffer);
    seq.forEach((v, i) => {
        mem[i] = v;
    });
    const res = fptr(flag, mem, size);
    return res;
}
export {mancalaOperator};