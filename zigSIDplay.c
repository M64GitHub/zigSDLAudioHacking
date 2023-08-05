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

    flush_term();

    // printf("%s", zsp_logo120_txt);
    // printf("%s                                              "
    //        "  v00.00, M64%s\n",
    //        TERM_COLOR_LIGHTGRAY,
    //        TERM_DEFAULT
    //        );
}

void delay(int ms, double pb_width) {
    println_inf("waiting for sound to finish ...");
    int count = ms / 100;
    char buf[256];
    double max;
    int j=0;

    buf[0] = 0x00; 
    for(int i=0; i<=count; i++) {
        print_inf("[WAIT] ");
        double pct = 100.0 - ((double)((count-i))/((double)count)) * 100.0;

        printf("[%3.0f%%] %s| %s", 
               pct, 
               TERM_COLOR_LIGHTGRAY, 
               TERM_COLOR_LIGHTBLUE); 
        max = pb_width - ((double)((count-i))/((double)count)) * pb_width;
        max = max - 1;
        for(j=0; j < pb_width; j++) {
            if(j < max) buf[j] = '#';
            else buf[j] = ' ';
        }
        buf[(int)max] = '>';
        if((int)max == ((int)pb_width-1)) buf[(int)max] = '#';
        buf[(int)pb_width + 0] = 0x00;

        printf("%s%s |%s", buf, TERM_COLOR_LIGHTGRAY, TERM_DEFAULT);
        printf("\r"); flush_term(); 
        SDL_Delay(100);
    }
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
    delay(2000, 40);
    cursor_on(); flush_term(); 

    printf("\n");
    println_blu("READY.");

    SDL_Delay(300);

    return 0;
}

