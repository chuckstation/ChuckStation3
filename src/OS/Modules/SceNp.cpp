#include "SceNp.hpp"
#include "PlayStation3.hpp"


u64 SceNp::sceNpDrmVerifyUpgradeLicense() {
    log("sceNpDrmVerifyUpgradeLicense()\n");
    
    //return SCE_NP_DRM_ERROR_LICENSE_NOT_FOUND;
    return CELL_OK;
}

u64 SceNp::sceNpManagerGetStatus() {
    const u32 status_ptr = ARG0;
    log("sceNpManagerGetStatus(status_ptr: 0x%08x) @ 0x%08x\n", status_ptr, ps3->ppu->state.lr);
    
    ps3->mem.write<u32>(status_ptr, -1);    // SCE_NP_MANAGER_STATUS_OFFLINE
    return CELL_OK;
}

u64 SceNp::sceNpBasicGetEvent() {
    //log("sceNpBasicGetEvent() STUBBED\n");

    return SCE_NP_BASIC_ERROR_NO_EVENT;
}

u64 SceNp::sceNpManagerGetNpId() {
    log("sceNpManagerGetNpId() STUBBED\n");

    //return CELL_OK;
    return SCE_NP_ERROR_OFFLINE;
}
