#pragma once

#include <common.hpp>

#include <deque>

#include <Lv2Base.hpp>
#include <Thread.hpp>


class Lv2Semaphore : public virtual Lv2Base {
public:
    Lv2Semaphore(Lv2Object* obj) : Lv2Base(obj) {}

    u32 val = 0;
    u32 max_val = 0;

    std::deque<u64> wait_list;

    void post(u32 val);
    void wait(u64 timeout);
};
