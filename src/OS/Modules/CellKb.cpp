#include "CellKb.hpp"
#include "PlayStation3.hpp"


u64 CellKb::cellKbGetInfo() {
    const u32 info_ptr = ARG0;
    log("cellKbGetInfo(info_ptr: 0x%08x)\n", info_ptr);

    CellKbInfo* info = (CellKbInfo*)ps3->mem.getPtr(info_ptr);
    info->max_connect = 1;
    info->now_connect = 0;

    return CELL_OK;
}

u64 CellKb::cellKbRead() {
    const u32 n_port = ARG0;
    const u32 data_ptr = ARG1;
    log("cellKbRead(n_port: %d, data_ptr: 0x%08x)\n", n_port, data_ptr);

    return CELL_KB_ERROR_NO_DEVICE;
}