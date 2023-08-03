/*--------------------------------------------------------------------------+
 | zigSIDplay                                                     2023, M64 |
 v 00.0.0-dev                                     ( afk aka Mario Schallner )
                                                                            |
 +--------------------------------------------------------------------------+
 |     Supposed to become a SID player utilizing an SDL2 AudioQueue to
 |     output sound. The sound is generated utilizing a virtual C64 cpu
 |     to send register changes to a virtual C64 soundchip (reSID). 
 |     First in C (zig compiler drop in replacement), then rewrite          ^
 |     main logic in zig, and use zig build. Then port to SoundIO.          |
\*==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "cpu.h"
#include "sidfile.h"
#include "zigSIDplay.h"

// -- ARG DEFAULTs, if not 0:
#define ARG_DEFAULT_BASENOTE 0xB0

// -- SDL2 stuff
#define SAMPLING_FREQ 48000  // 48khz
#define NUM_CHANNELS 2       // Stereo
#define BUFFER_SAMPLES 16384 // 64k buffer

// --

SID_FILE sf1;
CPU_6510 cpu1;
SDL_AudioDeviceID AUDIO_DEV_ID = 0;

// -- helpers

void init_cmdline_args(CMDLINE_ARGS *args) {
  memset(args, 0, sizeof(CMDLINE_ARGS));
  args->basenote = ARG_DEFAULT_BASENOTE;
}

int parse_cmdline(CMDLINE_ARGS *args) { return 0; }

void test_cpu(CPU_6510 *cpu) {
  printf("[DBG] testing cpu\n");
  //              PC      A     X     Y
  init_cpu(cpu, 0x0000, 0x10, 0x00, 0x00);

  // -- test ASL
  memset(cpu->mem, 0x0a, 0x10);
  run_cpu(cpu);
  run_cpu(cpu);
  run_cpu(cpu);
  run_cpu(cpu);
  run_cpu(cpu);
  run_cpu(cpu);

  // -- test ADC:
  // add 3 to a
  cpu->mem[0x006] = 0x69;
  cpu->mem[0x007] = 0x03;
  run_cpu(cpu);
  dmp_cpu_regs(cpu);

  // add 0xff -> overflow
  cpu1.mem[0x008] = 0x69;
  cpu1.mem[0x009] = 0xff;
  run_cpu(cpu);
  dmp_cpu_regs(cpu);
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
  audiospec.freq = SAMPLING_FREQ;
  audiospec.format = AUDIO_S8;
  audiospec.channels = NUM_CHANNELS;
  audiospec.samples = BUFFER_SAMPLES;
  audiospec.userdata = NULL;
  audiospec.callback = NULL;

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
