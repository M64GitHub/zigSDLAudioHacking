/*--------------------------------------------------------------------------+
 | zigSIDplay                                                     2023, M64 |
 v 00.0.0-dev                                     ( afk aka Mario Schallner )
                                                                            |
 +--------------------------------------------------------------------------+
 |     Aimed to become a SID player utilizing an SDL2 AudioQueue to
 |     output sound. The sound is generated utilizing a virtual C64 cpu
 |     to send register changes to a virtual C64 soundchip (SID) emulated
 |     by reSID code. 
 |     First in C (zig compiler drop in replacement), then rewrite          ^
 |     main logic in zig, and use zig build.                                |
\*==========================================================================*/
#include "zigSIDplay.h"
#include "zsp_logo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "zsp_term.h"
#include "zsp_sdl_audio.h"
#include "zsp_cpu.h"
#include "zsp_sid_music_file.h"
//
#include "zsp_SDLreSID.h"


// -- ARG DEFAULTs, if not 0:
#define ARG_DEFAULT_BASENOTE    0xB0

// -- SDL2 stuff
#define SAMPLING_FREQ           48000    // 48khz
#define NUM_CHANNELS            2        // stereo
#define SIZE_AUDIO_BUF          16384    // 64k buffer(1S = stereo i16 = 4b)

// --

CPU_6510 ZSP_CPU1;
// SID_FILE ZSP_SIDFile1;
SDLreSID ZSP_RESID1;
//
SDL_AudioDeviceID   ZSP_AudioDevID;
SDL_AudioSpec       ZSP_AudioSpec;
//
// -- helpers

void init_cmdline_args(CMDLINE_ARGS *args) {
    memset(args, 0, sizeof(CMDLINE_ARGS));
    args->basenote = ARG_DEFAULT_BASENOTE;
}

int parse_cmdline(CMDLINE_ARGS *args) { 
    return 0; 
}

void print_header() {
    setbuf(stdout, NULL); // turn off buffering
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
    print_header();
    CMDLINE_ARGS args;

    init_cmdline_args(&args);
    
    if (parse_cmdline(&args)) return 1;
    
    if (sdl_audio_init(&ZSP_AudioDevID, 
                       &ZSP_AudioSpec,
                        SAMPLING_FREQ,
                        NUM_CHANNELS,
                        SIZE_AUDIO_BUF))
                        return 2;

    // init
    //                  PC      A     X     Y     memchk enabled
    cpu_init(&ZSP_CPU1, 0x0000, 0x10, 0x00, 0x00, 0);
    cpu_test(&ZSP_CPU1);
    audio_test(ZSP_AudioDevID);

    println_inf("waiting for sound to finish ...");
    pb_delay(2000, 40);

    println_blu("READY.");
    return 0;
}

