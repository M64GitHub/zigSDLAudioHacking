![Selection_439](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/ec96f9b3-2a13-46a8-9247-795f7389b329)
# zigSDLAudioHacking
hacking playground to try out the zig compiler, SDL audio, combinations of C/C++ code and zig - by writing a SID music file player from scratch (reSID for SID soundchip emulation). First in C, then rewrite in zig and use zig build.

## zigSIDplay
zigSIDplay is the actual `.sid` music file player I am currently working on. When I reach a usable status with zig build, it will get it's own repository ...  

![ZigSID - page 2](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/ca6cbd89-623d-40e6-9c62-2e3c9456f6e6)

#### notes
I won't go into a full implementation for the C version: like implementing a clean and full blown arguments parser, logging infrastructure, terminal handling, tests, and features.  
This shall all be done in the final .zig version. C-code is mainly for testing the player concept, SDL interactions, in a language I am familiar with - before I start the conversion to zig.  
Once I can output a .sid file to the audio-device, and probably show a siddump-like output I will start the conversion.

Audio-related code is intentionally prepared / abstracted to allow easy replacement of `SDL2` by another audio-library (ie [`SoundIO`](https://github.com/cadaver/siddump)).

### status
Currently the following parts work:
 - cpu in general
 - reSID code integration
 - SDL2 audio output
 - terminal handling w/ colors and animations
   
## credits
 - the cpu code groundwork was done by Lasse Oorni and Stein Pedersen in their amazing little (but powerful) [`siddump`](https://github.com/cadaver/siddump)-utility. I have integrated the core code of it, and rewrite / extend it heavily. 
 - the SID (*S*ound *I*nterface *D*evice of the Commodore 64) emulation is done by reSID, where I wrap around my own code and connect it to the SDL-audio-subsystem.
 - [SDL](https://github.com/libsdl-org/SDL) for audio-output
