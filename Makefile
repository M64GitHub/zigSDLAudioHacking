# -------------------------------------------------------------------------- #
# zigSIDplay                                                       2023, M64 #
#                                                  (afk aka Mario Schallner) #
# -------------------------------------------------------------------------- #

BINARY=zigSIDplay
# CC=zig c++
# CXX=zig c++

CFLAGS+=-O3 -Wall
CXXFLAGS=$(CFLAGS)

$(BINARY): zigSIDplay.o \
	zsp_cpu.o \
	zsp_sdl_audio.o  \
	zsp_term.o\
	zsp_sid_music_file.o \
	zsp_sid.o \
	zsp_SDLreSID.o 

	$(CXX) -o $@ $^ -lm -lSDL2 

clean:
	$(RM) *.o reSID/*.o $(BINARY)

.phony: clean

