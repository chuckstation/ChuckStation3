#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class CellResc {
public:
    CellResc(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    u64 cellRescSetDsts();
    u64 cellRescSetWaitFlip();
    u64 cellRescResetFlipStatus();
    u64 cellRescSetDisplayMode();
    u64 cellRescSetConvertAndFlip();
    u64 cellRescInit();
    u64 cellRescGetBufferSize();
    u64 cellRescSetBufferAddress();
    u64 cellRescGetFlipStatus();
    u64 cellRescVideoOutResolutionId2RescBufferMode();

private:
    MAKE_LOG_FUNCTION(log, cellResc);
};