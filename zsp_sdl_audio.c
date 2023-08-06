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
    print_ok("SDL2 audio device opened: id: ");
    printf("%d\n", *id);

    // audio devices default to being paused, so turn off pause
    SDL_PauseAudioDevice(*id, 0);

    return 0;
}

// -- tests, quickhack

char testbuf[48000]; // 1 sec
void audio_generate_test_snd(float ampl, float freq) {
    float f; // general purpose float
    float PI = 3.141592;
   
    for(int i=0; i<(48000 / 4); i+=4){
        f = ampl * sin((float)i/freq *2*PI);
        if(f>ampl) f = ampl;
        if(f<0) f = 0;
        testbuf[i+0] = f;
        testbuf[i+1] = f;
        testbuf[i+2] = 0;
        testbuf[i+3] = 0;
    }
}

void audio_test(SDL_AudioDeviceID id) {
    println_inf("queuing audio ...");
    int err;
    for(int i=0; i<4; i++) {
        print_dbg("generating and qeueueing audio block #");
        audio_generate_test_snd(120, 400 + i * 50);
        printf("%s%d ...%s", TERM_COLOR_LIGHTGRAY, i+1, TERM_DEFAULT);
        printf("\n");

        err=SDL_QueueAudio(id, testbuf, 48000);
        if(err) printf("[ERR] queuing audio: %s", SDL_GetError());
    }
}

