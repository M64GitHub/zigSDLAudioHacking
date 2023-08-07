
#include "zsp_term.h"
#include <stdio.h>
#include <SDL2/SDL.h>

// -- print

void print_inf(char *msg) {
    printf("%s[INF] %s", TERM_DEFAULT, msg);
}

void print_dbg(char *msg) {
    printf("%s[DBG] %s%s", TERM_COLOR_LIGHTGRAY, msg, TERM_DEFAULT);
}

void print_ok(char *msg) {
    printf("%s[OK!] %s%s", TERM_COLOR_LIGHTGREEN, msg, TERM_DEFAULT);
}

void print_err(char *msg) {
    printf("%s[DBG] %s%s", TERM_COLOR_LIGHTRED, msg, TERM_DEFAULT);
}

// -- println

void println_inf(char *msg) {
    printf("%s[INF] %s\n", TERM_DEFAULT, msg);
}

void println_dbg(char *msg) {
    printf("%s[DBG] %s%s\n", TERM_COLOR_LIGHTGRAY, msg, TERM_DEFAULT);
}

void println_err(char *msg) {
    printf("%s[ERR] %s%s\n", TERM_COLOR_LIGHTRED, msg, TERM_DEFAULT);
}

void println_ok(char *msg) {
    printf("%s[OK!] %s%s\n", TERM_COLOR_LIGHTGREEN, msg, TERM_DEFAULT);
}

void println_blu(char *msg) {
    printf("%s[---] %s%s\n", TERM_COLOR_LIGHTBLUE, msg, TERM_DEFAULT);
}

void cursor_off() {
    printf("%s", TERM_CURSOR_OFF);
}

void cursor_on() {
    printf("%s", TERM_CURSOR_ON);
}

void flush_term() 
{
    fflush(stdout);
}

// --
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
        double pct = 100.0 - ((count-(double)i)/count) * 100.0;

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


