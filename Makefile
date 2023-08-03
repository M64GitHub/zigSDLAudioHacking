# -------------------------------------------------------------------------- #
# zigSIDplay                                                       2023, M64 #
#                                                  (afk aka Mario Schallner) #
# -------------------------------------------------------------------------- #

BINARY=zigSIDplay
CC=zig cc
CXX=zig c++
CFLAGS+=-O3 -Wall
CXXFLAGS=$(CFLAGS)

$(BINARY): zigSIDplay.o zsp_cpu.o zsp_sid_music_file.o zsp_sdl_audio.o \
	reSID/envelope.o \
	reSID/extfilt.o \
	reSID/filter.o \
	reSID/pot.o \
	reSID/version.o \
	reSID/voice.o \
	reSID/wave8580__ST.o \
	reSID/wave8580_P_T.o \
	reSID/wave8580_PS_.o \
	reSID/wave8580_PST.o \
	reSID/wave.o

	$(CXX) -o $@ $^ -lm -lSDL2 

clean:
	$(RM) *.o reSID/*.o $(BINARY)

.phony: clean

