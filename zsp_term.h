#ifndef ZSP_TERM_H
#define ZSP_TERM_H

#define TERM_DEFAULT            "\x1b[0m"
#define TERM_COLOR_LIGHTGRAY    "\x1b[90m"
#define TERM_COLOR_LIGHTRED     "\x1b[91m"
#define TERM_COLOR_LIGHTGREEN   "\x1b[92m"
#define TERM_COLOR_LIGHTYELLOW  "\x1b[93m"

void print_inf(char *msg);
void print_dbg(char *msg);
void print_ok(char *msg);
void print_err(char *msg);

void println_inf(char *msg);
void println_dbg(char *msg);
void println_ok(char *msg);
void println_err(char *msg);

#endif

