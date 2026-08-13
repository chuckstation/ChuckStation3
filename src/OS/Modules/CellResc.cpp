#include "CellResc.hpp"
#include "PlayStation3.hpp"


u64 CellResc::cellRescSetDsts() {
    const u32 buf_mode = ARG0;
    const u32 dsts_ptr = ARG1;
    log("cellRescSetDsts(buf_mode: 0x%08x, dsts_ptr: 0x%08x) UNIMPLEMENTED\n", buf_mode, dsts_ptr);

    return CELL_OK;
}

u64 CellResc::cellRescSetWaitFlip() {
    log("cellRescSetWaitFlip()\n");

    // See cellGcmSetWaitFlip
    //ps3->thread_manager.getCurrentThread()->sleepForCycles(CPU_FREQ - ps3->curr_block_cycles - ps3->cycle_count);
    return CELL_OK;
}

u64 CellResc::cellRescResetFlipStatus() {
    log("cellRescResetFlipStatus()\n");
    
    ps3->module_manager.cellGcmSys.flip = 1;
    return CELL_OK;
}

u64 CellResc::cellRescSetDisplayMode() {
    const u32 buf_mode = ARG0;
    log("cellRescSetDisplayMode(buf_mode: 0x%08x) UNIMPLEMENTED\n", buf_mode);

    return CELL_OK;
}

u64 CellResc::cellRescSetConvertAndFlip() {
    const u32 context_addr = ARG0;
    const u32 buf_id = ARG1;
    log("cellRescSetConvertAndFlip()\n");

    CellGcmSys::CellGcmContextData* context = (CellGcmSys::CellGcmContextData*)ps3->mem.getPtr(context_addr);
    if (context->current + 8 >= context->end) ps3->module_manager.cellGcmSys.cellGcmCallback();
    ps3->mem.write<u32>(context->current, RSX::GCM_FLIP_COMMAND | (1 << 18));   // 1 is argc
    ps3->mem.write<u32>(context->current + 4, buf_id);
    context->current = context->current + 8;
    //if (context_addr == ps3->module_manager.cellGcmSys.ctx_addr) {
    //    ps3->module_manager.cellGcmSys.ctrl->put = ps3->module_manager.cellGcmSys.ctrl->put + 8;
    //    ps3->rsx.runCommandList();
    //}

    return CELL_OK;
}

u64 CellResc::cellRescInit() {
    const u32 config_ptr = ARG0;
    log("cellRescInit(config_ptr: 0x%08x) UNIMPLEMENTED\n", config_ptr);

    return CELL_OK;
}

u64 CellResc::cellRescGetBufferSize() {
    const u32 color_bufs = ARG0;
    const u32 vertex_array = ARG1;
    const u32 frag_shader = ARG2;
    log("cellRescGetBufferSize(color_bufs: 0x%08x, vertex_array: 0x%08x, frag_shader: 0x%08x) UNIMPLEMENTED\n", color_bufs, vertex_array, frag_shader);

    return CELL_OK;
}

u64 CellResc::cellRescSetBufferAddress() {
    const u32 color_bufs = ARG0;
    const u32 vertex_array = ARG1;
    const u32 frag_shader = ARG2;
    log("cellRescSetBufferAddress(color_bufs: 0x%08x, vertex_array: 0x%08x, frag_shader: 0x%08x) UNIMPLEMENTED\n", color_bufs, vertex_array, frag_shader);

    return CELL_OK;
}

u64 CellResc::cellRescGetFlipStatus() {
    log("cellRescGetFlipStatus()\n");
    return ps3->module_manager.cellGcmSys.flip;
}

u64 CellResc::cellRescVideoOutResolutionId2RescBufferMode() {
    const u32 res_id = ARG0;
    const u32 buf_mode = ARG1;
    log("cellRescVideoOutResolutionId2RescBufferMode(res_id: %d, buf_mode: 0x%08x) UNIMPLEMENTED\n", res_id, buf_mode);

    return CELL_OK;
}
