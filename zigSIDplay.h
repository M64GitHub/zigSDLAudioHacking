#ifndef ZIGSIDPLAY_H
#define ZIGSIDPLAY_H

#include <stdio.h>

typedef struct cmdline_args_s {
    int subtune;
    int seconds;
    int instr;
    int frames;
    int spacing;
    int pattspacing;
    int firstframe;
    int counter;
    int basefreq;
    int basenote;
    int lowres;
    int rows;
    int timeseconds;
    int usage;
    int profiling;
} CMDLINE_ARGS;

void init_cmdline_args(CMDLINE_ARGS *args);

void print_header();

#endif
