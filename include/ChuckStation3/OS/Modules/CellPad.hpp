#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr u32 CELL_MAX_PADS = 127;

static constexpr u32 CELL_PAD_MAX_PORT_NUM = 7;
static constexpr u32 CELL_PAD_MAX_CODES = 64;

static constexpr u32 CELL_PAD_STATUS_DISCONNECTED       = 0;
static constexpr u32 CELL_PAD_STATUS_CONNECTED          = 1;
static constexpr u32 CELL_PAD_STATUS_ASSIGN_CHANGES     = 2;
static constexpr u32 CELL_PAD_STATUS_CUSTOM_CONTROLLER  = 4;

// Digital1
static constexpr u32 CELL_PAD_CTRL_LEFT      = (1 << 7);
static constexpr u32 CELL_PAD_CTRL_DOWN      = (1 << 6);
static constexpr u32 CELL_PAD_CTRL_RIGHT     = (1 << 5);
static constexpr u32 CELL_PAD_CTRL_UP        = (1 << 4);
static constexpr u32 CELL_PAD_CTRL_START     = (1 << 3);
static constexpr u32 CELL_PAD_CTRL_R3        = (1 << 2);
static constexpr u32 CELL_PAD_CTRL_L3        = (1 << 1);
static constexpr u32 CELL_PAD_CTRL_SELECT    = (1 << 0);
// Digital2
static constexpr u32 CELL_PAD_CTRL_SQUARE    = (1 << (7 + 16));
static constexpr u32 CELL_PAD_CTRL_CROSS     = (1 << (6 + 16));
static constexpr u32 CELL_PAD_CTRL_CIRCLE    = (1 << (5 + 16));
static constexpr u32 CELL_PAD_CTRL_TRIANGLE  = (1 << (4 + 16));
static constexpr u32 CELL_PAD_CTRL_R1        = (1 << (3 + 16));
static constexpr u32 CELL_PAD_CTRL_L1        = (1 << (2 + 16));
static constexpr u32 CELL_PAD_CTRL_R2        = (1 << (1 + 16));
static constexpr u32 CELL_PAD_CTRL_L2        = (1 << (0 + 16));

class CellPad {
public:
    CellPad(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct CellPadData {
        BEField<s32> len;
        BEField<u16> button[CELL_PAD_MAX_CODES];
    };

    struct CellPadInfo {
        BEField<u32> max_connect;
        BEField<u32> now_connect;
        BEField<u32> system_info;
        BEField<u16> vendor_id[CELL_MAX_PADS];
        BEField<u16> product_id[CELL_MAX_PADS];
        u8 status[CELL_MAX_PADS];
    };

    struct CellPadInfo2 {
        BEField<u32> max_connect;
        BEField<u32> now_connect;
        BEField<u32> system_info;
        BEField<u32> port_status[CELL_PAD_MAX_PORT_NUM];
        BEField<u32> port_setting[CELL_PAD_MAX_PORT_NUM];
        BEField<u32> device_capability[CELL_PAD_MAX_PORT_NUM];
        BEField<u32> device_type[CELL_PAD_MAX_PORT_NUM];
    };

    u64 cellPadInit();
    u64 cellPadGetInfo();
    u64 cellPadGetData();
    u64 cellPadGetInfo2();

    u16 buttons[CELL_PAD_MAX_CODES];

private:
    MAKE_LOG_FUNCTION(log, cellPad);
};