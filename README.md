![Selection_439](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/ec96f9b3-2a13-46a8-9247-795f7389b329)
# zigSDLAudioHacking
hacking playground to try out the zig compiler, SDL audio, combinations of C/C++ code and zig - by writing a SID music file player from scratch. First in C/C++, then rewrite in zig and use zig build.

## zigSIDplay
zigSIDplay is the actual `.sid` music file player I am currently working on. When I reach a usable status with zig build, it will get it's own repository ...  

![ZigSID - page 2](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/ca6cbd89-623d-40e6-9c62-2e3c9456f6e6)

#### notes
I do not plan a full implementation for the C/C++ version: like implementing a clean and full blown .sid file- and arguments-parser, logging infrastructure, terminal handling, tests, and features.  
This shall all be done in the final .zig version. C/C++-code is mainly for: 
 - testing the player concept, SDL interactions, in a language I am familiar with - before I start the conversion to zig.
 - creating an open component for the MOS 6510 CPU, so I can utilize it's features afterwards directly by importing and binding it to the zig code, and also re-use it in potential future C/C++ as well as zig projects.

Audio-related code allows easy replacement of `SDL2` by another audio-library (ie [`libsoundio`](https://github.com/andrewrk/libsoundio)).  
Once I can output a .sid file to the audio-device, and probably show a siddump-like output I will start the conversion from C to zig.

#### status
Currently the following parts work:
 - cpu in general
 - reSID code integration
 - SDL2 audio output
 - build with zig as C/C++ compiler
 - terminal handling w/ colors and animations
![image](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/2636d53f-ba9c-4d53-8941-aa52a0f1c834)

## credits
 - the cpu code groundwork was done by Lasse Öörni and Stein Pedersen in their little (but amazingly powerful and elegant) [`siddump`](https://github.com/cadaver/siddump)-utility. I have integrated the core code of it, and rewrite / extend it heavily. 
 - the SID (*S*ound *I*nterface *D*evice of the Commodore 64) emulation is done by [`reSID`](https://github.com/daglem/reSID). I wrap around my own code to extend it's capabilities and connect it to the SDL-audio-subsystem.
 - [SDL](https://github.com/libsdl-org/SDL) for platform independent audio-output.
