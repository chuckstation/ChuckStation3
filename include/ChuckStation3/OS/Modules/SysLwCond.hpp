#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>
#include <Lv2Objects/Lv2LwCond.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class SysLwCond {
public:
    SysLwCond(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct LwCond {
        BEField<u32> lwmutex_ptr;
        BEField<u32> id;
    };

    struct LwCondAttr {
        u8 name[8];
    };

    u64 sysLwCondWait();
    u64 sysLwCondCreate();
    u64 sysLwCondSignalAll();
    u64 sysLwCondSignal();

private:
    MAKE_LOG_FUNCTION(log, sysLwCond);
};