#include "PPUDisassembler.hpp"

#include <array>
#include <span>
#include <format>

Helpers::CapstoneDisassembler PPUDisassembler::capstone;

std::string PPUDisassembler::disasm(PPUTypes::State& state, PPUTypes::Instruction instr, Memory* mem, u32 override_pc) {
    auto capstoneDisasm = [](u32 instr, u32 pc) {
        if (!capstone.isInitialized()) {
            capstone.init(CS_ARCH_PPC, cs_mode(CS_MODE_64 | CS_MODE_BIG_ENDIAN | CS_MODE_PWR7));
        }

        std::string disassembly;
        // Convert instruction to byte array to pass to Capstone
        std::array<u8, 4> bytes = {
            u8((instr >> 24) & 0xff),
            u8((instr >> 16) & 0xff),
            u8((instr >> 8) & 0xff),
            u8(instr & 0xff),
        };
        if (!capstone.disassemble(disassembly, pc, std::span(bytes)))
            disassembly = std::format("???  0x{:08x}", instr);
        return std::format("0x{:08x}   |     {:s}", pc, disassembly.c_str());
    };
    
    return capstoneDisasm(instr.raw, override_pc ? override_pc : state.pc);

    /* TODO: Figure out what I want to do with this
    switch (instr.opc) {
    
    case PPUTypes::Instructions::ADDI:      return std::format("0x{:08x} | addi       r%d, r%d, 0x%04x", state.pc, (u8)instr.rt, (u8)instr.ra, (u8)instr.si); break;
    case PPUTypes::Instructions::ADDIS:     return std::format("0x{:08x} | addis      r%d, r%d, 0x%04x", state.pc, (u8)instr.rt, (u8)instr.ra, (u8)instr.si); break;
    case PPUTypes::Instructions::BC:        return std::format("0x{:08x} | bc%c%c%c       %d, %d, 0x%08x		; {:s}", state.pc, instr.lk ? 'l' : (instr.aa ? '\0' : ' '), instr.aa ? 'a' : ' ', (instr.aa && !instr.lk) ? ' ' : '\0', (u8)instr.bo, (u8)instr.bi, (s32)(s16)(instr.bd << 2), branchCondition(instr.bo, instr.bi, state) ? "taken" : "not taken");  break;
    case PPUTypes::Instructions::SC:        return std::format("0x{:08x} | sc", state.pc); break;
    case PPUTypes::Instructions::B:         return std::format("0x{:08x} | b%c%c%c        0x%08x", state.pc, instr.lk ? 'l' : (instr.aa ? '\0' : ' '), instr.aa ? 'a' : ' ', (instr.aa && !instr.lk) ? ' ' : '\0', (s32)(instr.li << 8) >> 6);  break;   // What the fuck did I just do
    case PPUTypes::Instructions::G_13: {
        switch (instr.g_13_field) {

        case PPUTypes::G_13Opcodes::BCLR:   return std::format("0x{:08x} | bclr%c      %d, %d  				; {:s}", state.pc, instr.lk ? 'l' : ' ', (u8)instr.bo, (u8)instr.bi, branchCondition(instr.bo, instr.bi, state) ? "taken" : "not taken");    break;
        case PPUTypes::G_13Opcodes::BCCTR:  return std::format("0x{:08x} | bcctr%c     %d, %d  				; {:s}", state.pc, instr.lk ? 'l' : ' ', (u8)instr.bo, (u8)instr.bi, branchCondition(instr.bo, instr.bi, state) ? "taken" : "not taken");    break;

        default: capstoneDisasm(instr.raw, state.pc); break;
        }
        break;
    }
    case PPUTypes::Instructions::ORI:       return std::format("0x{:08x} | ori        r%d, r%d, 0x%04x", state.pc, (u8)instr.ra, (u8)instr.rs, (u16)instr.ui); break;
    case PPUTypes::Instructions::ORIS:      return std::format("0x{:08x} | oris       r%d, r%d, 0x%04x 		; r%d = 0x{:08x}", state.pc, (u8)instr.ra, (u8)instr.rs, (u8)instr.ui, (u8)instr.rs, state.gprs[instr.rs]); break;
    case PPUTypes::Instructions::XORI:      return std::format("0x{:08x} | xori       r%d, r%d, 0x%04x", state.pc, (u8)instr.ra, (u8)instr.rs, (u16)instr.ui); break;
    case PPUTypes::Instructions::XORIS:     return std::format("0x{:08x} | xoris      r%d, r%d, 0x%04x", state.pc, (u8)instr.ra, (u8)instr.rs, (u16)instr.ui); break;
    case PPUTypes::Instructions::ANDI:      return std::format("0x{:08x} | andi       r%d, r%d, 0x%04x", state.pc, (u8)instr.ra, (u8)instr.rs, (u16)instr.ui); break;
    case PPUTypes::Instructions::ANDIS:     return std::format("0x{:08x} | andis      r%d, r%d, 0x%04x", state.pc, (u8)instr.ra, (u8)instr.rs, (u16)instr.ui); break;
    case PPUTypes::Instructions::G_1E: {
        switch (instr.g_1e_field) {

        case PPUTypes::G_1EOpcodes::RLDICL: return std::format("0x{:08x} | rldicl%c    r%d, r%d, %d, %d", state.pc, instr.rc ? '.' : ' ', (u8)instr.ra, (u8)instr.rs, instr.sh_lo | (instr.sh_hi << 5), ((instr.mb_6 & 1) << 5) | (instr.mb_6 >> 1));    break;
        case PPUTypes::G_1EOpcodes::RLDICR: return std::format("0x{:08x} | rldicr%c    r%d, r%d, %d, %d", state.pc, instr.rc ? '.' : ' ', (u8)instr.ra, (u8)instr.rs, instr.sh_lo | (instr.sh_hi << 5), ((instr.mb_6 & 1) << 5) | (instr.mb_6 >> 1));    break;

        default: capstoneDisasm(instr.raw, state.pc); break;
        }
        break;
    }
    case PPUTypes::Instructions::G_1F: {
        switch (instr.g_1f_field) {
        
        case PPUTypes::G_1FOpcodes::CMP:    return std::format("0x{:08x} | cmp        %d, %d, r%d, r%d", state.pc, (u8)instr.bf, (u8)instr.l, (u8)instr.ra, (u8)instr.rb);   break;
        case PPUTypes::G_1FOpcodes::CMPL:   return std::format("0x{:08x} | cmpl       %d, %d, r%d, r%d", state.pc, (u8)instr.bf, (u8)instr.l, (u8)instr.ra, (u8)instr.rb);   break;
        case PPUTypes::G_1FOpcodes::ADD:    return std::format("0x{:08x} | add%c       r%d, r%d, r%d", state.pc, instr.rc ? '.' : ' ', (u8)instr.rt, (u8)instr.ra, (u8)instr.rb);   break;
        case PPUTypes::G_1FOpcodes::MFSPR:  return std::format("0x{:08x} | mfspr", state.pc);    break;
        case PPUTypes::G_1FOpcodes::OR:     return std::format("0x{:08x} | or%c        r%d, r%d, r%d", state.pc, instr.rc ? '.' : ' ', (u8)instr.ra, (u8)instr.rs, (u8)instr.rb);   break;
        case PPUTypes::G_1FOpcodes::MTSPR:  return std::format("0x{:08x} | mtspr", state.pc);    break;
        case PPUTypes::G_1FOpcodes::EXTSH:  return std::format("0x{:08x} | extsh%c     r%d, r%d", state.pc, instr.rc ? '.' : ' ', (u8)instr.ra, (u8)instr.rs);   break;
        case PPUTypes::G_1FOpcodes::EXTSW:  return std::format("0x{:08x} | extsw%c     r%d, r%d", state.pc, instr.rc ? '.' : ' ', (u8)instr.ra, (u8)instr.rs);   break;

        default: capstoneDisasm(instr.raw, state.pc); break;
        }
        break;
    }
    case PPUTypes::Instructions::LWZ:       return std::format("0x{:08x} | lwz        r%d, %d(r%d)			; r%d <- [0x%08llx]", state.pc, (u8)instr.rt, (s32)(s16)instr.d, (u8)instr.ra, (u8)instr.rt, (instr.ra == 0) ? (s32)(s16)instr.d : state.gprs[instr.ra] + (s32)(s16)instr.d); break;
    case PPUTypes::Instructions::STW:       return std::format("0x{:08x} | stw        r%d, %d(r%d)			; [0x%08llx] <- r%d", state.pc, (u8)instr.rs, (s32)(s16)instr.d, (u8)instr.ra, (instr.ra == 0) ? (s32)(s16)instr.d : state.gprs[instr.ra] + (s32)(s16)instr.d, (u8)instr.rs); break;
    case PPUTypes::Instructions::LHZ:       return std::format("0x{:08x} | lhz        r%d, %d(r%d)			; r%d <- [0x%08llx]", state.pc, (u8)instr.rt, (s32)(s16)instr.d, (u8)instr.ra, (u8)instr.rt, (instr.ra == 0) ? (s32)(s16)instr.d : state.gprs[instr.ra] + (s32)(s16)instr.d); break;
    case PPUTypes::Instructions::G_3A: {
        switch (instr.g_3a_field) {

        case PPUTypes::G_3AOpcodes::LD:    return std::format("0x{:08x} | ld         r%d, %d(r%d)			; [0x%08llx] <- r%d", state.pc, (u8)instr.rt, (s32)(s16)(instr.ds << 2), (u8)instr.ra, (instr.ra == 0) ? (s32)(s16)(instr.ds << 2) : (s32)(s16)(instr.ds << 2) + state.gprs[instr.ra], (u8)instr.rs); break;
        case PPUTypes::G_3AOpcodes::LDU:   return std::format("0x{:08x} | ldu        r%d, %d(r%d)			; [0x%08llx] <- r%d", state.pc, (u8)instr.rt, (s32)(s16)(instr.ds << 2), (u8)instr.ra, (s32)(s16)(instr.ds << 2) + state.gprs[instr.ra], (u8)instr.rs); break;
        default: capstoneDisasm(instr.raw, state.pc); break;

        }
        break;
    }
    case PPUTypes::Instructions::G_3E: {
        switch (instr.g_3e_field) {

        case PPUTypes::G_3EOpcodes::STD:    return std::format("0x{:08x} | std        r%d, %d(r%d)			; [0x%08llx] <- r%d", state.pc, (u8)instr.rs, (s32)(s16)(instr.ds << 2), (u8)instr.ra, (instr.ra == 0) ? (s32)(s16)(instr.ds << 2) : (s32)(s16)(instr.ds << 2) + state.gprs[instr.ra], (u8)instr.rs); break;
        case PPUTypes::G_3EOpcodes::STDU:   return std::format("0x{:08x} | stdu       r%d, %d(r%d)			; [0x%08llx] <- r%d", state.pc, (u8)instr.rs, (s32)(s16)(instr.ds << 2), (u8)instr.ra, (s32)(s16)(instr.ds << 2) + state.gprs[instr.ra], (u8)instr.rs); break;
        default: capstoneDisasm(instr.raw, state.pc); break;

        }
        break;
    }
    
    default: capstoneDisasm(instr.raw, state.pc); break;
    }
    */
}
