#pragma once

#include <common.hpp>

#include <Lv2Base.hpp>
#include "Syscalls/sys_event.hpp"


using namespace sys_event;

class Lv2EventFlag : public virtual Lv2Base {
public:
    Lv2EventFlag(Lv2Object* obj) : Lv2Base(obj) {}

    struct EventFlagWaiter {
        u64 id;
        u64 bitptn;
        u32 mode;
        u32 res_ptr;
    };
    
    u64 val;
    std::vector<EventFlagWaiter> wait_list;
    
    bool wait(u64 bitptn, u32 mode, u32 res_ptr);
    void set(u64 bitptn);
    void clear(u64 bitptn);
    
private:
    bool maybeWakeUp(const EventFlagWaiter& waiter);
    void clearWithMode(u32 mode, u32 bitptn);
};
