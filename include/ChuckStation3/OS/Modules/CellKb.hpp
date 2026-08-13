#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr u32 CELL_KB_ERROR_NO_DEVICE = 0x80121007;
static constexpr u32 CELL_KB_MAX_KEYBOARDS = 127;
static constexpr u32 CELL_KB_MAX_KEYCODES = 62;

class CellKb {
public:
    CellKb(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct CellKbInfo {
        BEField<u32> max_connect;
        BEField<u32> now_connect;
        BEField<u32> info;
        u8 status[CELL_KB_MAX_KEYBOARDS];
    };

    u64 cellKbGetInfo();
    u64 cellKbRead();

private:
    MAKE_LOG_FUNCTION(log, cellKb);
};