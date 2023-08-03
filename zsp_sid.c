#include "zsp_sid.h"

#include "reSID/envelope.cc"
#include "reSID/extfilt.cc"
#include "reSID/filter.cc"
#include "reSID/pot.cc"
#include "reSID/version.cc"
#include "reSID/voice.cc"

#include "reSID/wave8580__ST.cc" 
#include "reSID/wave8580_P_T.cc"
#include "reSID/wave8580_PS_.cc"
#include "reSID/wave8580_PST.cc"

#include "reSID/wave.cc"
#include "reSID/sid.cc"

// --

CHANNEL chn[3];
CHANNEL prevchn[3];
CHANNEL prevchn2[3];
FILTER filt;
FILTER prevfilt;

// --

int sid_init() { return 0; }
