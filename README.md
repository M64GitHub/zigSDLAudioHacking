![Selection_439](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/ec96f9b3-2a13-46a8-9247-795f7389b329)
# zigSDLAudioHacking
hacking playground to try out the zig compiler, SDL audio, combinations of C/C++ code and zig - by writing a SID music file player from scratch (reSID for SID soundchip emulation). First in C, then rewrite in zig and use zig build.

## zigSIDplay
zigSIDplay is the actual `.sid` music file player I am currently working on. When I reach a usable status with zig build, it will get it's own repository ...  

![ZigSID - page 2](https://github.com/M64GitHub/zigSDLAudioHacking/assets/84202356/ca6cbd89-623d-40e6-9c62-2e3c9456f6e6)

#### note
I won't go into a full implementation for the C version: like implementing a clean and full blown arguments parser, logging infrastructure, terminal handling, tests, and features.  
This shall all be done in the final .zig version. C-code is mainly for testing the player concept, SDL interactions, in a language I am familiar with - before I start the conversion to zig.  
Once I can output a .sid file to the audio-device, and probably show a siddump-like output I will start the conversion.
