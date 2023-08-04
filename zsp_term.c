#include "zsp_term.h"
#include <stdio.h>

void print_inf(char *msg) {
    printf("%s[INF] %s\n", TERM_DEFAULT, msg);
}


void print_dbg(char *msg) {
    printf("%s[DBG] %s%s", TERM_COLOR_LIGHTGRAY, msg, TERM_DEFAULT);
}


void print_ok(char *msg) {
    printf("%s[OK!]%s %s", TERM_COLOR_LIGHTGREEN, TERM_DEFAULT, msg);
}

void print_err(char *msg) {
    printf("%s[DBG]%s %s", TERM_COLOR_LIGHTRED, TERM_DEFAULT, msg);
}


void println_inf(char *msg) {
    printf("%s[INF] %s\n", TERM_DEFAULT, msg);
}

void println_dbg(char *msg) {
    printf("%s[DBG] %s%s\n", TERM_COLOR_LIGHTGRAY, msg, TERM_DEFAULT);
}

void println_err(char *msg) {
    printf("%s[ERR]%s %s\n", TERM_COLOR_LIGHTRED, TERM_DEFAULT, msg);
}

void println_ok(char *msg) {
    printf("%s[OK!]%s %s\n", TERM_COLOR_LIGHTGREEN, TERM_DEFAULT, msg);
}

