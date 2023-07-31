
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

int main(int argc, char **argv);
unsigned char readbyte(FILE *f);
unsigned short readword(FILE *f);

CHANNEL chn[3];
CHANNEL prevchn[3];
CHANNEL prevchn2[3];
FILTER filt;
FILTER prevfilt;

extern unsigned short pc;

