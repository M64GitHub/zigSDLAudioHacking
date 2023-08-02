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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "cpu.h"
#include "sidfile.h"
#include "zigSIDplay.h"

SID_FILE sf1;
CPU_6510 cpu1;

// -- ARG DEFAULTs, if not 0:

#define ARG_DEFAULT_BASENOTE 0xB0

// -- SDL2 stuff

#define SAMPLING_FREQ 48000  // 48khz
#define REVERB_BUF_LEN 4800  // 50ms
#define OVERSAMPLE 2         // 2x oversampling
#define NUM_CHANNELS 2       // Stereo
#define BUFFER_SAMPLES 16384 // 64k buffer

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

  // initialise SDL audio subsystem only
  if (SDL_Init(SDL_INIT_AUDIO)) {
    printf("[ERR] initializing SDL_AUDIO subsystem: %s", SDL_GetError());
    return 2;
  }

  // configure audio device struct
  memset(&audiospec, 0, sizeof(SDL_AudioSpec));
  audiospec.freq = SAMPLING_FREQ;
  audiospec.format = AUDIO_S16SYS;
  audiospec.channels = NUM_CHANNELS;
  audiospec.samples = BUFFER_SAMPLES;
  audiospec.userdata = NULL;

  // open audio device
  result = SDL_OpenAudio(&audiospec, NULL);
  if (result == 0) {
    // Wait until we have set up the audiostream(s)
    SDL_PauseAudio(0);
    printf("[OK!] sdl audio device opened\n");
    return 0;
  } else {
    printf("[ERR] initializing audio device: %s", SDL_GetError());
    return 1;
  }

  return 0;
}

int main(int argc, char **argv) {
    CMDLINE_ARGS args;
    SID_FILE sidfile;

    init_cmdline_args(&args);

    if (init_sdl()) return 1;

    // -- test cpu
    
    //              PC      A     X     Y
    init_cpu(&cpu1, 0x0000, 0x10, 0x00, 0x00);

    // -- test ASL
    memset(cpu1.mem, 0x0a, 0x10);
    run_cpu(&cpu1);
    run_cpu(&cpu1);
    run_cpu(&cpu1);
    run_cpu(&cpu1);
    run_cpu(&cpu1);
    run_cpu(&cpu1);

    // -- test ADC
    cpu1.mem[0x006] = 0x69;
    cpu1.mem[0x007] = 0x03;
    run_cpu(&cpu1);
    debug_cpu_regs(&cpu1);
    
    return 0;
}
