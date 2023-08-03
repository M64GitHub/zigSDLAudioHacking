/*--------------------------------------------------------------------------+
 | zigSIDplay                                                     2023, M64 |
 v 00.0.0-dev                                     ( afk aka Mario Schallner )
                                                                            |
 +--------------------------------------------------------------------------+
 |     Supposed to become a SID player utilizing an SDL2 AudioQueue to
 |     output sound. The sound is generated utilizing a virtual C64 cpu
 |     to send register changes to a virtual C64 soundchip (SID) emulated
 |     by reSID code. 
 |     First in C (zig compiler drop in replacement), then rewrite          ^
 |     main logic in zig, and use zig build.                                |
\*==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "cpu.h"
#include "sidfile.h"
#include "zigSIDplay.h"

// -- ARG DEFAULTs, if not 0:
#define ARG_DEFAULT_BASENOTE    0xB0

// -- SDL2 stuff
#define SAMPLING_FREQ           48000    // 48khz
#define NUM_CHANNELS            2        // stereo
#define SIZE_AUDIO_BUF          16384    // 64k buffer(1S = stereo i16 = 2b)

// --

CPU_6510 cpu1;
SID_FILE sidfile1;

SDL_AudioDeviceID AUDIO_DEV_ID;

// -- helpers

void init_cmdline_args(CMDLINE_ARGS *args) {
    memset(args, 0, sizeof(CMDLINE_ARGS));
    args->basenote = ARG_DEFAULT_BASENOTE;
}

int parse_cmdline(CMDLINE_ARGS *args) { 
    return 0; 
}

// -- AUDIO

int init_sdl_audio() {
    SDL_AudioSpec audiospec;

    // initialise SDL audio subsystem only
    if (SDL_Init(SDL_INIT_AUDIO)) {
        printf("[ERR] initializing SDL_AUDIO subsystem: %s\n", SDL_GetError());
        return 2;
    }

    printf("[OK!] SDL2 audio subsystem initialized\n");

    // configure audio device struct
    memset(&audiospec, 0, sizeof(SDL_AudioSpec));

    audiospec.freq      = SAMPLING_FREQ;
    audiospec.format    = AUDIO_S8;         // 8 bit
    audiospec.channels  = NUM_CHANNELS;     // mono  / stereo
    audiospec.samples   = SIZE_AUDIO_BUF;   // size in samples
    audiospec.userdata  = NULL;
    audiospec.callback  = NULL;             // we use SDL_QueueAudio

    // use new interface, let QueueAudio convert
    AUDIO_DEV_ID = SDL_OpenAudioDevice(NULL, 0, &audiospec, NULL, 0);

    if (AUDIO_DEV_ID < 1) {
        printf("[ERR] initializing audio device: %s", SDL_GetError());
        return 1;
    }
    printf("[OK!] SDL2 audio device opened: ID:%d\n", AUDIO_DEV_ID);

    // audio devices default to being paused, so turn off pause
    SDL_PauseAudioDevice(AUDIO_DEV_ID, 0);

    return 0;
}

void test_audio() {
    char testbuf[48000]; // 1 sec
    float f;
    float PI = 3.141592;
   
    float ampl = 120.0;
    float freq = 400.0;
    for(int i=0; i<(48000 / 2); i++){
        f = ampl * sin((float)i/freq *2*PI);
        testbuf[i] = f;
        testbuf[i+1] = f;
        // if(i < freq) printf("f: %f (%d)\n",f, (char)f);
    }

    printf("[DBG] Queuing audio ...\n");
    int err;

    for(int i=0; i<3; i++) {
        printf("[DBG] Queuing audio %d ...\n", i);
        err=SDL_QueueAudio(AUDIO_DEV_ID, testbuf, 48000);
        if(err) printf("[ERR] initializing audio device: %s", SDL_GetError());
    }
}

// --

int main(int argc, char **argv) {
    CMDLINE_ARGS args;
    init_cmdline_args(&args);

    if (parse_cmdline(&args))
    return 1;

    if (init_sdl_audio())
    return 2;

    test_cpu(&cpu1);

    test_audio();

    printf("[INF] waiting 3 seconds for sound to finish ...\n");
    SDL_Delay(3000);

    return 0;
}
