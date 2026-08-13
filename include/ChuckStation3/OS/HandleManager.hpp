#pragma once


class HandleManager {
public:
    u32 next_handle = 0x100;

    u32 request() { return next_handle++; }
};