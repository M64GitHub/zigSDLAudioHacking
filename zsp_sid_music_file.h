#ifdef __cplusplus
extern "C" {
#endif
#ifndef ZSP_SID_MUSIC_FILE_H
#define ZSP_SID_MUSIC_FILE_H

#include <stdio.h>

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


typedef struct sid_file_s {
    FILE *in;
    unsigned loadend;
    unsigned loadpos;
    unsigned loadsize;
    unsigned loadaddress;
    unsigned initaddress;
    unsigned playaddress;
    unsigned dataoffset;
    char *sidname;
} SID_FILE;

unsigned char readbyte(FILE *f);
unsigned short readword(FILE *f);

#endif

#ifdef __cplusplus
}
#endif
