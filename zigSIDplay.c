/*--------------------------------------------------------------------------+
 | zigSIDplay                                                     2023, M64 |
 v 00.0.0-dev                                     ( afk aka Mario Schallner )
                                                                            |
 +--------------------------------------------------------------------------+
 |
 |   supposed to become a SID player using reSID feeding SDL or sound.io
 |   audiostream, ... first in C (zig drop in compiler, then zig build),
 |   then rewrite in zig.                                                   ^
 |                                                                          |
\*==========================================================================*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zigSIDplay.h"

// -- DEFAULTs, if not 0:

#define ARG_DEFAULT_BASENOTE 0xB0

// -- SDL2 stuff

#define SAMPLING_FREQ 48000  // 48khz.
#define REVERB_BUF_LEN 4800  // 50ms.
#define OVERSAMPLE 2         // 2x oversampling.
#define NUM_CHANNELS 2       // Stereo.
#define BUFFER_SAMPLES 16384 // 64k buffer.

// -- helpers

void init_cmdline_args(CMDLINE_ARGS *args) {
    memset(args, 0, sizeof(CMDLINE_ARGS));
    args->basenote = ARG_DEFAULT_BASENOTE;
}

int parse_cmdline(CMDLINE_ARGS *args) { return 0; }

// -- SDL

int init_sdl() {
    SDL_AudioSpec audiospec;
    long result;

    // Initialise SDL audio subsystem only
    if (SDL_Init(SDL_INIT_AUDIO)) {
        printf("Error: initializing SDL_AUDIO: %s", SDL_GetError());
        return 2;
    }

    memset(&audiospec, 0, sizeof(SDL_AudioSpec));
    audiospec.freq = SAMPLING_FREQ;
    audiospec.format = AUDIO_S16SYS;
    audiospec.channels = NUM_CHANNELS;
    audiospec.samples = BUFFER_SAMPLES;
    // audiospec.callback = audio_callback;
    audiospec.userdata = NULL;

    // Open the audio device
    result = SDL_OpenAudio(&audiospec, NULL);
    if (result == 0) {
        // Wait until we have set up the audiostream(s)
        SDL_PauseAudio(0);
        printf("OK!: AudioDevice opened ....\n");
    }

    return 0;
}

int main(int argc, char **argv) {
    CMDLINE_ARGS args;
    SID_FILE sidfile;

    init_cmdline_args(&args);

    if (!init_sdl()) return 1;

    return 0;
}
