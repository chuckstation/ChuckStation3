#include <Syscall.hpp>
#include "PlayStation3.hpp"
#include <Lv2Objects/Lv2EventPort.hpp>
#include <Lv2Objects/Lv2EventQueue.hpp>


MAKE_LOG_FUNCTION(log_sys_event, sys_event);

u64 Syscall::sys_event_port_create() {
    const u32 port_id_ptr = ARG0;
    const s32 port_type = ARG1;
    const u64 name = ARG2;
    log_sys_event("sys_event_port_create(port_id_ptr: 0x%08x, port_type: %d, name: 0x%016x)\n", port_id_ptr, port_type, name);

    Lv2EventPort* eport = ps3->lv2_obj.create<Lv2EventPort>();
    eport->name = name;
    eport->type = port_type;

    ps3->mem.write<u32>(port_id_ptr, eport->handle());
    return CELL_OK;
}

u64 Syscall::sys_event_port_connect_local() {
    const u32 port_id = ARG0;
    const u32 queue_id = ARG1;
    log_sys_event("sys_event_port_connect_local(port_id: %d, queue_id: %d)\n", port_id, queue_id);

    Lv2EventPort* eport = ps3->lv2_obj.get<Lv2EventPort>(port_id);
    eport->equeue_id = queue_id;

    return CELL_OK;
}

u64 Syscall::sys_event_port_send() {
    const u32 port_id = ARG0;
    const u64 data1 = ARG1;
    const u64 data2 = ARG2;
    const u64 data3 = ARG3;
    log_sys_event("sys_event_port_send(port_id: %d, data1: 0x%016llx, data2: 0x%016llx, data3: 0x%016llx)\n", port_id, data1, data2, data3);

    Lv2EventPort* eport = ps3->lv2_obj.get<Lv2EventPort>(port_id);
    Lv2EventQueue* equeue = ps3->lv2_obj.get<Lv2EventQueue>(eport->equeue_id);
    equeue->send({ eport->name, data1, data2, data3 });

    return CELL_OK;
}