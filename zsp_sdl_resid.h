#ifndef ZSP_SDL_RESID_H
#define ZSP_SDL_RESID_H

#include "zsp_sid.h"
//#include <SDL2/SDL.h>

class SDLreSID  
{
public:
	SDLreSID(void);
	void begin(void);
	void setSampleParameters(float clockfreq, float samplerate);
	inline void setreg(int ofs, int val) { sid.write(ofs, val); }
	inline uint8_t getreg(int ofs) { return sid.read(ofs); }
	void reset(void);
	void stop(void);
	inline bool isPlaying(void) { return playing; }	
private:
	cycle_count csdelta;
	volatile bool playing;
	virtual void update(void);	
	SID  sid;
	SID *sidptr;
};

#endif
