#ifdef __cplusplus
extern "C" {
#endif

#ifndef ZSP_TERM_H
#define ZSP_TERM_H

#define TERM_DEFAULT            "\x1b[0m"
#define TERM_COLOR_LIGHTGRAY    "\x1b[90m"
#define TERM_COLOR_LIGHTRED     "\x1b[91m"
#define TERM_COLOR_LIGHTGREEN   "\x1b[92m"
#define TERM_COLOR_LIGHTYELLOW  "\x1b[93m"
#define TERM_COLOR_LIGHTBLUE    "\x1b[94m"
#define TERM_COLOR_LIGHTWHITE   "\x1b[97m"

#define TERM_CURSOR_OFF         "\x1b[?25l"
#define TERM_CURSOR_ON          "\x1b[?25h"

void term_init();

void print_inf(char *msg);
void print_dbg(char *msg);
void print_ok(char *msg);
void print_err(char *msg);
void println_blu(char *msg);

void println_inf(char *msg);
void println_dbg(char *msg);
void println_ok(char *msg);
void println_err(char *msg);

void pb_delay(int ms, double pb_width);

void cursor_on();
void cursor_off();
void flush_term();

#endif

#ifdef __cplusplus
}
#endif

