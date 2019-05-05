#pragma once
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <fstream>
using namespace std;
//CPU is similar to Z80 but with modified instructions
class CPU {

public:
	//RAM size is 8192 bytes
	unsigned char RAM[8192];

	//VRAM size is 8192
	unsigned char VRAM[8192];

	//Program counter of gameboy
	uint16_t pc;

	//Stack Pointer
	uint16_t sp;

	//16 Bit registers
	//AF -> 0
	//BC -> 1
	//DE -> 2
	//HL -> 3
	uint16_t sixteen_registers[4];

	//Registers of gameboy
	//A -> 0
	//F -> 1
	//B -> 2
	//C -> 3
	//D -> 4
	//E -> 5
	//H -> 6
	//L -> 7
	uint8_t eight_registers[8];
	
	void loadGame(const char* path);

	void runLife();

	void loadBIOS();

	void init();
private:
	void setARegister();
	void setBRegister();
	void setCRegister();
	void setDRegister();
	void setERegister();
	void setFRegister();
	void setHRegister();
	void setLRegister();
	void setAFRegister();
	void setBCRegister();
	void setDERegister();
	void setHLRegister();
	void JP_NN(uint16_t opcode);
	void NOP(uint16_t opcode);
};