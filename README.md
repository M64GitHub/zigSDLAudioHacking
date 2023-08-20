![Selection_439](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/ec96f9b3-2a13-46a8-9247-795f7389b329)
# zigSDLAudioHacking
hacking playground to try out the zig compiler, SDL audio, combinations of C/C++ code and zig. Planning a SID debugger / ".sid" music file player, and various related tools from scratch. 
The goal is to have handy commandline tools available, which would be great to have to assisting me in the audio creation creative work. Ways for inspection on various levels: the C64 cpu, SID registers, work with "editor files" (`.swm`, `.swi`) and so on.
First parts in C/C++, then rewrite as necessary in zig and use zig build.

## zigsidplay / siddbg
zigsidplay will be the first dropout of the experiments, an actual standalone `.sid` music file player, with various visualisation options, I am currently working on. When I reach a usable status with zig build, it will get it's own repository ... I added the logo already, just because I like it a lot ;]

siddbg shall become an advanced `.sid` music file player, that allows for displaying / visualizing details of what's going on in the player - routine - so: typical cpu debugger, and new, yet to come SID 'debug' features - like: register display / change on the fly, change filter, etc

swmdbg shall become a `.swm`, `.swi` file visualizer / dumper, probably editor and maybe even player for the commandline.

#### notes
C/C++-code is mainly for: 
 - testing the player concept, SDL interactions, in a language I am familiar with - before I start the zig adventure ...
 - creating an open component for the MOS 6510 CPU, so I can utilize it's features afterwards directly by importing and binding it to the zig code, and also re-use it in potential future C/C++ as well as zig projects.

Audio-related code allows easy replacement of `SDL2` by another audio-library (ie [`libsoundio`](https://github.com/andrewrk/libsoundio)).  

#### status
Currently the following parts work:
 - cpu in general (but gonna rewrite it anyways)
 - reSID code integration
 - SDL2 audio output
 - build with zig as C/C++ compiler
 - terminal handling w/ colors and animations
![image](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/b555e013-4b1c-4515-a7ef-99c37f08c0d9)

#### plans
 - work out the final siddbg concept:
 - rewrite the cpu-emulator in C++
 - add CPU debug features:
  - breakpoints
  - disassembly
  - reg / mem visualisation
  - ...
 - add SID dbg features:
  - SID registers display
  - ways to control the filter
  - log register changes
  - ...
 - create a whole bunch of tools / or a single one to
  - view / dump SID-Wizard `.swm` file contents
  - ...

## credits
 - the current cpu code groundwork was done by Lasse Öörni and Stein Pedersen in their little (but amazingly powerful and elegant) [`siddump`](https://github.com/cadaver/siddump)-utility. I have integrated some core code of it, and extended it heavily. 
 - the SID (*S*ound *I*nterface *D*evice of the Commodore 64) emulation is done by [`reSID`](https://github.com/daglem/reSID). I wrap around my own code to extend it's capabilities and connect it to the SDL-audio-subsystem.
 - [SDL](https://github.com/libsdl-org/SDL) for platform independent audio-output.
