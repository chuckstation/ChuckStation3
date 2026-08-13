#include "Lv2EventFlag.hpp"
#include "PlayStation3.hpp"


bool Lv2EventFlag::wait(u64 bitptn, u32 mode, u32 res_ptr) {
    // Verify the mode is valid
    if ((mode & SYS_EVENT_FLAG_WAIT_AND) && (mode & SYS_EVENT_FLAG_WAIT_OR))
        Helpers::panic("Event flag: mode has both WAIT_AND and WAIT_OR set\n");
    if ((mode & SYS_EVENT_FLAG_WAIT_CLEAR) && (mode & SYS_EVENT_FLAG_WAIT_CLEAR_ALL))
        Helpers::panic("Event flag: mode has both WAIT_CLEAR and WAIT_CLEAR_ALL set\n");
    if (!(mode & SYS_EVENT_FLAG_WAIT_AND) && !(mode & SYS_EVENT_FLAG_WAIT_OR))
        Helpers::panic("Event flag: mode has neither WAIT_AND or WAIT_OR set\n");
    
    auto thread = ps3->thread_manager.getCurrentThread();
    EventFlagWaiter waiter = { thread->id, bitptn, mode, res_ptr };
    if (maybeWakeUp(waiter)) return false;
    
    thread->wait(std::format("eflag {:d}", handle()));
    wait_list.push_back(waiter);
    return true;
}

void Lv2EventFlag::set(u64 bitptn) {
    val |= bitptn;
    
    // We do this weird loop to avoid iterating a vector while we are removing elements from it.
    // Every time we erase one, start iterating over
    bool keep_running;
    do {
        keep_running = false;
        for (int i = 0; i < wait_list.size(); i++) {
            const auto& waiter = wait_list[i];
            if (maybeWakeUp(waiter)) {
                wait_list.erase(wait_list.begin() + i);
                keep_running = true;
                break;
            }
        }
    } while (keep_running);
}

void Lv2EventFlag::clear(u64 bitptn) {
    val &= bitptn;
}

// Returns whether or not the waiter woke up
bool Lv2EventFlag::maybeWakeUp(const EventFlagWaiter& waiter) {
    bool woke_up = false;
    
    // We verify that the mode is valid when we enlist the waiter, so there is no need to do that again here
    if (waiter.mode & SYS_EVENT_FLAG_WAIT_AND) {
        if ((val & waiter.bitptn) == waiter.bitptn) {
            ps3->thread_manager.getThreadByID(waiter.id)->wakeUp();
            woke_up = true;
        }
    } else if (waiter.mode & SYS_EVENT_FLAG_WAIT_OR) {
        if (val & waiter.bitptn) {
            ps3->thread_manager.getThreadByID(waiter.id)->wakeUp();
            woke_up = true;
        };
    }
    
    if (woke_up) {
        if (waiter.res_ptr)
            ps3->mem.write<u64>(waiter.res_ptr, val);
        clearWithMode(waiter.mode, waiter.bitptn);
    }
    
    return woke_up;
}

void Lv2EventFlag::clearWithMode(u32 mode, u32 bitptn) {
    if (mode & SYS_EVENT_FLAG_WAIT_CLEAR) {
        val &= ~bitptn;
    } else if (mode & SYS_EVENT_FLAG_WAIT_CLEAR_ALL) {
        val = 0;
    }
}
