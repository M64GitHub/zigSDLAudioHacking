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
    
    setbuf(stdout, NULL); // turn off buffering
    printf("%s", zsp_logo_txt); 
    printf("%s                                                            "
           "  v00.00, M64%s\n",
           TERM_COLOR_LIGHTGRAY,
           TERM_DEFAULT
           );

    flush_term();
}

// delay w/ optional progress bar
void pb_delay(int ms, double pb_width) {
    char buf[256];
    int count = ms / 100;   // 100ms update frequency
    double current;         // current pct scaled to progressbar width
                            // (number of "fill chars" ('#')
    buf[0] = 0x00;          // init buf as empty string

    cursor_off(); flush_term();
    if(pb_width > 0)        // 0: dont't draw progress bar
    // print progress_bar
    for(int i=0; i<=count; i++) {
        double pct = 100.0 - ((count-i)/count) * 100.0;

        print_inf("[WAIT] ");
        printf("[%3.0f%%] %s| %s", 
               pct, 
               TERM_COLOR_LIGHTGRAY, 
               TERM_COLOR_LIGHTBLUE); 

        current = pb_width - (count-(double)i)/count * pb_width;
        current = current - 1;

        // draw progress bar into buf
        for(int j=0; j < pb_width; j++) {
            if(j < current) buf[j] = '#';
            else buf[j] = '-';
        }
        buf[(int)current] = '>';

        if((int) current == ((int)pb_width-1)) buf[(int)current] = '#';
        buf[(int)pb_width + 0] = 0x00; // terminate string

        printf("%s%s |%s", 
               buf, 
               TERM_COLOR_LIGHTGRAY, 
               TERM_DEFAULT);

        printf("\r"); flush_term(); 
        SDL_Delay(100); // 100ms update frequency
    } else SDL_Delay(ms);

    printf("\n"); cursor_on(); flush_term(); 
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

    SDL_Delay(300);

    return 0;
}

