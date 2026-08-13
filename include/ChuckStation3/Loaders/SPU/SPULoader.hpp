#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>
#include <elfio/elfio.hpp>

#include <unordered_map>

#include <OS/Syscalls/sys_spu.hpp>


// Circular dependency
class PlayStation3;

static constexpr size_t EHDR_SIZE = 42;
static constexpr u64 INFO = 4;

using namespace sys_spu;

class SPULoader {
public:
    SPULoader(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;    

    void load(u32 img_ptr, sys_spu_image* img);
    void load(fs::path path, sys_spu_image* img, bool remove = false);

private:
    MAKE_LOG_FUNCTION(log, loader_spu);
};