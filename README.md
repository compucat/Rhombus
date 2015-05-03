# Rhombus
an emulator for LFT's "Parallelogram" FPGA demo

In his "Parallelogram" demo, Linus Akesson, aka LFT, used an FPGA to implement a custom CPU, a GPU composed of 10 shaders, a VGA backend to support the GPU, and an FM synthesizer to create a demo the likes of which are few and far between. The demo itself is packed with the FPGA configuration, embedded in the 16 KB CPU cache. If we could get a copy of this ROM, I'd like to attempt to create a platform so those without FPGAs (such as myself) can run the demo and develop their own.

This is also partly one of my own personal challenges: to write an emulator. It probably won't be the easiest thing in the world, but who cares? I'm going to try it.

This emulator is written in plain old C. I'm starting out by writing a CPU interpreter core for LFT's custom CPU. The CPU runs at 63 MHz and accesses RAM through a 16 KB cache. I'm doing away with the cache to simplify things-hopefully this won't mess up any techniques that rely on (mis)use of the cache. The demo binary will be preloaded into RAM instead of the cache as it is in hardware, but LFT's hardware eventually writes the binary back into RAM anyway. 

I'm not quite sure how I'm going to do video yet. I'm thinking that SDL might be the easiest way to do it if I can get my compiler to agree with me. As SDL can use OpenGL inside of it, I'm thinking about attempting to interpret LFT's shader code on the fly into GLSL (OpenGL Shader Language) and using the computer's GPU to run that.

I'll probably borrow someone else's FM synthesizer library to produce sound-it'll make things a bit easier.
