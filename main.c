/*
	Name: Rhombus v0.0.1
	Copyright: May 2015
	Author: sirwinstoncat5
	Date: 01/05/15 21:17
	Description: An emulator for LFT's "Parallelogram" FPGA demo.
*/

#include <stdio.h>
#include <stdint.h>
uint8_t fpga_mem[2097152];
//uint16_t cpu_cache[8192];
main()
{
	printf("Rhombus: the Parallelogram by LFT FPGA Demo Emulator v0.0.1 (c) sirwinstoncat5\nPress Enter to continue...\n");
	//Load ROM binary into first 16k of RAM
	FILE* demoROM=fopen("rom.bin", "rb");
	if(demoROM==NULL)
	{
		printf("Error: ROM failed to load. Does the file exist?\n");
		return;
	}
	printf("%d words loaded from \"rom.bin\".\n", fread(fpga_mem, sizeof(uint16_t), 8192, demoROM));
	//getchar();
	//ROMList();
	printf("Ready to run.\n");
	getchar();
	for(;;) CPUcycle();
}
