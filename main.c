/*
	Dedicated to "M" Akesson, hinted at in the original video and YouTube comments.

	Rhombus: the emulator for LFT's FPGA demo "Parallelogram"
    Copyright (C) 2015 sirwinstoncat5 (GitHub username)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdint.h>
#define GPL_LICENSE_STRING "\n\nLicensed under the GNU GPL v3\nThis program comes with ABSOLUTELY NO WARRANTY; for details, see\n\tgithub.com/sirwinstoncat5/Rhombus/blob/master/LICENSE.txt.\nThis is free software, and you are welcome to redistribute it\nunder certain conditions; for details, see the license mentioned above.\n\n"
uint16_t fpga_mem[1048576];
//uint16_t cpu_cache[8192];
char* version="v0.0.4";
int main( int argc, char* args[] )
{
	RhombusGFXInit(800, 600);
	printf("Rhombus: the Parallelogram by LFT FPGA Demo Emulator %s\nCopyright (C) 2015 sirwinstoncat5 (GitHub username)\n%s\nPress Enter to continue...\n", version, GPL_LICENSE_STRING);
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
	RhombusDrawFrame(); //Note: call every 20 ms (2 music ticks) for 50 fps, every 16.6666666.... ms (1.6666666... music ticks) for 60 fps
	int i=0;
	for(;;)
	{
		for(i=0; i<630000; i++); //10 ms worth of CPU cycling
		{
			CPUcycle();
			//Update synth?
		}
		//Call audio routine
		//Wait until 10ms has actually passed
		//printf("\nping\n");
		for(i=0; i<630000; i++); //10 ms worth of CPU cycling
		{
			CPUcycle();
			//Update synth?
		}
		//Call audio routine
		//Wait until 10ms has actually passed
		//printf("\npong\n");
		RhombusDrawFrame();
	}
	RhombusGFXQuit();
}
