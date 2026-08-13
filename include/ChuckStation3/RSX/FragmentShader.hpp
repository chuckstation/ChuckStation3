#pragma once

#include <common.hpp>

#include <Memory.hpp>
#include <FragmentInstruction.hpp>


class FragmentShader {
public:
    FragmentShader() { addr = 0; ctrl = 0; }
    FragmentShader(u32 addr, u32 ctrl) : addr(addr), ctrl(ctrl) {}

    u32 addr;
    u32 ctrl;

    // Returns size of the fragment shader
    // TODO: when I implement control flow instructions, we need to analyze all possible code paths to find the actual size 
    u32 getSize(Memory& mem) {
        Helpers::debugAssert(addr != 0, "FragmentShader::getSize(): addr == 0\n");

        u32 offs = addr;
        while (true) {
            FragmentInstruction instr = fetchInstr(offs, mem);
            offs += sizeof(FragmentInstruction);
            if (instr.dst.end) break;
        }
        
        return offs - addr;
    }

    u8* getData(Memory& mem) {
        const auto size = getSize(mem);
        data.clear();
        data.resize(size);
        std::memcpy(data.data(), mem.getPtr(addr), size);
        return data.data();
    }

private:
    std::vector<u8> data;

    // TODO: this code is duplicated from FragmentShaderDecompiler.hpp
    // Maybe there is a better way to structure this
    FragmentInstruction fetchInstr(u32 addr, Memory& mem) {
        FragmentInstruction instr;
        instr.dst.raw = fetch32(addr + 0, mem);
        instr.src0.raw = fetch32(addr + 4, mem);
        instr.src1.raw = fetch32(addr + 8, mem);
        instr.src2.raw = fetch32(addr + 12, mem);
        return instr;
    }

    u32 fetch32(u32 addr, Memory& mem) {
        const auto data = mem.read<u32>(addr);
        return swap(data);
    }

    static u32 swap(u32 v) { return (v >> 16) | (v << 16); }
};