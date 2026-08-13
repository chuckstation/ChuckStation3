#pragma once

#include <common.hpp>

#include <PlayStation3.hpp>


namespace StubPatcher {

void patch(u32 addr, bool lle, PlayStation3* ps3);

}   // End namespace StubPatcher