#pragma once

#include <common.hpp>

#include <Lv2Base.hpp>


class Lv2EventPort : public virtual Lv2Base {
public:
    Lv2EventPort(Lv2Object* obj) : Lv2Base(obj) {}

    u64 name = 0;
    s32 type = 0;
    u64 equeue_id = 0;
};