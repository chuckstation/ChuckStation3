#include <Syscall.hpp>
#include "sys_event.hpp"
#include "PlayStation3.hpp"
#include "Lv2Objects/Lv2EventFlag.hpp"


MAKE_LOG_FUNCTION(log_sys_event_flag, sys_event_flag);

u64 Syscall::sys_event_flag_create() {
    const u32 id_ptr = ARG0;
    const u32 attr_ptr = ARG1;
    const u64 init = ARG2;
    log_sys_event_flag("sys_event_flag_create(id_ptr: 0x%08x, attr_ptr: 0x%08x, init: 0x%016llx)\n", id_ptr, attr_ptr, init);
    
    Lv2EventFlag* eflag = ps3->lv2_obj.create<Lv2EventFlag>();
    eflag->val = init;
    
    ps3->mem.write<u32>(id_ptr, eflag->handle());
    return CELL_OK;
}

u64 Syscall::sys_event_flag_wait() {
    const u32 id = ARG0;
    const u64 bitptn = ARG1;
    const u32 mode = ARG2;
    const u32 res_ptr = ARG3;   // res is u64
    const u64 timeout = ARG4;
    log_sys_event_flag("sys_event_flag_wait(id: %d, bitptn: 0x%016llx, mode: 0x%08x, res_ptr: 0x%08x, timeout: %d)\n", id, bitptn, mode, res_ptr, timeout);
    
    Lv2EventFlag* eflag = ps3->lv2_obj.get<Lv2EventFlag>(id);
    eflag->wait(bitptn, mode, res_ptr);
    
    return CELL_OK;
}

u64 Syscall::sys_event_flag_set() {
    const u32 id = ARG0;
    const u64 bitptn = ARG1;
    log_sys_event_flag("sys_event_flag_set(id: %d, bitptn: 0x%016llx)\n", id, bitptn);
    
    Lv2EventFlag* eflag = ps3->lv2_obj.get<Lv2EventFlag>(id);
    eflag->set(bitptn);
    
    return CELL_OK;
}

u64 Syscall::sys_event_flag_clear() {
    const u32 id = ARG0;
    const u64 bitptn = ARG1;
    log_sys_event_flag("sys_event_flag_clear(id: %d, bitptn: 0x%016llx)\n", id, bitptn);

    Lv2EventFlag* eflag = ps3->lv2_obj.get<Lv2EventFlag>(id);
    eflag->clear(bitptn);
    
    return CELL_OK;
}
