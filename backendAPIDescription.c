/*
***Rhombus Backend API Description***
-------------------------------------

Greetings, fellow API developer! Here's a description of what you need to do to write a new backend for Rhombus.

The official Parallelogram spec (as published by LFT) is available at:
	http://www.linusakesson.net/scene/parallelogram/index.php

***GENERAL***
-------------
Really, once you write an API, all you need to do is link the .c file in during compilation instead of one of the stock backends.

Please note that I originally intended only the graphics and sound (and possibly blitter) to be a
replaceable backend, so some of the rest might not be as well documented or modular. Even the
graphics backend has quite a bit of other code in it that SDL had handy functions for.

***GRAPHICS***
--------------
The graphics backend handles...well, graphics. Namely, graphics output. It also
handles other functions that I found SDL was useful for when I wrote the stock backend.

The stock graphics backend uses SDL 1.2.

Function list:
void RhombusGFXInit(int scr_height, int scr_width); 
	Initializes your backend with the given resolution. Display a splash screen, if you like.
void RhombusGFXQuit(void); 
	Perform any clean-up work your backend requires (such as freeing memory, etc.)
void RhombusDrawFrame(void); 
	Access the emulated 2 megabyte RAM and read one of three framebuffers.
	One of the CPU output ports (see notes.c) is set to either 1, 2, or 3 to indicate the active framebuffer.
	See LFT's Parallelogram page for a memory map and the address of each framebuffer.

I haven't finished writing this yet-this may/will change as I improve the backend.

***CPU***
---------
The CPU is a custom 16-bit design by LFT with 52 instructions and 4 addressing modes, documented in "notes.c".
It runs at 63 MHz, cycled by "main.c". The original hardware had a 16 KB cache, but I've gotten rid of this to simplify
memory management. (The shaders would also have to go through the cache, making things quite complex.)

The stock CPU backend is a simple interpreter.

Function list:
inline void CPUcycle(void);
	This performs one cycle of the CPU-getting an instruction, running it, incrementing the program counter, etc.
void CPUinit(void);
	Initializes the program counter, stack pointer, etc.
void ROMList(void);
	This lists the 16 KB ROM in hexadecimal using printf. Created for testing purposes.

I haven't finished writing this yet-this may/will change as I improve the backend.

***SOUND***
-----------
Sound is handled by a "16-channel, 4-op FM synthesizer with resonant low-pass filters on each channel and a global echo facility". (LFT's page)
For technical details, see (as always) LFT's page.

I haven't wrote a stock sound backend yet.

I haven't finished writing this yet-this may/will change as I improve the backend.

***BLITTER***
-------------
The blitter is composed of 10 DSP-like shader cores that are the main meat of the graphics rendering process. Each takes some custom shader code
written by the CPU into memory and outputs a pixel into a framebuffer. See LFT's page for the instruction set, etc.

I haven't wrote a stock blitter backend yet.

I haven't finished writing this yet-this may/will change as I improve the backend.

*/
