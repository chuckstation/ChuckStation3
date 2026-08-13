#pragma once

#include <common.hpp>
#include <BEField.hpp>


namespace sys_event {

static constexpr u32 SYS_EVENT_FLAG_WAIT_AND = 0x01;
static constexpr u32 SYS_EVENT_FLAG_WAIT_OR = 0x02;
static constexpr u32 SYS_EVENT_FLAG_WAIT_CLEAR = 0x10;
static constexpr u32 SYS_EVENT_FLAG_WAIT_CLEAR_ALL = 0x20;

struct sys_event {
    BEField<u64> source;
    BEField<u64> data1;
    BEField<u64> data2;
    BEField<u64> data3;
};

}   // End namespace sys_event