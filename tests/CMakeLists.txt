# ChuckStation 3 — tests/.
#
# Opt-in via -DENABLE_TESTS=ON. Builds a small smoke test that links against
# chuckstation3_core and verifies the core can be instantiated without
# crashing. Real emulation tests would need fixtures (PS3 BIOS, disc images)
# and are out of scope for the initial scaffold.

# We use a vendored single-header test runner so the smoke test has zero
# external dependencies. Replace with GoogleTest if the suite grows.
add_executable(chuckstation3_smoke_test
    smoke/smoke_main.cpp
)

target_link_libraries(chuckstation3_smoke_test PRIVATE
    ChuckStation3::Core
)

target_compile_definitions(chuckstation3_smoke_test PRIVATE
    CHUCKSTATION3_TEST
)

# Smoke test is registered with CTest.
include(CTest)
add_test(
    NAME chuckstation3_smoke_test
    COMMAND chuckstation3_smoke_test
)
