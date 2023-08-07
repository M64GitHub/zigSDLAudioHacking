#include "zsp_sdl_audio.h"
#include "zsp_term.h"
#include <SDL2/SDL.h>

#include "testsound_raw.h"

int sdl_audio_init(SDL_AudioDeviceID *id, 
                   SDL_AudioSpec     *spec,
                   int sampling_freq,
                   int num_channels,
                   int size_audiobuf) {
    // initialise SDL audio subsystem only
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        print_err("initializing SDL_AUDIO subsystem: ");
        printf("%s", SDL_GetError());
        return 2;
    }

    println_ok("SDL2 audio subsystem initialized");

    // configure audio device struct
    SDL_AudioSpec spec_in;
    memset(spec, 0, sizeof(SDL_AudioSpec));

    spec_in.freq      = sampling_freq;
    spec_in.format    = AUDIO_S8;         // 8 bit
    spec_in.channels  = num_channels;     // mono  / stereo
    spec_in.samples   = size_audiobuf;   // size in samples
    spec_in.userdata  = NULL;
    spec_in.callback  = NULL;             // we use SDL_QueueAudio

    // use new interface, let QueueAudio convert
    // *id = SDL_OpenAudioDevice(NULL, 0, spec, NULL, 0);
    *id = SDL_OpenAudioDevice(NULL, 0, &spec_in, spec, 0);

    if (*id < 1) {
        printf("[ERR] initializing audio device: %s", SDL_GetError());
        return 1;
    }
    print_ok("SDL2 audio device opened: id: ");
    printf("%d, bufsize_s: %d, bufsize_b: %d, freq: %d\n", 
           *id,
           spec->samples,
           spec->size,
           spec->freq);

    // audio devices default to being paused, so turn off pause
    SDL_PauseAudioDevice(*id, 0);
    SDL_Delay(050); //  

    return 0;
}

// -- tests, quickhack

char testbuf[44100 * 4 * 2]; // 4 sec, 1b, stereo
unsigned int testbuf_idx = 0;
void audio_generate_test_snd(float ampl, float freq) {
    float f; // general purpose float
    float PI = 3.141592;
   
    float t = 0;
    for(int i=0; i<(44100); i+=2){
        // f = ...

        // clipping
        if(f>ampl) f = ampl;
        if(f<(ampl * -1.0)) f = ampl * -1.0;

        testbuf[testbuf_idx + i+0] = f;
        testbuf[testbuf_idx + i+1] = f;
        // testbuf[i+2] = 0;
        // testbuf[i+3] = 0;
        testbuf_idx+=2;
    }
}

void audio_test(SDL_AudioDeviceID id) {
    println_inf("queuing audio ...");
    int err;

    // clear buffer
    for(int i=0; i<48000;i++) testbuf[i] = 0;

    testbuf_idx = 0;
    // for(int i=0; i<4; i++) {
    //     print_dbg("generating audio block #");
    //     // audio_generate_test_snd(60, 400 + i * 50);
    //     printf("%s%d ...%s", TERM_COLOR_LIGHTGRAY, i+1, TERM_DEFAULT);
    //     printf("\n");
    // }
    // printf("testbuf_idx: %d\n", testbuf_idx);
    // queueing
    println_inf("queuing audio buf with slices of test audio ...");
    // err=SDL_QueueAudio(id, testbuf, 79196);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 2);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 4);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 8);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 8);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 8);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 8);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 16);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 32);
    err=SDL_QueueAudio(id, TEST_raw, 79196 / 8);
    err=SDL_QueueAudio(id, TEST_raw, 79196);
    err=SDL_QueueAudio(id, TEST_raw, 79196);
    err=SDL_QueueAudio(id, TEST_raw, 79196);
    if(err) printf("[ERR] queuing audio: %s", SDL_GetError());
}

