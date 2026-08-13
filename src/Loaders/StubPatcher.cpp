#include "StubPatcher.hpp"


void StubPatcher::patch(u32 addr, bool lle, PlayStation3* ps3) {
    // Patch import stub
    // There are 2 different kinds of stubs, one uses bcctr, the other bcctrl, both are patched using syscalls.
    // We patch LLE stubs with a syscall as well. The syscall will then redirect the PPU to the appropiate function export.
    // While this isn't optimal (we could just patch the stub to directly jump to the function), it allows me to easily track LLE function calls.
    bool stubbed = false;
    bool addr_in_r2 = false;
    for (int i = 0; i < 128; i += 4) {
        // Find BCCTR or BCCTRL instructions, and patch the stub accordingly to whether it's HLE or LLE
        const auto instr_raw = ps3->mem.read<u32>(addr + i);
        const Instruction instr = { .raw = instr_raw };
        if (instr_raw == 0x4e800420) {
            if (!lle) {
                u32 sc = 0x10;
                if (addr_in_r2) sc += 0x2000;
                ps3->mem.write<u32>(addr + i - 4, 0x44000000 | sc);     // sc   (we check the low 16 bits to figure out the stub type)
                ps3->mem.write<u32>(addr + i - 0, 0x4e800020);          // blr
                stubbed = true;
            }
            else {
                u32 sc = 0x1010;
                if (addr_in_r2) sc += 0x2000;
                ps3->mem.write<u32>(addr + i - 4, 0x44000000 | sc);     // sc   (we check the low 16 bits to figure out the stub type)
                ps3->mem.write<u32>(addr + i - 0, 0x4e800020);          // blr
                stubbed = true;
            }
            break;
        }
        else if (instr_raw == 0x4e800421) {
            if (!lle) {
                u32 sc = 0x10;
                if (addr_in_r2) sc += 0x2000;
                ps3->mem.write<u32>(addr + i - 0, 0x44000000 | sc);          // sc
                stubbed = true;
            }
            else {
                u32 sc = 0x1010;
                if (addr_in_r2) sc += 0x2000;
                ps3->mem.write<u32>(addr + i - 0, 0x44000000 | sc);          // sc
                stubbed = true;
            }
            break;
        }
        // The stubs contain a branch to an other trampoline stub function, which should jump to the actual module function.
        // In these kinds of stubs, the address is stored in R2 instead of R12
        else if (instr.opc == PPUTypes::B) {
            if (instr.lk) continue;
            const s64 sli = (s64)(s32)(instr.li << 8) >> 6;
            addr += sli;
            addr_in_r2 = true;
            i = 0;
        }
        // Check if the stub was already patched.
        // This happens in games where the stubs all branch to the same trampoline function (see the if above)
        else if (instr.opc == PPUTypes::SC) {
            stubbed = true;
            break;
        }
        else if (addr_in_r2) {
            // Preserve the stub address pointer in R2 from getting overwritten
            if (instr.opc == PPUTypes::LWZ)
                ps3->mem.write<u32>(addr + i, 0x60000000);   // nop
        }
    }
    if (!stubbed) Helpers::panic("Couldn't patch stub\n");
}