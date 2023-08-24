#ifndef ZSP_SDL_AUDIO_H
#define ZSP_SDL_AUDIO_H
#ifdef __cplusplus
extern "C" {
#endif
#include <SDL2/SDL.h>

extern int PL_PLAYING;
extern int PL_SAMPLES_TOTAL;
extern int PL_SAMPLES_CURRENT;

int sdl_audio_init(SDL_AudioDeviceID *id, 
                   SDL_AudioSpec     *spec,
                   int sampling_freq,
                   int num_channels,
                   int size_audiobuf);

void sdl_audio_play();

#ifdef __cplusplus
}
#endif

#endif
