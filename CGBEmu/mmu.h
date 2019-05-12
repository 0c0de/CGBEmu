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
	uint8_t A;
	uint8_t F;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;
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
		//Memory Map
		uint8_t bios[0xFF];
		uint16_t rom[0x4000];
		uint16_t switchable_rom[0x4000];
		uint16_t vram[0x2000];
		uint16_t switchable_ram[0x2000];
		uint16_t ram[0x2000];
		uint16_t eram[0x2000];
		uint16_t echo_ram[0x1E00];
		uint8_t sprite_attrib[0xA0];
		uint8_t io[0x4C];
		uint8_t internal_ram[0x7F];

		//Stack Pointer
		uint16_t sp;

		uint16_t read(uint16_t addr);

		uint8_t read8(uint16_t addr);

		void PUSH(uint16_t addrToPush);

	private:
		bool isInBIOS = false;
};