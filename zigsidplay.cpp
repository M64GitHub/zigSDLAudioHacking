// ==========================================================================
// zigsidplay, 2023, M64 <afk aka M. Schallner>                   v00.0.0-dev
// <m64.overdrive@gmail.com>
// --------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zigsidplay.h"
#include "zsp_logo.h"
#include "zsp_term.h"
#include "zsp_sdl_audio.h"
#include "zsp_cpu.h"
#include "zsp_sidfile.h"
#include "zsp_sdl_resid.h"

// -- arg defaults if not 0:

#define ARG_DEFAULT_BASENOTE    0xB0    // C-3

// --  

#define SAMPLING_FREQ           44100    // 44.1 kHz
#define NUM_CHANNELS            2        // stereo
#define SIZE_AUDIO_BUF_SAMPLES  1024     // in samples

// --

CPU_6510 ZSP_CPU1;
SID_FILE ZSP_SIDFile1;
SDLreSID ZSP_RESID1;

SDL_AudioDeviceID   ZSP_AudioDevID;
SDL_AudioSpec       ZSP_AudioSpec;

// -- helpers

void init_cmdline_args(CMDLINE_ARGS *args) {
    memset(args, 0, sizeof(CMDLINE_ARGS));
    args->basenote = ARG_DEFAULT_BASENOTE;
}

int parse_cmdline(CMDLINE_ARGS *args) { 
    init_cmdline_args(args);
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


    println_inf( (char *) "starting playback");
    sdl_audio_play();

    println_inf( (char *) "waiting for sound to finish ...");
    cursor_off(); flush_term();
    while(PL_PLAYING) {
        progressbar(PL_SAMPLES_TOTAL, PL_SAMPLES_CURRENT, 40); // width: 40
        SDL_Delay(150);
    }                                              
    progressbar(PL_SAMPLES_TOTAL, PL_SAMPLES_CURRENT, 40); 
    printf("\n"); cursor_on(); flush_term(); 
    
    SDL_Quit();
    println_blu( (char *) "READY.");

    return 0;
}

