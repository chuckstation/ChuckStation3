#pragma once

#include <common.hpp>


template<typename T>
struct BEField {
public:
    T val = 0;

    operator T() {
        return Helpers::bswap<T>(val);
    }

    T& operator=(const T& other) {
        val = Helpers::bswap<T>(other);
        return val;
    }
};
