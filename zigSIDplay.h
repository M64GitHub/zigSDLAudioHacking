// zigSIDplay, 2023, M64

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
  int oldnotefactor;
  int timeseconds;
  int usage;
  int profiling;
} CMDLINE_ARGS;

typedef struct sid_file_s {
  unsigned loadend;
  unsigned loadpos;
  unsigned loadsize;
  unsigned loadaddress;
  unsigned initaddress;
  unsigned playaddress;
  unsigned dataoffset;
  FILE *in;
  char *sidname;
} SID_FILE;

void init_cmdline_args(CMDLINE_ARGS *args);
int init_sdl();

#endif
