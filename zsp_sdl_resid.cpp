#include "zsp_sdl_resid.h"

#define SAMPLERATE 44100
#define CLOCKFREQ 985248

// incredibly small buf!!! TODO: CHANGEME !! M64
#define AUDIO_BLOCK_SAMPLES  32

SDLreSID::SDLreSID(void) {

}

void SDLreSID::begin(void)
{
	// sidptr = &sid;
	this->reset();
	setSampleParameters(CLOCKFREQ, SAMPLERATE);
	playing = true;
}

void SDLreSID::setSampleParameters(float clockfreq, float samplerate) 
{
	sid.set_sampling_parameters(clockfreq, SAMPLE_FAST, samplerate); 
	csdelta = round((float)clockfreq / ((float)samplerate / AUDIO_BLOCK_SAMPLES));
}

void SDLreSID::reset(void)
{
	sid.reset();
}

void SDLreSID::stop(void)
{
	playing = false;	
}

void SDLreSID::update(void) 
{
	char *block = 0;

	// only update if we're playing
	if (!playing) return;

	// allocate the audio blocks to transmit
	// block = allocate();
	// if (block == NULL) return;
		
	//cycle_count delta_t = CLOCKFREQ / (SAMPLERATE / AUDIO_BLOCK_SAMPLES);
    cycle_count delta_t = csdelta;
    sidptr->clock(delta_t, (short int*)block, AUDIO_BLOCK_SAMPLES);

	// transmit(block);
	// release(block);
}
