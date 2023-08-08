// ==========================================================================
// zigSIDplay, 2023, M64 
// v00.0.0-dev
// --------------------------------------------------------------------------
// Aimed to become a SID player, using SDL2 to output audio. The sound is 
// generated utilizing a virtual C64 cpu to send register changes to a 
// virtual C64 soundchip (SID, emulated by reSID code). 
// ==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "zigSIDplay.h"
#include "zsp_logo.h"
#include "zsp_term.h"
#include "zsp_sdl_audio.h"
#include "zsp_cpu.h"
#include "zsp_sidfile.h"
#include "zsp_sdl_resid.h"

// -- arg defaults if not 0:

#define ARG_DEFAULT_BASENOTE    0xB0

// --  

#define SAMPLING_FREQ           44100    // 44.1 kHz
#define NUM_CHANNELS            2        // stereo
#define SIZE_AUDIO_BUF_SAMPLES  1024     // in samples 4k buffer
                                         // we will go with 16bit ->
                                         // (1sf = stereo i16 = 4b)

// --

CPU_6510 ZSP_CPU1;
SID_FILE ZSP_SIDFile1;
SDLreSID ZSP_RESID1;

SDL_AudioDeviceID   ZSP_AudioDevID;
SDL_AudioSpec       ZSP_AudioSpec;

SDL_TimerID ZSP_TimerID_CPU;
SDL_TimerID ZSP_TimerID_AUDIO;
  
// -- helpers

void init_cmdline_args(CMDLINE_ARGS *args) {
    memset(args, 0, sizeof(CMDLINE_ARGS));
    args->basenote = ARG_DEFAULT_BASENOTE;
}

int parse_cmdline(CMDLINE_ARGS *args) { 
    return 0; 
}

void print_header() {
    printf("%s", zsp_logo_txt); 
    printf("%s                                                            "
           "  v00.00, M64%s\n",
           TERM_COLOR_LIGHTGRAY,
           TERM_DEFAULT
           );
    flush_term();
}

// --

int main(int argc, char **argv) {
    CMDLINE_ARGS args;

    term_init();
    print_header();

    init_cmdline_args(&args);
    if (parse_cmdline(&args)) 
        return 1;
    
    // init
    //                  PC      A     X     Y     memchk 
    //                                            enabled
    cpu_init(&ZSP_CPU1, 0x0000, 0x10, 0x00, 0x00, 0);
    cpu_test(&ZSP_CPU1);

    if (sdl_audio_init(&ZSP_AudioDevID, 
                       &ZSP_AudioSpec,
                        SAMPLING_FREQ,
                        NUM_CHANNELS,
                        SIZE_AUDIO_BUF_SAMPLES)) 
        return 2;

    audio_test(ZSP_AudioDevID);
    println_inf("waiting for sound to finish ...");
    //       4.4s  40 chars progressbar
    pb_delay(4400, 40);


    SDL_Quit();
    println_blu("READY.");

    return 0;
}

