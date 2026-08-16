#pragma once

#include <BEField.hpp>
#include <CellTypes.hpp>
#include <common.hpp>
#include <logger.hpp>

// Circular dependency
class PlayStation3;

using namespace CellTypes;

class SceNpTrophy {
public:
    SceNpTrophy(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    u64 sceNpTrophyRegisterContext();
    u64 sceNpTrophyCreateHandle();
    u64 sceNpTrophyGetRequiredDiskSpace();
    u64 sceNpTrophyGetGameInfo();
    u64 sceNpTrophyGetTrophyUnlockState();
    u64 sceNpTrophyCreateContext();

private:
    MAKE_LOG_FUNCTION(log, sceNpTrophy);
};