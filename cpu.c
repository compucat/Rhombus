/*
	CPU Interpreter Backend
	
	This file is part of Rhombus: the emulator for LFT's FPGA demo "Parallelogram".
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
//CPU runs at 63 MHz?
#include <stdio.h>
#include <stdint.h>
#define CPU_SWITCH_DEBUG 0
#define BINARY_BIG_ENDIAN 1 //Switches the endian-ness of the instruction on-the-fly, may or may not be needed.
//Determine if this is needed experimentally.
//*****Begin definitions/prototypes*****

extern uint16_t fpga_mem[1048576]; 
uint16_t GPreg[8], PC, SP;
int zeroFlag, carryFlag;
uint16_t outPort[8], inPort[8];
//Input ports (0-5):
//	productLow, productHigh, status flags, uart receive, frame counter, benchmark timer
//Output ports (0-7):
//	blitter row, blitter col, blitter width, blitter height (write starts blit), blitter program, active vid page (1-3), synth register select, synth register data
inline uint16_t CPUReadMemory(uint16_t address);
inline void CPUWriteMemory(uint16_t address, uint16_t value);

//CPU cycle variables
uint16_t instruction, opgroup, opcodea, opcodeb, opcodec, opera1, opera2, opera3, operb, operc1, operc2, operc3;

//Test functions
void ROMList(void) //Lists the ROM in hex
{
	int i;
	for(i=0; i<8192; i++)
	{
		printf("%x\n", CPUReadMemory(i));
	}
}

//Main CPU functions
void CPUinit(void);
inline void CPUcycle(void);

//Opcode function prototypes
void OpcodeNotDone(void) {printf("CPU Error: Opcode not written yet!\n");}
inline void movih(void); //done
inline void add(void); //done
inline void addi(void); //done
inline void adc(void); //done
inline void adci(void); //done
inline void sub(void); //done
inline void subi(void); //done
inline void and_inst(void); //done
inline void andi(void); //done
inline void or_inst(void); //done
inline void ori(void); //done
inline void xor_inst(void); //done
inline void xori(void); //done
inline void cmp(void); //done
inline void cmpi(void); //done
inline void mov(void); //done
inline void movi(void); //done
inline void bgt(void);
inline void bne(void); //done
inline void bcc(void); //done
inline void bcs(void); //done
inline void beq(void); //done
inline void ble(void);
inline void bal(void); //done
//Call instructions rely on branch instructions
inline void cgt(void); //done
inline void cne(void); //done
inline void ccc(void); //done
inline void ccs(void); //done
inline void ceq(void); //done
inline void cle(void); //done
inline void cal(void); //done
//End call instructions
inline void ld(void); //done   //Assuming opera2 is an immediate value, opera3 refers to a register, and opera2 is unsigned
inline void st(void); //done   //Assuming opera2 is an immediate value, opera3 refers to a register, and opera2 is unsigned
inline void in(void); //done
inline void out(void); //done
inline void jv(void);
inline void cv(void); //done, relies on jv
inline void lea(void);
inline void push(void); //done
inline void pop(void); //done
inline void nop(void); //done
inline void mul(void); //done
inline void stsp(void); //done
inline void prod(void);
inline void jr(void); //done
inline void cr(void); //done
inline void ret(void); //done
inline void wait(void);
inline void send(void); //done
inline void ldsf(void);
inline void stsf(void);
inline void initv(void);

//*****Begin functions*****

inline uint16_t CPUReadMemory(uint16_t address)
{
	return fpga_mem[address];
}
inline void CPUWriteMemory(uint16_t address, uint16_t value)
{
	fpga_mem[address]=value;
}

void CPUinit(void)
{
	PC=0;
	SP=65535;
	//ROM is loaded by main.c
}

inline void CPUcycle(void)
{
	if(CPU_SWITCH_DEBUG==1) printf("CPU Cycle! PC=%d\n", PC);
	instruction=CPUReadMemory(PC++); //Increment PC after accessing it
	//*********NOTE********* This should be big-endian.
	if(BINARY_BIG_ENDIAN==1) instruction=(instruction>>8)+(instruction<<8);
	if(CPU_SWITCH_DEBUG==1) printf("Instruction in hex: %x\n", instruction);
	//Decode instruction into opcode and operands
	//Note that depending on addressing mode, some operands may be garbage
	opgroup=instruction>>14;
	opcodea=instruction>>11;
	opcodeb=instruction>>9;
	opcodec=opcodea;
	//Addressing mode A, used for most instructions
	opera1=(instruction>>8)-(opcodea<<3);
	opera2=(instruction>>3)-(opcodea<<8)-(opera1<<5);
	opera3=instruction-(opcodea<<11)-(opera1<<8)-(opera2<<3);
	//Addressing mode B, used for branches and subroutine calls
	operb=instruction-(opcodeb<<9);
	//Addressing mode C, used for vector jump/calls and load effective address
	operc1=(instruction>>8)-(opcodec<<3);
	operc2=(instruction>>7)-(opcodec<<4)-(operc1<<1);
	operc3=instruction-(opcodec<<11)-(operc1<<8)-(operc2<<7);
	
	if(CPU_SWITCH_DEBUG==1) printf("OpcodeA: %d\nOperA1: %d\nOperA2: %d\nOperA3: %d\n----------------------\n", opcodea, opera1, opera2, opera3);
	//Determine addressing mode
	switch(opgroup)
	{
		case 0b10: 
			switch(opcodeb) //Addressing mode B, branches/subroutine calls
			{
				case 0b1000001:
					bgt();
					break;
				case 0b1000011: 
					bne();
					break;
				case 0b1000101: 
					bcc();
					break;
				case 0b1001010: 
					bcs();
					break;
				case 0b1001100: 
					beq();
					break;
				case 0b1001110: 
					ble();
					break;
				case 0b1001111: 
					bal();
					break;
				case 0b1010001:
					cgt();
					break;
				case 0b1010011: 
					cne();
					break;
				case 0b1010101: 
					ccc();
					break;
				case 0b1011010: 
					ccs();
					break;
				case 0b1011100: 
					ceq();
					break;
				case 0b1011110: 
					cle();
					break;
				case 0b1011111: 
					cal();
					break;  
				default: 
					OpcodeNotDone();
			}
			break;
		case 0b00:
			movih(); //Movih has its own opgroup, addressing mode A
			break;
		case 0b01:
			modeA: //Addressing mode A...
			if(CPU_SWITCH_DEBUG==1) printf("Mode A!\n");
			switch(opcodea)
			{
				case 0b01000: 
					if(opera2==0b00000) add();
					else addi();
					break;
				case 0b01001: 
					if(opera2==0b00000) adc();
					else adci();
					break;
				case 0b01010: 
					if(opera2==0b00000) sub();
					else subi();
					break;
				case 0b01011: 
					if(opera2==0b00000) and_inst();
					else andi();
					break;
				case 0b01100: 
					if(opera2==0b00000) or_inst();
					else ori();
					break;
				case 0b01101: 
					if(opera2==0b00000) xor_inst();
					else xori();
					break;
				case 0b01110: 
					if(opera2==0b00000) cmp();
					else cmpi();
					break;
				case 0b01111: 
					if(opera2==0b00000) mov();
					else movi();
					break;
				case 0b11000: 
					ld();
					break;
				case 0b11001: 
					st();
					break;
				case 0b11010: 
					in();
					break;
				case 0b11011: 
					out();
					break;
				case 0b11100: 
					jv();
					break;
				case 0b11101:
					if(operc2==1) lea();
					else cv();
					break;
				case 0b11111: 
					if(CPU_SWITCH_DEBUG==1) printf("Other Instructions!\n");
					switch(opera2)
					{
						case 0b00000: 
							push();
							break;
						case 0b00001: 
							pop();
							break;
						case 0b00010: 
							nop();
							break;
						case 0b00011: 
							mul();
							break;
						case 0b00100: 
							stsp();
							break;
						case 0b00101: 
							prod();
							break;
						case 0b00110: 
							jr();
							break;
						case 0b00111: 
							cr();
							break;
						case 0b01000: 
							ret();
							break;
						case 0b01001: 
							wait();
							break;
						case 0b01010: 
							send();
							break;
						case 0b01011: 
							ldsf();
							break;
						case 0b01100: 
							stsf();
							break;
						case 0b01101: 
							initv();
							break;
						default: OpcodeNotDone();
					}
					break;
				default: OpcodeNotDone();
			}
			break; 
		case 0b11: //...or mode C?
			goto modeA;
			break;
		default: 
			printf("CPU Error: unimplemented opgroup!\n");
			break;
	}
	if(CPU_SWITCH_DEBUG==1) getchar();
}

//Opcode functions
//Arithmetic, etc.
inline void movih(void)
{
	if(CPU_SWITCH_DEBUG==1) printf("Movih\n");
	GPreg[opera1]=(opcodea-(opgroup<<3))+(opera2<<3)+opera3;
}
inline void add(void) 
{
	int i=GPreg[opera1]+GPreg[opera3];
	if(i>65535) carryFlag=1;
	else carryFlag=0;
	GPreg[opera1]+=GPreg[opera3];
}
inline void addi(void)
{
	int i=GPreg[opera1]+(opera3+(opera2<<3)-0b10000000);
	if(i>65535) carryFlag=1;
	else carryFlag=0;
	GPreg[opera1]+=(opera3+(opera2<<3)-0b10000000);
}
inline void adc(void)
{
	int i=GPreg[opera1]+GPreg[opera3];
	if(carryFlag==1) i++;
	GPreg[opera1]+=GPreg[opera3];
	if(carryFlag==1) GPreg[opera1]++;
	if(i>65535) carryFlag=1;
	else carryFlag=0;
}
inline void adci(void)
{
	int i=GPreg[opera1]+(opera3+(opera2<<3)-0b10000000);
	if(carryFlag==1) i++;
	GPreg[opera1]+=(opera3+(opera2<<3)-0b10000000);
	if(carryFlag==1) GPreg[opera1]++;
	if(i>65535) carryFlag=1;
	else carryFlag=0;
}
inline void sub(void)
{
	GPreg[opera1]-=GPreg[opera3];
}
inline void subi(void)
{
	GPreg[opera1]-=(opera3+(opera2<<3)-0b10000000);
}
inline void and_inst(void)
{
	GPreg[opera1]=GPreg[opera1]&GPreg[opera3];
}
inline void andi(void)
{
	GPreg[opera1]=GPreg[opera1]&(opera3+(opera2<<3)-(0b10000000));
}
inline void or_inst(void)
{
	GPreg[opera1]=GPreg[opera1]|GPreg[opera3];
}
inline void ori(void)
{
	GPreg[opera1]=GPreg[opera1]|(opera3+(opera2<<3)-(0b10000000));
}
inline void xor_inst(void)
{
	GPreg[opera1]=GPreg[opera1]^GPreg[opera3];
}
inline void xori(void)
{
	GPreg[opera1]=GPreg[opera1]^(opera3+(opera2<<3)-(0b10000000));
}
inline void cmp(void)
{
	if(GPreg[opera1]==GPreg[opera3]) zeroFlag=1;
	else zeroFlag=0;
}
inline void cmpi(void)
{
	if(GPreg[opera1]==(opera3+(opera2<<3)-(0b10000000))) zeroFlag=1;
	else zeroFlag=0;
}
inline void mov(void)
{
	GPreg[opera1]=GPreg[opera3];
}
inline void movi(void)
{
	GPreg[opera1]=(opera3+(opera2<<3)-(0b10000000));
}
//Branches
inline void bgt(void) {OpcodeNotDone();}
inline void bne(void)
{
	if(zeroFlag==0)
	{
		int i;
		if((operb>>8)==1) i=0-(operb-0b100000000); //Negative signed number
		else i=(operb-0b100000000); //Positive signed number
		PC+=i;
	}
}
inline void bcc(void)
{
	if(carryFlag==0)
	{
		int i;
		if((operb>>8)==1) i=0-(operb-0b100000000); //Negative signed number
		else i=(operb-0b100000000); //Positive signed number
		PC+=i;
	}
}
inline void bcs(void)
{
	if(carryFlag==1)
	{
		int i;
		if((operb>>8)==1) i=0-(operb-0b100000000); //Negative signed number
		else i=(operb-0b100000000); //Positive signed number
		PC+=i;
	}
}
inline void beq(void)
{
	if(zeroFlag==1)
	{
		int i;
		if((operb>>8)==1) i=0-(operb-0b100000000); //Negative signed number
		else i=(operb-0b100000000); //Positive signed number
		PC+=i;
	}
}
inline void ble(void) {OpcodeNotDone();}
inline void bal(void)
{
	int i;
	if((operb>>8)==1) i=0-(operb-0b100000000); //Negative signed number
	else i=(operb-0b100000000); //Positive signed number
	PC+=i;
}
//Subroutine calls
inline void cgt(void)
{
	CPUWriteMemory(PC, SP--);
	bgt;
}
inline void cne(void)
{
	CPUWriteMemory(PC, SP--);
	bne;
}
inline void ccc(void)
{
	CPUWriteMemory(PC, SP--);
	bcc;
}
inline void ccs(void)
{
	CPUWriteMemory(PC, SP--);
	bcs;
}
inline void ceq(void)
{
	CPUWriteMemory(PC, SP--);
	beq;
}
inline void cle(void)
{
	CPUWriteMemory(PC, SP--);
	ble;
}
inline void cal(void)
{
	CPUWriteMemory(PC, SP--);
	bal;
}
//Memory
inline void ld(void) //Assuming opera2 is an immediate value, opera3 refers to a register, and opera2 is unsigned
{
	GPreg[opera1]=CPUReadMemory(GPreg[opera3]+opera2);
}
inline void st(void) //Assuming opera2 is an immediate value, opera3 refers to a register, and opera2 is unsigned
{
	CPUWriteMemory(GPreg[opera1], GPreg[opera3]+opera2);
}
//IO
inline void in(void)
{
	GPreg[opera1]=inPort[opera2];
}
inline void out(void)
{
	outPort[opera2]=GPreg[opera1];
}
//Vector jump/call
inline void jv(void) {OpcodeNotDone();}
inline void cv(void)
{
	CPUWriteMemory(PC, SP--);
	jv();
}
//Load effective address
inline void lea(void) {OpcodeNotDone();}
//Miscellaneous
inline void push(void)
{
	CPUWriteMemory(GPreg[opera1], SP--);
}
inline void pop(void)
{
	GPreg[opera1]=CPUReadMemory(++SP);
}
inline void nop(void)
{
	return;
}
inline void mul(void)
{
	uint32_t i=GPreg[opera1]*GPreg[opera3];
	inPort[1]=i>>16;
	inPort[0]=i-(inPort[1]<<16);
}
inline void stsp(void)
{
	SP=GPreg[opera1];
}
inline void prod(void) {OpcodeNotDone();}
inline void jr(void)
{
	PC=GPreg[opera1];
}
inline void cr(void)
{
	CPUWriteMemory(PC, SP--);
	jr();
}
inline void ret(void)
{
	PC=CPUReadMemory(++SP);
}
inline void wait(void) {OpcodeNotDone();}
inline void send(void)
{
	if(CPU_SWITCH_DEBUG==1) printf("Serial Send!\n");
	printf("%c", GPreg[opera1]);
}
inline void ldsf(void) {OpcodeNotDone();}
inline void stsf(void) {OpcodeNotDone();}
inline void initv(void) {OpcodeNotDone();}
