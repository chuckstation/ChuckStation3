#include "CellAudioOut.hpp"
#include "PlayStation3.hpp"


u64 CellAudioOut::cellAudioOutGetSoundAvailability() {
    const u32 audio_out = ARG0;
    const u32 type = ARG1;
    const u32 fs = ARG2;
    const u32 option = ARG3;
    log("cellAudioOutGetSoundAvailability(audio_out: %d, type: %d, fs: %d, option: %d)\n", audio_out, type, fs, option);
    
    // 2 == number of channels available
    if (audio_out == 0) return 2;   // CELL_AUDIO_OUT_PRIMARY
    
    return 0;
}

u64 CellAudioOut::cellAudioOutGetState() {
    const u32 audio_out = ARG0;
    const u32 dev_idx = ARG1;
    const u32 state_ptr = ARG2;
    log("cellAudioOutGetState(audio_out: %d, dev_idx: %d, state_ptr: 0x%08x) STUBBED\n", audio_out, dev_idx, state_ptr);

    CellAudioOutState* state = (CellAudioOutState*)ps3->mem.getPtr(state_ptr);
    state->state = CELL_AUDIO_OUT_OUTPUT_STATE_ENABLED;
    state->encoder = 0; // CELL_AUDIO_OUT_CODING_TYPE_LPCM
    state->down_mixer = 0;  // CELL_AUDIO_DOWNMIXER_NONE
    state->sound_mode.type = 0; // CELL_AUDIO_OUT_CODING_TYPE_LPCM
    state->sound_mode.channel = 2;
    state->sound_mode.fs = 4;   // CELL_AUDIO_OUT_FS_48KHZ
    state->sound_mode.layout = 1;   // CELL_AUDIO_OUT_SPEAKER_LAYOUT_2CH

    return CELL_OK;
}
