#include "zsp_sdl_audio.h"
#include "zsp_term.h"
#include <SDL2/SDL.h>

#include "testsound_raw.h"
unsigned int wav_pos = 0;

int PLAYING = 0;

static void audio_callback(void *userdata, uint8_t *stream, int len) {
    if(!PLAYING) return;

    memset(stream, 0, len);

}

int sdl_audio_init(SDL_AudioDeviceID *id, 
                   SDL_AudioSpec     *spec,
                   int sampling_freq,
                   int num_channels,
                   int size_audiobuf) {
    wav_pos = 0;

    // initialise SDL audio subsystem only
    if (SDL_Init(SDL_INIT_AUDIO)) {
        print_err("initializing SDL_AUDIO subsystem: ");
        printf("%s\n", SDL_GetError());
        return 2;
    }

    println_ok("[AUDIO] SDL2 audio subsystem initialized");

    // configure audio device struct
    SDL_AudioSpec spec_in;
    memset(spec, 0, sizeof(SDL_AudioSpec));

    spec_in.freq      = sampling_freq;
    spec_in.format    = AUDIO_S8;       // 8 bit
    spec_in.channels  = num_channels;   // mono  / stereo
    spec_in.samples   = size_audiobuf;  // size in samples
    spec_in.userdata  = NULL;
    spec_in.callback  = audio_callback;

    *id = SDL_OpenAudioDevice(NULL, 0, &spec_in, spec, 0);

    if (*id < 1) {
        printf("[ERR] initializing audio device: %s", SDL_GetError());
        return 1;
    }
    print_ok("[AUDIO] SDL2 audio device opened: id: ");
    printf("%s%d%s\n", TERM_COLOR_LIGHTWHITE, *id, TERM_DEFAULT);
    
    print_dbg("[AUDIO] audio spec: ");
    printf("%sbufsize_s: %d, bufsize_b: %d, freq: %d%s\n", 
           TERM_COLOR_LIGHTGRAY,
           spec->samples,
           spec->size,
           spec->freq,
           TERM_DEFAULT);

    // audio devices default to being paused, so turn off pause
    SDL_PauseAudioDevice(*id, 0);
    SDL_Delay(050); //  

    return 0;
}


