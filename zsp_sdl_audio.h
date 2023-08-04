#ifndef ZSP_SDL_AUDIO_H
#define ZSP_SDL_AUDIO_H

#include <SDL2/SDL.h>

int sdl_audio_init(SDL_AudioDeviceID *id, 
                   SDL_AudioSpec     *spec,
                   int SAMPLING_FREQ,
                   int NUM_CHANNELS,
                   int SIZE_AUDIO_BUF);

void audio_test(SDL_AudioDeviceID id);

#endif

