#pragma once

#include <common.hpp>

#include <queue>

#include <Lv2Base.hpp>


class Lv2Mutex : public virtual Lv2Base {
public:
    Lv2Mutex(Lv2Object* obj) : Lv2Base(obj) {}

    u32 owner = -1; // -1 (0xffffffff) == free
    bool recursive = false;
    int recursive_cnt = 0;
    std::queue<u64> wait_list;

    bool isFree();
    bool lock();
    bool tryLock();
    void unlock();
};
