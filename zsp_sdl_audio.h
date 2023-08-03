#ifndef ZSP_SDL_AUDIO_H
#define ZSP_SDL_AUDIO_H

#include <SDL2/SDL.h>

int init_sdl_audio(SDL_AudioDeviceID *id, 
                   SDL_AudioSpec     *spec,
                   int SAMPLING_FREQ,
                   int NUM_CHANNELS,
                   int SIZE_AUDIO_BUF);

void test_audio(SDL_AudioDeviceID id);

#endif

