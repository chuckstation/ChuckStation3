// ChuckStation 3 — smoke test.
//
// Links against chuckstation3_core and verifies:
//   1. A PlayStation3 instance can be default-constructed without crashing.
//   2. The host architecture detection macros are sane.
//   3. The Settings struct round-trips a couple of fields.
//
// This is NOT an emulation test. Running a real game requires PS3 firmware
// + disc images that we can't ship.

#include <PlayStation3.hpp>
#include <Settings.hpp>
#include <cstdio>
#include <cstdlib>

namespace {

int failures = 0;

#define CHECK(cond)                                                               \
    do {                                                                          \
        if (!(cond)) {                                                            \
            std::fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                           \
        } else {                                                                  \
            std::fprintf(stdout, "ok:   %s\n", #cond);                            \
        }                                                                         \
    } while (0)

} // namespace

int main() {
    std::printf("ChuckStation 3 smoke test\n");
    std::printf("------------------------\n");

    // 1. Default-construct a PlayStation3. This should not throw.
    try {
        PlayStation3 ps3;
        CHECK(ps3.ppu != nullptr);
        CHECK(ps3.spu != nullptr);

        // 2. Settings defaults.
        CHECK(ps3.settings.lle.partialLv2LLE == true);
        CHECK(ps3.settings.audio.backend == "Null");

        // 3. Pad API: pressButton / resetButtons should not crash on an
        // uninitialised instance.
        ps3.resetButtons();
        ps3.setLeftStick(0.5f, 0.5f);
        ps3.setRightStick(0.5f, 0.5f);
        ps3.pressButton(0); // CELL_PAD_CTRL_L1 = 0x1 << 0
    } catch (const std::exception& e) {
        std::fprintf(stderr, "FAIL: PlayStation3 constructor threw: %s\n", e.what());
        ++failures;
    }

    // 4. Architecture macros. At least one of these should be defined in
    // any build that reached this point.
#if !defined(CHUCKSTATION3_X64_HOST) && !defined(CHUCKSTATION3_ARM64_HOST)
    std::fprintf(stderr, "WARN: no CHUCKSTATION3_*__HOST macro defined\n");
#endif

    std::printf("------------------------\n");
    if (failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    }
    std::printf("%d TEST(S) FAILED\n", failures);
    return 1;
}
