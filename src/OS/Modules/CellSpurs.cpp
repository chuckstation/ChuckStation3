#include "CellSpurs.hpp"
#include "PlayStation3.hpp"
#include <Loaders/SPU/SPULoader.hpp>


// TODO: SPU :(

u64 CellSpurs::_cellSpursLFQueueInitialize() {
    log("_cellSpursLFQueueInitialize() UNIMPLEMENTED\n");

    return CELL_OK;
}

u64 CellSpurs::cellSpursAttributeSetNamePrefix() {
    const u32 attr = ARG0;
    const u32 prefix_ptr = ARG1;
    const u32 size = ARG2;
    std::string prefix = Helpers::readString(ps3->mem.getPtr(prefix_ptr));
    log("cellSpursAttributeSetNamePrefix(attr: 0x%08x, prefix_ptr: 0x%08x, size: 0x%08x) [prefix: %s] UNIMPLEMENTED\n", attr, prefix_ptr, size, prefix.c_str());

    return CELL_OK;
}

u64 CellSpurs::cellSpursAttributeEnableSpuPrintfIfAvailable() {
    const u32 attr_ptr = ARG0;
    log("cellSpursAttributeEnableSpuPrintfIfAvailable(attr_ptr: 0x%08x) UNIMPLEMENTED\n", attr_ptr);

    return CELL_OK;
}

u64 CellSpurs::_cellSpursTasksetAttributeInitialize() {
    const u32 attr_ptr = ARG0;
    const u32 revision = ARG1;
    const u32 sdk_ver = ARG2;
    const u64 args = ARG3;
    const u32 prio_ptr = ARG4;
    const u32 max_contention = ARG5;
    log("_cellSpursTasksetAttributeInitialize(attr_ptr: 0x%08x, revision: 0x%08x, sdk_ver: 0x%08x, args: 0x%016llx, prio_ptr: 0x%08x, max_contention: %d) UNIMPLEMENTED\n", attr_ptr, revision, sdk_ver, args, prio_ptr, max_contention);

    return CELL_OK;
}

u64 CellSpurs::cellSpursLFQueueAttachLv2EventQueue() {
    const u32 queue_ptr = ARG0;
    log("cellSpursLFQueueAttachLv2EventQueue(queue_ptr: 0x%08x) UNIMPLEMENTED\n", queue_ptr);

    return CELL_OK;
}

u64 CellSpurs::cellSpursRequestIdleSpu() {
    log("cellSpursRequestIdleSpu() UNIMPLEMENTED\n");

    return CELL_OK;
}

u64 CellSpurs::cellSpursCreateTaskWithAttribute() {
    log("cellSpursCreateTaskWithAttribute() UNIMPLEMENTED\n");

    return CELL_OK;
}

u64 CellSpurs::cellSpursGetInfo() {
    const u32 spurs_ptr = ARG0;
    const u32 info_ptr = ARG1;
    log("cellSpursGetInfo(spurs_ptr: 0x%08x, info_ptr: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, info_ptr);

    return CELL_OK;
}

u64 CellSpurs::cellSpursInitializeWithAttribute2() {
    const u32 spurs_ptr = ARG0;
    const u32 attr_ptr = ARG1;
    log("cellSpursInitializeWithAttribute2(spurs_ptr: 0x%08x, attr_ptr: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, attr_ptr);

    return CELL_OK;
}

u64 CellSpurs::cellSpursWorkloadAttributeSetName() {
    const u32 attr_ptr = ARG0;
    const u32 name_class_ptr = ARG1;
    const u32 name_instance_ptr = ARG2;
    std::string name_class = Helpers::readString(ps3->mem.getPtr(name_class_ptr));
    std::string name_instance = Helpers::readString(ps3->mem.getPtr(name_instance_ptr));
    log("cellSpursWorkloadAttributeSetName(attr_ptr: 0x%08x, name_class_ptr: 0x%08x, name_instance_ptr: 0x%08x) [name_class: %s, name_instance: %s] UNIMPLEMENTED\n", attr_ptr, name_class_ptr, name_instance_ptr, name_class.c_str(), name_instance.c_str());

    return CELL_OK;
}

u64 CellSpurs::cellSpursCreateTaskset2() {
    const u32 spurs_ptr = ARG0;
    const u32 taskset_ptr = ARG1;
    const u32 attr_ptr = ARG2;
    log("cellSpursCreateTaskset2(spurs_ptr: 0x%08x, taskset_ptr: 0x%08x, attr_ptr: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, taskset_ptr, attr_ptr);

    return CELL_OK;
}

u64 CellSpurs::cellSpursEventFlagClear() {
    log("cellSpursEventFlagClear() UNIMPLEMENTED\n");
    
    return CELL_OK;
}

u64 CellSpurs::cellSpursCreateTaskset() {
    const u32 spurs_ptr = ARG0;
    const u32 taskset_ptr = ARG1;
    const u32 attr_ptr = ARG2;
    log("cellSpursCreateTaskset(spurs_ptr: 0x%08x, taskset_ptr: 0x%08x, attr_ptr: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, taskset_ptr, attr_ptr);

    return CELL_OK;
}

u64 CellSpurs::_cellSpursEventFlagInitialize() {
    const u32 spurs_ptr = ARG0;
    const u32 taskset_ptr = ARG1;
    const u32 event_flag_ptr = ARG2;
    const u32 flag_clear_mode = ARG3;
    const u32 flag_dir = ARG4;
    log("_cellSpursEventFlagInitialize(spurs_ptr: 0x%08x, taskset_ptr: 0x%08x, event_flag_ptr: 0x%08x, flag_clear_mode: 0x%08x, flag_dir: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, taskset_ptr, event_flag_ptr, flag_clear_mode, flag_dir);

    //ps3->mem.write<u16>(event_flag_ptr, 0xffff);
    //ps3->mem.write<u16>(event_flag_ptr + 2, 0xffff);
    //ps3->mem.write<u8>(event_flag_ptr + 7, 0xff);

    return CELL_OK;
}

u64 CellSpurs::cellSpursTasksetAttributeSetName() {
    const u32 attr_ptr = ARG0;
    const u32 name_ptr = ARG1;
    std::string name = Helpers::readString(ps3->mem.getPtr(name_ptr));
    log("cellSpursTasksetAttributeSetName(attr_ptr: 0x%08x, name_ptr: 0x%08x) [name: %s] UNIMPLEMENTED\n", attr_ptr, name_ptr, name.c_str());

    return CELL_OK;
}

u64 CellSpurs::cellSpursAttributeSetMemoryContainerForSpuThread() {
    const u32 attr_ptr = ARG0;
    const u32 container_id = ARG1;
    log("cellSpursAttributeSetMemoryContainerForSpuThread(attr_ptr: 0x%08x, container_id: %d) UNIMPLEMENTED\n", attr_ptr, container_id);

    return CELL_OK;
}

u64 CellSpurs::cellSpursAttributeEnableSystemWorkload() {
    const u32 attr_ptr = ARG0;
    const u32 prio_ptr = ARG1;
    const u32 max_spu = ARG2;
    const u32 is_preemptible_ptr = ARG3;
    std::string prio = Helpers::readString(ps3->mem.getPtr(prio_ptr));
    std::string is_preemptible = Helpers::readString(ps3->mem.getPtr(is_preemptible_ptr));
    log("cellSpursAttributeEnableSystemWorkload(attr_ptr: 0x%08x, prio_ptr: 0x%08x, max_spu: %d, is_preemptible_ptr: 0x%08x) [prio: %s, is_preemptible: %s] UNIMPLEMENTED\n", attr_ptr, prio_ptr, max_spu, is_preemptible_ptr, prio.c_str(), is_preemptible.c_str());

    return CELL_OK;
}

u64 CellSpurs::cellSpursEventFlagAttachLv2EventQueue() {
    const u32 event_flag_ptr = ARG0;
    log("cellSpursEventFlagAttachLv2EventQueue(event_flag_ptr: 0x%08x) UNIMPLEMENTED\n", event_flag_ptr);

    return CELL_OK;
}

u64 CellSpurs::_cellSpursLFQueuePushBody() {
    log("_cellSpursLFQueuePushBody() UNIMPLEMENTED\n");
    
    return CELL_OK;
}

u64 CellSpurs::_cellSpursAttributeInitialize() {
    const u32 attr_ptr = ARG0;
    const u32 revision = ARG1;
    const u32 sdk_ver = ARG2;
    const u32 n_spus = ARG3;
    const s32 spu_prio = ARG4;
    const s32 ppu_prio = ARG5;
    const u8 exit_if_no_work = ARG5;
    log("_cellSpursAttributeInitialize(attr_ptr: 0x%08x, revision: 0x%08x, sdk_ver: 0x%08x, n_spus: 0x%08x, spu_prio: 0x%08x, ppu_prio: 0x%08x, exit_if_no_work: 0x%08x) UNIMPLEMENTED\n", attr_ptr, revision, sdk_ver, n_spus, spu_prio, ppu_prio, exit_if_no_work);

    return CELL_OK;
}

u64 CellSpurs::_cellSpursWorkloadFlagReceiver() {
    const u32 spurs_ptr = ARG0;
    const u32 workload_id = ARG1;
    const u32 is_set = ARG2;
    log("_cellSpursWorkloadFlagReceiver(spurs_ptr: 0x%08x, workload_id: %d, is_set: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, workload_id, is_set);

    return CELL_OK;
}

u64 CellSpurs::cellSpursAttributeSetSpuThreadGroupType() {
    const u32 attr_ptr = ARG0;
    const s32 type = ARG1;
    log("cellSpursAttributeSetSpuThreadGroupType(attr_ptr: 0x%08x, type: %d) UNIMPLEMENTED\n", attr_ptr, type);

    return CELL_OK;
}

u64 CellSpurs::cellSpursInitializeWithAttribute() {
    const u32 spurs_ptr = ARG0;
    const u32 attr_ptr = ARG1;
    log("cellSpursInitializeWithAttribute(spurs_ptr: 0x%08x, attr_ptr: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, attr_ptr);
    
    return CELL_OK;
}

u64 CellSpurs::cellSpursInitialize() {
    const u32 spurs_ptr = ARG0;
    const u32 n_spus = ARG1;
    const u32 spu_prio = ARG2;
    const u32 ppu_prio = ARG3;
    const u8 exit_if_no_work = ARG4;
    log("cellSpursInitialize(spurs_ptr: 0x%08x, n_spus: %d, spu_prio: %d, ppu_prio: %d, exit_if_no_work: %d) UNIMPLEMENTED\n", spurs_ptr, n_spus, spu_prio, ppu_prio, exit_if_no_work);

    // Create SPURS thread
    auto thread = ps3->spu_thread_manager.createThread("HLE SPURS Thread");
    spurs_thread_id = thread->id;
    thread->wait();

    return CELL_OK;
}

u64 CellSpurs::_cellSpursTaskAttributeInitialize() {
    log("_cellSpursTaskAttributeInitialize() UNIMPLEMENTED\n");

    return CELL_OK;
}

u64 CellSpurs::cellSpursAttachLv2EventQueue() {
    const u32 spurs_ptr = ARG0;
    const u32 queue_id = ARG1;
    const u32 port_ptr = ARG2;
    const s32 is_dynamic = ARG3;
    log("cellSpursAttachLv2EventQueue(spurs_ptr: 0x%08x, queue_id: %d, port_ptr: 0x%08x, is_dynamic: %d) UNIMPLEMENTED\n", spurs_ptr, queue_id, port_ptr, is_dynamic);

    return CELL_OK;
}

u64 CellSpurs::cellSpursCreateTask() {
    const u32 taskset_ptr = ARG0;
    const u32 task_id_ptr = ARG1;
    const u32 elf_ptr = ARG2;
    const u32 ctx_ptr = ARG3;
    const u32 size = ARG4;
    const u32 pattern_ptr = ARG5;
    const u32 arg_ptr = ARG6;
    log("cellSpursCreateTask(taskset_ptr: 0x%08x, task_id_ptr: 0x%08x, elf_ptr: 0x%08x, ctx_ptr: 0x%08x, size: 0x%08d, pattern_ptr: 0x%08x, arg_ptr: 0x%08x) UNIMPLEMENTED\n", taskset_ptr, task_id_ptr, elf_ptr, ctx_ptr, size, pattern_ptr, arg_ptr);

    auto thread = ps3->spu_thread_manager.getThreadByID(spurs_thread_id);
    SPULoader loader = SPULoader(ps3);
    sys_spu_image* img = new sys_spu_image();
    loader.load(elf_ptr, img);
    thread->loadImage(img);
    thread->wakeUp();

    return CELL_OK;
}

u64 CellSpurs::cellSpursAddWorkloadWithAttribute() {
    const u32 spurs_ptr = ARG0;
    const u32 workload_id_ptr = ARG1;
    const u32 attr_ptr = ARG2;
    log("cellSpursAddWorkloadWithAttribute(spurs_ptr: 0x%08x, workload_id_ptr: 0x%08x, attr_ptr: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, workload_id_ptr, attr_ptr);

    return CELL_OK;
}

u64 CellSpurs::cellSpursCreateTasksetWithAttribute() {
    const u32 spurs_ptr = ARG0;
    const u32 taskset_ptr = ARG1;
    const u32 attr_ptr = ARG2;
    log("cellSpursCreateTasksetWithAttribute(spurs_ptr: 0x%08x, taskset_ptr: 0x%08x, attr_ptr: 0x%08x) UNIMPLEMENTED\n", spurs_ptr, taskset_ptr, attr_ptr);

    return CELL_OK;
}

u64 CellSpurs::_cellSpursTasksetAttribute2Initialize() {
    const u32 attr_ptr = ARG0;
    const u32 revision = ARG1;
    log("_cellSpursTasksetAttribute2Initialize(attr_ptr: 0x%08x, revision: 0x%08x) UNIMPLEMENTED\n", attr_ptr, revision);

    return CELL_OK;
}

u64 CellSpurs::cellSpursGetWorkloadFlag() {
    const u32 spurs_ptr = ARG0;
    const u32 flag_ptr = ARG1;
    log("cellSpursGetWorkloadFlag(spurs_ptr: 0x%08x, flag_ptr: 0x%08x) UNIMPLEMENTED @ 0x%08x\n", spurs_ptr, flag_ptr, ps3->ppu->state.lr);

    ps3->mem.write<u32>(flag_ptr, 0x400);  // This should be a pointer to a CellSpursWorkloadFlag I think? It's not the flag itself
    //CellSpursWorkloadFlag* flag = (CellSpursWorkloadFlag*)ps3->mem.getPtr(flag_ptr);
    //flag->flag = -1;

    return CELL_OK;
}

u64 CellSpurs::cellSpursSetExceptionEventHandler() {
    const u32 handler_ptr = ARG0;
    const u32 arg_ptr = ARG1;
    log("cellSpursSetExceptionEventHandler(handler_ptr: 0x%08x, arg_ptr: 0x%08x) UNIMPLEMENTED\n");

    return CELL_OK;
}

u64 CellSpurs::_cellSpursWorkloadAttributeInitialize() {
    const u32 attr_ptr = ARG0;
    const u32 revision = ARG1;
    const u32 sdk_ver = ARG2;
    const u32 pm_ptr = ARG3;
    const u32 size = ARG4;
    const u64 data = ARG5;
    const u32 prio_ptr = ARG6;
    const u32 min_cnt = ARG7;
    //const u32 max_cnt = ARG8;
    log("_cellSpursWorkloadAttributeInitialize(attr_ptr: 0x%08x, revision: 0x%08x, sdk_ver: 0x%08x, pm_ptr: 0x%08x, size: 0x%08x, data: 0x%016llx, prio_ptr: 0x%08x, min_cnt: %d, max_cnt: ???) UNIMPLEMENTED\n", attr_ptr, revision, sdk_ver, pm_ptr, size, data, prio_ptr, min_cnt);
    
    std::memset(ps3->mem.getPtr(attr_ptr), 0, sizeof(CellSpursWorkloadAttribute));
    CellSpursWorkloadAttribute* attr = (CellSpursWorkloadAttribute*)ps3->mem.getPtr(attr_ptr);
    attr->revision = revision;
    attr->sdk_ver = sdk_ver;
    attr->pm_ptr = pm_ptr;
    attr->size = size;
    attr->data = data;

    return CELL_OK;
}

// ***** SPU SIDE *****

void CellSpurs::spursTasksetProcessRequest() {
    const s32 request = ps3->spu->state.gprs[3].w[3];
    const u32 task_id_ptr = ps3->spu->state.gprs[4].w[3];
    const u32 is_waiting_ptr = ps3->spu->state.gprs[5].w[3];
    log("spursTasksetProcessRequest(request: %d, task_id_ptr: 0x%08x, is_waiting_ptr: 0x%08x)\n", request, task_id_ptr, is_waiting_ptr);
    log("request: %s\n", requestToString(request).c_str());

    SpursTasksetContext* taskset_ctxt = (SpursTasksetContext*)&ps3->spu->ls[0x2700];
    log("taskset addr: 0x%08x\n", (u64)taskset_ctxt->taskset_ptr);
    if (!ps3->mem.isMapped(taskset_ctxt->taskset_ptr).first) {
        log("taskset addr is unmapped, this is not a real call, returning\n");
        return;
    }

    auto read128 = [this](u32 addr) {
        v128 v;
        v.dw[1] = ps3->mem.read<u64>(addr + 0);
        v.dw[0] = ps3->mem.read<u64>(addr + 8);
        return v;
    };
    
    auto write128 = [this](u32 addr, v128 v) {
        ps3->mem.write<u64>(addr + 0, v.dw[1]);
        ps3->mem.write<u64>(addr + 8, v.dw[0]);
    };

    auto print = [](v128 v) {
        for (int j = 0; j < 16; j++)
            printf("%02x", v.b[15 - j]);
        printf("\n");
    };
    
    v128 waiting = read128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, waiting));
    v128 running = read128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, running));
    v128 ready = read128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, ready));
    v128 pending_ready = read128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, pending_ready));
    v128 enabled = read128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, enabled));
    v128 signalled = read128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, signalled));

    log("waiting        : "); print(waiting);
    log("running        : "); print(running);
    log("ready          : "); print(ready);
    log("pending_ready  : "); print(pending_ready);
    log("enabled        : "); print(enabled);
    log("signalled      : "); print(signalled);
    
    // TODO: This is a temporary hack
    for (int i = 0; i < 2; i++) {
        for (int task = 0; task < 64; task++) {
            // If the task is running but it is not enabled (invalid state)
            if (((running.dw[i] >> task) & 1) && !((enabled.dw[i] >> task) & 1)) {
                log("Correcting invalid taskset state\n");
                //running.dw[i] &= ~(1 << task);
                //write128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, running), running);
                enabled.dw[i] |= 1 << task;
                write128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, enabled), enabled);
            }
            // If the task is waiting but it is not enabled (invalid state)
            if (((waiting.dw[i] >> task) & 1) && !((enabled.dw[i] >> task) & 1)) {
                log("Correcting invalid taskset state\n");
                //waiting.dw[i] &= ~(1 << task);
                //write128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, waiting), waiting);
                enabled.dw[i] |= 1 << task;
                write128(taskset_ctxt->taskset_ptr + offsetof(CellSpursTaskset, enabled), enabled);
            }
        }
    }
}
