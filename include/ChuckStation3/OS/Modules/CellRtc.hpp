#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <chrono>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class CellRtc {
public:
    CellRtc(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct CellRtcDateTime {
        BEField<u16> year;       
        BEField<u16> month;      
        BEField<u16> day;        
        BEField<u16> hour;       
        BEField<u16> minute;     
        BEField<u16> second;     
        BEField<u32> microsecond;
    };

    u64 cellRtcGetCurrentClockLocalTime();
    u64 cellRtcGetCurrentTick();
    u64 cellRtcGetTime_t();

private:
    MAKE_LOG_FUNCTION(log, cellRtc);
};