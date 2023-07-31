# zigSIDplay                                                          2023, M64
#                                                       afk aka Mario Schallner

BINARY=zigSIDplay
CC=zig cc
CXX=zig c++
CFLAGS+=-O3 -Wall
CXXFLAGS=$(CFLAGS)

$(BINARY): zigSIDplay.o cpu.o
	$(CXX) -o $@ $^ -lm -lSDL2
#	echo $(CXX) -o $@ $^ -lm `sdl2-config --cflags --libs`

clean:
	$(RM) *.o $(BINARY)

.phony: clean

