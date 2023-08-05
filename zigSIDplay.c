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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include "zsp_sdl_audio.h"

#include "zsp_term.h"
#include "zsp_logo.h"
#include "zsp_logo120.h"

#include "zsp_cpu.h"
#include "zsp_sid_music_file.h"
#include "zigSIDplay.h"

// -- ARG DEFAULTs, if not 0:
#define ARG_DEFAULT_BASENOTE    0xB0

// -- SDL2 stuff
#define SAMPLING_FREQ           48000    // 48khz
#define NUM_CHANNELS            2        // stereo
#define SIZE_AUDIO_BUF          16384    // 64k buffer(1S = stereo i16 = 4b)

// --

CPU_6510 ZSP_CPU1;
SID_FILE ZSP_SIDFile1;

SDL_AudioDeviceID   ZSP_AudioDevID;
SDL_AudioSpec       ZSP_AudioSpec;

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

    // printf("%s", zsp_logo120_txt);
    // printf("%s                                              "
    //        "  v00.00, M64%s\n",
    //        TERM_COLOR_LIGHTGRAY,
    //        TERM_DEFAULT
    //        );
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
    setbuf(stdout, NULL);
    // init
    //                  PC      A     X     Y     memchk enabled
    cpu_init(&ZSP_CPU1, 0x0000, 0x10, 0x00, 0x00, 0);
    cpu_test(&ZSP_CPU1);
    audio_test(ZSP_AudioDevID);

    cursor_off(); flush_term();
    println_inf("waiting for sound to finish ...");
    for(int i=0; i<3; i++) {
        print_inf("wait ");
        printf("%d",3-i);
        printf("\r"); flush_term(); 
        SDL_Delay(1000);
    }
    cursor_on(); flush_term(); 

    return 0;
}

