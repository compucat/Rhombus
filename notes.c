/*
Opgroups (binary):
00: movih (mode A)
01: arithmetic/logic (mode A)
10: branch/subroutine call (mode B)
11: other (mode A/C)

and, or, xor functions are and_inst, or_inst, xor_inst
0101000011111001


The 16k cache is not emulated; all components access RAM directly. As such, the 16k binary is prewritten into RAM.


Addressing modes:
	Mode A: aa bbb ccc ddddd eee
	Mode B: aa b bbbb cccccc ccc
	Mode C: aa bbb ccc d eeeeeee

where a: opgroup, b: opcode, c: operand 1, d: operand 2, e: operand 3


CPU Instructions

Move immediate high (d <- c * 32)
00 ccc ddd ccccc ccc    movih   d, c

Arithmetic/Logic

****************************************NOTE!!! Immediate means it operates with a constant set immediately
****************************************as opposed to a register.

01 000 ddd 00000 sss    add     d, s	Add
01 000 ddd 1cccc-ccc    addi    d, c	Add Immediate
01 001 ddd 00000 sss    adc     d, s	Add With Carry
01 001 ddd 1cccc-ccc    adci    d, c	Add With Carry Immediate
01 010 ddd 00000 sss    sub     d, s	Subtract
01 010 ddd 1cccc-ccc    subi    d, c	Subtract Immediate
01 011 ddd 00000 sss    and     d, s	Bitwise AND
01 011 ddd 1cccc-ccc    andi    d, c	Bitwise AND Immediate
01 100 ddd 00000 sss    or      d, s	Bitwise OR
01 100 ddd 1cccc-ccc    ori     d, c	Bitwise OR Immediate
01 101 ddd 00000 sss    xor     d, s	Bitwise XOR
01 101 ddd 1cccc-ccc    xori    d, c	Bitwise XOR Immediate
01 110 ddd 00000 sss    cmp     d, s	Compare
01 110 ddd 1cccc-ccc    cmpi    d, c	Compare Immediate
01 111 ddd 00000 sss    mov     d, s	Move value from register S to register D
01 111 ddd 1cccc-ccc    movi    d, c	Move immediate value to register D

Branch (o = signed offset relative pc)
10 0 0001 oooooo-ooo    bgt     label	Branch when () is greater than ()
10 0 0011 oooooo-ooo    bne     label	Branch when Compare returns "not equal"
10 0 0101 oooooo-ooo    bcc,bge label	Branch when the Carry bit is clear
10 0 1010 oooooo-ooo    bcs,blt label	Branch when the Carry bit is set
10 0 1100 oooooo-ooo    beq     label	Branch when Compare returns "equal"
10 0 1110 oooooo-ooo    ble     label	Branch when () is less than or equal to ()
10 0 1111 oooooo-ooo    bal     label	Branch always (?)

Subroutine call
10 1 0001 oooooo-ooo    cgt     label	***Same as for branches, but first push address of next instruction onto stack***
10 1 0011 oooooo-ooo    cne     label
10 1 0101 oooooo-ooo    ccc,cge label
10 1 1010 oooooo-ooo    ccs,clt label
10 1 1100 oooooo-ooo    ceq     label
10 1 1110 oooooo-ooo    cle     label
10 1 1111 oooooo-ooo    cal     label

Memory
11 000 ddd ooooo sss    ld      d, s+o	Load
11 001 ddd ooooo sss    st      s+o, d	Store

I/O
11 010 ddd 00ppp 000    in      d, p	Store value from input port P to register D
11 011 ddd 00ppp 000    out     p, d	Store value from register D in output port P

Vector jump/call (e = entry in global vector table)
11 100 000 0 eeeeeee    jv      e
11 101 000 0 eeeeeee    cv      e

Load effective address (o = unsigned offset relative pc)
11 101 ddd 1 ooooooo    lea     d, label

Miscellaneous
11 111 ddd 00000 000    push    d	Push register d onto the stack
11 111 ddd 00001 000    pop     d	Pop something from the stack into register d
11 111 000 00010 000    nop     	Do nothing!
11 111 ddd 00011 sss    mul     d, s    Store result in special product register
11 111 ddd 00100 000    stsp    d       Store d into stack pointer
11 111 ddd 00101 000    prod    d, s    Store s:d in product register
11 111 ddd 00110 000    jr      d       Jump to address in register
11 111 ddd 00111 000    cr      d       Call address in register
11 111 000 01000 000    ret             Return from subroutine
11 111 ddd 01001 000    wait    d       Wait for status bit (blitter done, vblank...)
11 111 ddd 01010 000    send    d       Transmit on debug UART
11 111 ddd 01011 000    ldsf    d       Load d from status flags
11 111 ddd 01100 000    stsf    d       Store d into status flags
11 111 ddd 01101 000    initv   d       Set global vector table address
Input ports

000 product, low half
001 product, high half
010 status flags (blitter done, vblank...)
011 uart receive buffer
100 frame counter (global time)
101 benchmark timer
Output ports

000 blitter row
001 blitter column
010 blitter width
011 blitter height + start
100 blitter program
101 active video page [1..3]
110 synth register select
111 synth register data
*/
