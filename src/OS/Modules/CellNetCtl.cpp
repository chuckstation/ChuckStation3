#include "CellNetCtl.hpp"
#include "PlayStation3.hpp"


u64 CellNetCtl::cellNetCtlGetState() {
    const u32 state_ptr = ARG0;
    log("cellNetCtlGetState(state_ptr: 0x%08x)\n", state_ptr);

    //ps3->mem.write<u32>(state_ptr, CELL_NET_CTL_STATE_Disconnected);
    ps3->mem.write<u32>(state_ptr, CELL_NET_CTL_STATE_IPObtained);
    return CELL_OK;
}
