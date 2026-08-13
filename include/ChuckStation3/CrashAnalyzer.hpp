#pragma once

#include <common.hpp>

#include <queue>

#include <PPUTypes.hpp>


using namespace PPUTypes;
static constexpr size_t MAX_SAVED_STATES = 64_MB / sizeof(PPUTypes::State);

class CrashAnalyzer {
public:
    struct Step {
        PPUTypes::State state;    // The state is the state of the PPU *before* executing the instruction
        Instruction instr;
    };
    std::deque<Step> state_queue;

    void saveState(Step step) {
        state_queue.push_back(step);
        if (state_queue.size() >= MAX_SAVED_STATES)
            state_queue.pop_front();
    }

    void analyzeCrash(std::string reason) {
        // Invalid memory access - try to find out where the address came from
        // TODO: This doesn't work if the crash happened on an indexed load/store
        if (reason.contains("unmapped vaddr")) {
            auto last_state = lastState();
            auto last_instr = lastInstr();
            u32 reg = last_instr.ra;
            u32 addr = last_state.gprs[reg];
            s32 new_reg = -1;

            // Find the first instruction without addr in reg
            for (int i = state_queue.size() - 1 - 1; i > 0; i--) {
                if (state_queue[i].state.gprs[reg] != addr && state_queue[i + 1].state.gprs[reg] == addr) {
                    // This is the instruction that the address came from - print it and continue searching recursively
                    printf("The address 0x%08x was assigned to R%d on the following instruction:\n", addr, reg);
                    PPUDisassembler::disasm(state_queue[i].state, state_queue[i].instr, nullptr);
                    printf("Enter what register to continue tracking, or -1 to quit: ");
                    std::cin >> new_reg;
                    if (new_reg >= 0) {
                        reg = new_reg;
                        addr = state_queue[i].state.gprs[reg];
                    }
                    else return;
                    printf("Tracking address 0x%08x in register R%d\n", addr, reg);
                }
            }
            printf("Couldn't find more information\n");
        }
    }

    PPUTypes::State&    lastState() { return state_queue.back().state; }
    Instruction&        lastInstr() { return state_queue.back().instr; }
};