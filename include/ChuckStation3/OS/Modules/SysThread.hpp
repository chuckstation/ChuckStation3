#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>
#include <PPUTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr u32 SYS_PPU_THREAD_ONCE_INIT = 0;
static constexpr u32 SYS_PPU_THREAD_DONE_INIT = 1;

class SysThread {
public:
    SysThread(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    u64 sysPPUThreadCreate();
    u64 sysPPUThreadGetID();
    u64 sysPPUThreadInitializeTLS();
    u64 sysPPUThreadOnce();
    u64 sysPPUThreadExit();

    void initializeTLS(u64 thread_id, u32 tls_seg_addr, u32 tls_seg_size, u32 tls_mem_size, PPUTypes::State& state);

private:
    MAKE_LOG_FUNCTION(log, sysThread);
};