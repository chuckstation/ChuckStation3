#include "SysMMapper.hpp"
#include "PlayStation3.hpp"


u64 SysMMapper::sysMMapperAllocateMemory() {
    const u32 size = ARG0;
    const u64 flags = ARG1;
    const u32 addr_ptr = ARG2;
    
    auto block = ps3->mem.allocPhys(size);
    auto handle = ps3->handle_manager.request();
    block->handle = handle;
    ps3->mem.write<u32>(addr_ptr, handle);

    log("sysMMapperAllocateMemory(size: 0x%08x, flags: 0x%016llx, addr_ptr: 0x%08x) [allocated memory paddr: 0x%08llx]\n", size, flags, addr_ptr, block->start);
    return CELL_OK;
}

u64 SysMMapper::sysMMapperFreeMemory() {
    const u32 handle = ARG0;
    log("sysMMapperFreeMemory(handle: 0x%08x)\n", handle);

    auto block = ps3->mem.findMapEntryWithHandle(handle);
    //Helpers::debugAssert(block.first, "sysMMapperFreeMemory: tried to free unmapped memory\n");
    if (!block.first) return CELL_OK;

    ps3->mem.free(block.second);

    return CELL_OK;
}

u64 SysMMapper::sysMMapperUnmapMemory() {
    // TODO: Right now I just free (and unmap) the memory when the free memory function is called
    const u32 addr = ARG0;
    const u32 handle_ptr = ARG1;
    log("sysMMapperUnmapMemory(addr: 0x%08x, handle_ptr: 0%08x)\n", addr, handle_ptr);

    auto info = ps3->mem.isMapped(addr);
    //Helpers::debugAssert(info.first, "sysMMapperUnmapMemory: tried to unmap unmapped memory\n");
    if (!info.first) return CELL_OK;

    ps3->mem.write<u32>(handle_ptr, info.second->handle);

    return CELL_OK;
}

u64 SysMMapper::sysMMapperMapMemory() {
    const u32 start_addr = ARG0;
    const u32 handle = ARG1;
    const u64 flags = ARG2;
    log("sysMMapperMapMemory(start_addr: 0x%08x, handle: 0x%08x, flags: 0x%08x)", start_addr, handle, flags);

    auto block = ps3->mem.ram.findBlockWithHandle(handle);
    Helpers::debugAssert(block.first, "sysMMapperMapMemory: unknown handle\n");
    if (ps3->mem.isMapped(start_addr).first) {
        logNoPrefix(" [already mapped, freeing block]\n");
        // If this area was already mapped, we free the new block. Should be OK. If things break try removing this.
        ps3->mem.freeBlockWithHandle(handle);
    }
    else {
        logNoPrefix("\n");
        ps3->mem.mmap(start_addr, block.second->start, block.second->size);
    }

    return CELL_OK;
}