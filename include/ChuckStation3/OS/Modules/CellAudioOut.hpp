#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr u8 CELL_AUDIO_OUT_OUTPUT_STATE_ENABLED    = 0;
static constexpr u8 CELL_AUDIO_OUT_OUTPUT_STATE_DISABLED   = 1;
static constexpr u8 CELL_AUDIO_OUT_OUTPUT_STATE_PREPARING  = 2;

class CellAudioOut {
public:
    CellAudioOut(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct CellAudioOutSoundMode {
        u8 type;
        u8 channel;
        u8 fs;
        u8 reserved;
        BEField<u32> layout;
    };
    
    struct CellAudioOutState {
        u8 state;
        u8 encoder;
        u8 reserved[6];
        BEField<u32> down_mixer;
        CellAudioOutSoundMode sound_mode;
    };
    
    u64 cellAudioOutGetSoundAvailability();
    u64 cellAudioOutGetState();

private:
    MAKE_LOG_FUNCTION(log, cellAudioOut);
};
