#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class CellNetCtl {
public:
    CellNetCtl(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    enum CellNetCtlState : s32 {
        CELL_NET_CTL_STATE_Disconnected = 0,
        CELL_NET_CTL_STATE_Connecting = 1,
        CELL_NET_CTL_STATE_IPObtaining = 2,
        CELL_NET_CTL_STATE_IPObtained = 3,
    };

    u64 cellNetCtlGetState();

private:
    MAKE_LOG_FUNCTION(log, cellNetCtl);
};