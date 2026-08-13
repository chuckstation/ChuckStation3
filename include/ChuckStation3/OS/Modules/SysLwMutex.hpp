#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>
#include <Lv2Objects/Lv2Mutex.hpp>
#include "Syscalls/sys_mutex.hpp"


// Circular dependency
class PlayStation3;

using namespace CellTypes;
using namespace sys_mutex;

class SysLwMutex {
public:
    SysLwMutex(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct LwMutex {
        BEField<u32> owner;
        BEField<u32> waiter;

        BEField<u32> attribute;
        BEField<u32> recursive_count;
        BEField<u32> sleep_queue;
    };

    struct LwMutexAttrib {
        BEField<u32> protocol;
        BEField<u32> recursive;
        u8 name[8];
    };

    u64 sysLwMutexCreate();
    u64 sysLwMutexLock();
    u64 sysLwMutexTryLock();
    u64 sysLwMutexUnlock();
    u64 sysLwMutexDestroy();

private:
    MAKE_LOG_FUNCTION(log, sysLwMutex);
};
