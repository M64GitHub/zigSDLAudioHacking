// ------------------------------------------------------------------------------
// zigSIDplay                                                         2023, M64
//                                                    (afk aka Mario Schallner)
// ------------------------------------------------------------------------------
//
// supposed to become a SID player using reSID, SDL or sound.io,
// ... first in C (zig build), then rewrite in zig.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zigSIDplay.h"

// DEFAULTs, if not 0:

#define ARG_DEFAULT_SECONDS 60
#define ARG_DEFAULT_BASENOTE 0xB0
#define ARG_DEFAULT_OLDNOTEFACTOR 1

// -- helpers

void init_cmdline_args(CMDLINE_ARGS *args) {
  memset(args, 0, sizeof(CMDLINE_ARGS));
  args->seconds = ARG_DEFAULT_SECONDS;
  args->basenote = ARG_DEFAULT_BASENOTE;
  args->oldnotefactor = ARG_DEFAULT_OLDNOTEFACTOR;
}

// --

int main(int argc, char **argv) {
  CMDLINE_ARGS args;
  SID_FILE sidfile;

  init_cmdline_args(&args);

  printf("Hello Mario64World!\n");

  return 0;
}
