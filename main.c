/*
	Name: Rhombus
	Copyright: May 2015
	Author: sirwinstoncat5
	Date: 01/05/15 21:17
	Description: An emulator for Linus "lft" Akesson's "Parallelogram" FPGA demo.
	
	Dedicated to "M" Akesson, hinted at in the original video and YouTube comments.
*/

#include <stdio.h>
#include <stdint.h>
uint16_t fpga_mem[1048576];
//uint16_t cpu_cache[8192];
char* version="v0.0.2";
int main( int argc, char* args[] )
{
	RhombusGFXInit(320, 240);
	printf("Rhombus: the Parallelogram by LFT FPGA Demo Emulator %s (c) sirwinstoncat5\nPress Enter to continue...\n", version);
	//Load ROM binary into first 16k of RAM
	FILE* demoROM=fopen("rom.bin", "rb");
	if(demoROM==NULL)
	{
		printf("Error: ROM failed to load. Does the file exist?\n");
		return;
	}
	printf("%d bytes loaded from \"rom.bin\".\n", 2*fread(fpga_mem, sizeof(uint16_t), 8192, demoROM));
	fclose(demoROM);
	//getchar();
	//ROMList();
	printf("Ready to run.\n");
	sleep(1);
	//getchar();
	RhombusDrawFrame();
	for(;;) CPUcycle();
	RhombusGFXQuit();
}
