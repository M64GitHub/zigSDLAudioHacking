# -------------------------------------------------------------------------- #
#  zigsidplay - commandline sid music file player                  2023, M64 # 
#                                                  <m64.overdrive@gmail.com> #
# -------------------------------------------------------------------------- #

BINARY=zigsidplay
CC=zig c++
CXX=zig c++

CFLAGS+=-O3 -Wall
CXXFLAGS=$(CFLAGS)

$(BINARY): zigsidplay.o \
	zsp_cpu.o \
	zsp_sdl_audio.o  \
	zsp_term.o\
	zsp_sidfile.o \
	zsp_sid.o \
	zsp_sdl_resid.o 

	$(CXX) -o $@ $^ -lm -lSDL2 

clean:
	$(RM) *.o reSID/*.o $(BINARY)

.phony: clean

