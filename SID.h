#ifndef SID_H
#define SID_H
#include <stdio.h>

#define MAX_INSTR 0x100000

typedef struct {
  unsigned short freq;
  unsigned short pulse;
  unsigned short adsr;
  unsigned char wave;
  int note;
} CHANNEL;

typedef struct {
  unsigned short cutoff;
  unsigned char ctrl;
  unsigned char type;
} FILTER;

unsigned char readbyte(FILE *f);
unsigned short readword(FILE *f);

#endif
