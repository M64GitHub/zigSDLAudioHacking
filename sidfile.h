#ifndef SIDFILE_H
#define SIDFILE_H

#include <stdio.h>

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

