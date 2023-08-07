#ifndef ZSP_SDL_AUDIO_H
#define ZSP_SDL_AUDIO_H
#ifdef __cplusplus
extern "C" {
#endif
#include <SDL2/SDL.h>

int sdl_audio_init(SDL_AudioDeviceID *id, 
                   SDL_AudioSpec     *spec,
                   int sampling_freq,
                   int num_channels,
                   int size_audiobuf);

void audio_test(SDL_AudioDeviceID id);

#endif
#ifdef __cplusplus
}
#endif
