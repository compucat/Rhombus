# Rhombus
an emulator for LFT's "Parallelogram" FPGA demo

In his "Parallelogram" demo, Linus Akesson, aka LFT, used an FPGA to implement a custom CPU, a GPU composed of 10 shaders, a VGA backend to support the GPU, and an FM synthesizer to create a demo the likes of which are few and far between. The demo itself is packed with the FPGA configuration, embedded in the 16 KB CPU cache. If we could get a copy of this ROM, I'd like to attempt to create a platform so those without FPGAs (such as myself) can run the demo and develop their own.

This is also partly one of my own personal challenges: to write an emulator. It probably won't be the easiest thing in the world, but who cares? I'm going to try it.

This emulator is written in plain old C. I'm starting out by writing a CPU interpreter core for LFT's custom CPU. The CPU runs at 63 MHz and accesses RAM through a 16 KB cache. I'm doing away with the cache to simplify things-hopefully this won't mess up any techniques that rely on (mis)use of the cache. The demo binary will be preloaded into RAM instead of the cache as it is in hardware, but LFT's hardware eventually writes the binary back into RAM anyway. 

I'm not quite sure how I'm going to do video yet. I'm thinking that SDL might be the easiest way to do it if I can get my compiler to agree with me. As SDL can use OpenGL inside of it, I'm thinking about attempting to interpret LFT's shader code on the fly into GLSL (OpenGL Shader Language) and using the computer's GPU to run that.

I'll probably borrow someone else's FM synthesizer library to produce sound-it'll make things a bit easier.

The code is being developed in Orwell Dev-C++, which creates a proprietary project file and autogenerated Makefile. It should still be possible to compile it on another system when set up correctly.

File listing:
- Makefile.win: autogenerated by Dev-C++
- README.md: you're reading it right now
- Rhombus.dev: Orwell Dev-C++ project file
- Rhombus.exe: Windows binary
- Rhombus.layout: autogenerated by Dev-C++
- cpu.c: Contains CPU interpreter core
- cpu.o: generated by gcc
- example.c: Dev-C++'s OpenGL example file, commented out
- example.o: Take a wild guess. :)
- main.c: Handles user I/O, cycles the CPU
- main.o: Do I really have to explain this again?
- notes.c: My personal annotations, commented out. Made as a ".c" file to allow it to show up in Dev-C++
- notes.o: A blank file-why does gcc insist on outputting ".o" files for completely commented out source files?
- rom.bin: A binary file containing a 16k ROM for the CPU. Currently contains whatever test code I happen to throw in there.

Note that LFT's demo is currently only available packed inside of his FPGA binary, effectively making it impossible to obtain. Eventually, unless LFT posts his source Verilog and demo on his website, I'll probably have to contact him for some more details. I'll want to test with custom test code for the majority of development, however, as his demo is compressed and prepended with decompression code-a nightmare to debug.
