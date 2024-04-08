import { readFileSync } from 'fs';
const wf = readFileSync('t1-c/bocchiShutUp.wasm')
const u8f = new Uint8Array(wf);
const wm = new WebAssembly.Module(u8f);

const wi = new WebAssembly.Instance(wm, 
{
    env:{
        bocchiShutUp: function(flag, seq, size) {
            console.log(size);
        }
    }
})

function bocchiShutUp(flag, seq, size) {
    const fptr = wi.exports.bocchiShutUp;
    const mem = new Int32Array(wi.exports.memory.buffer);
    seq.forEach((v, i) => {
        mem[i] = v;
    });
    const res = fptr(flag, mem, size);
    return res;
}
export {bocchiShutUp};
