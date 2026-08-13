#pragma once

#include <common.hpp>

#include <functional>

#include <CellTypes.hpp>


using namespace CellTypes;

class Import {
public:
    Import() {}
    Import(std::string name, std::function<u64(void)> const& handler, bool force_hle = false) : name(name), handler(handler), force_hle(force_hle) {}
    std::string name;
    std::function<u64(void)> handler;
    bool force_hle;
};