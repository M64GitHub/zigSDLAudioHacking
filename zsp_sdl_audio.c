#include "zsp_sdl_audio.h"
#include "zsp_term.h"
#include <SDL2/SDL.h>

int sdl_audio_init(SDL_AudioDeviceID *id, 
                   SDL_AudioSpec     *spec,
                   int SAMPLING_FREQ,
                   int NUM_CHANNELS,
                   int SIZE_AUDIO_BUF) {
    // initialise SDL audio subsystem only
    if (SDL_Init(SDL_INIT_AUDIO)) {
        print_err("initializing SDL_AUDIO subsystem: ");
        printf("%s", SDL_GetError());
        return 2;
    }

    println_ok("SDL2 audio subsystem initialized");

    // configure audio device struct
    memset(spec, 0, sizeof(SDL_AudioSpec));

    spec->freq      = SAMPLING_FREQ;
    spec->format    = AUDIO_S8;         // 8 bit
    spec->channels  = NUM_CHANNELS;     // mono  / stereo
    spec->samples   = SIZE_AUDIO_BUF;   // size in samples
    spec->userdata  = NULL;
    spec->callback  = NULL;             // we use SDL_QueueAudio

    // use new interface, let QueueAudio convert
    *id = SDL_OpenAudioDevice(NULL, 0, spec, NULL, 0);

    if (*id < 1) {
        printf("[ERR] initializing audio device: %s", SDL_GetError());
        return 1;
    }
    print_ok("SDL2 audio device opened: id:");
    printf("%d\n", *id);

    // audio devices default to being paused, so turn off pause
    SDL_PauseAudioDevice(*id, 0);

    return 0;
}


void audio_test(SDL_AudioDeviceID id) {
    char testbuf[48000]; // 1 sec
    float f;
    float PI = 3.141592;
   
    float ampl = 120.0;
    float freq = 400.0;
    for(int i=0; i<(48000 / 2); i++){
        f = ampl * sin((float)i/freq *2*PI);
        testbuf[i] = f;
        testbuf[i+1] = f;
        // if(i < freq) printf("f: %f (%d)\n",f, (char)f);
    }

    println_inf("Queuing audio ...");
    int err;

    for(int i=0; i<3; i++) {
        print_dbg("Queuing audio");
        printf("%s%d ...%s\n", TERM_COLOR_LIGHTGRAY, i, TERM_DEFAULT);
        err=SDL_QueueAudio(id, testbuf, 48000);
        if(err) printf("[ERR] queuing audio: %s", SDL_GetError());
    }
}

