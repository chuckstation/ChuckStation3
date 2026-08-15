#pragma once

#include <PlayStation3.hpp>
#include <common.hpp>

namespace StubPatcher {

void patch(u32 addr, bool lle, PlayStation3* ps3);

} // End namespace StubPatcher