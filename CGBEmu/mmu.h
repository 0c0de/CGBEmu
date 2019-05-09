#pragma once
#include <stdio.h>
#include <cstdio>
#include <iostream>

//16 Bit registers
		//AF
		//BC
		//DE
		//HL
		//8 Bit registers
		//A -> 0
		//F -> 1
		//B -> 2
		//C -> 3
		//D -> 4
		//E -> 5
		//H -> 6
		//L -> 7
struct GameboyRegisters
{
	uint16_t A;
	uint16_t F;
	uint16_t B;
	uint16_t C;
	uint16_t D;
	uint16_t E;
	uint16_t H;
	uint16_t L;
	uint16_t AF;
	uint16_t BC;
	uint16_t DE;
	uint16_t HL;

};

struct GameboyFlags {
	//Carry Flag
	bool C;
	//Zero Flag
	bool Z;
	//Substraction Flag
	bool N;
	//Half Carry flag
	bool H;
};

class MMU {
	public:
		//Game Memory
		uint16_t memory[0xFFFF];

		//RAM size is 8192 bytes
		unsigned char RAM[0x2000];

		//VRAM size is 8192
		unsigned char VRAM[0x2000];

		//Stack Pointer
		uint16_t sp;

		uint16_t read(uint16_t addr);

		uint8_t read8(uint16_t addr);

		void PUSH(uint16_t addrToPush);
};