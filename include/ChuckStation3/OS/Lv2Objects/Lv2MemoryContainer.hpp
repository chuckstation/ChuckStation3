#pragma once

#include <common.hpp>

#include <queue>

#include <Lv2Base.hpp>


class Lv2MemoryContainer : public virtual Lv2Base {
public:
    Lv2MemoryContainer(Lv2Object* obj) : Lv2Base(obj) {}
    
    u32 vaddr = 0;
    u64 size = 0;

    void create(u64 size);
    void free();
};