#pragma once

#include <common.hpp>
#include <BEField.hpp>


namespace sys_mutex {

static constexpr u32 SYS_SYNC_RECURSIVE = 0x10;
static constexpr u32 SYS_SYNC_NOT_RECURSIVE = 0x20;

struct sys_mutex_attribute {
    BEField<u32> protocol;
    BEField<u32> recursive;
    BEField<u32> pshared;
    BEField<u32> adaptive;
    BEField<u64> ipc_key;
    BEField<s32> flags;
    BEField<u32> pad;
    char name[8];
};

}   // End namespace sys_mutex