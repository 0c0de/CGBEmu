#pragma once
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <fstream>
using namespace std;
//CPU is similar to Z80 but with modified instructions
class CPU {

public:
	//IME(Interrupt Master Enable) disabled by DI, enabled by EI
	bool IME;

	//Count cicles for keep games at the same speed
	uint16_t cicles;

	//Program counter of gameboy
	uint16_t pc;
	
	//Screen bidimensional array
	uint16_t gfx[160][144];

	void loadGame(const char* path);

	void runLife();

	void loadBIOS();

	void init();
private:
	void addCycles(int ciclesToAdd);
	void clearCycles();
	void JP_NN(uint16_t opcode);
	void NOP(uint16_t opcode);
	void DI();
	void EI();
	void CALL_NN(uint16_t opcode);
	void LD_A_NN(uint16_t opcode);
	void CP_N(uint16_t opcode);
	void JR_CC_N(uint16_t opcode);
	void LD_REG16_NN(uint16_t opcode);
	void LDH_A_N(uint16_t opcode);
};