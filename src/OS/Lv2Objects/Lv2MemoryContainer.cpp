#include "Lv2MemoryContainer.hpp"
#include "PlayStation3.hpp"


void Lv2MemoryContainer::create(u64 size) {
    vaddr = ps3->mem.alloc(size)->vaddr;
    this->size = size;
}

void Lv2MemoryContainer::free() {
    if (!vaddr) Helpers::panic("Lv2MemoryContainer::free: tried to free an already destroyed memory container\n");

    auto [ok, entry] = ps3->mem.isMapped(vaddr);
    if (!ok) Helpers::panic("Lv2MemoryContainer::free: vaddr was unmapped\n");

    ps3->mem.free(entry);
}