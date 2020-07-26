#pragma once
#include "CPU.h"
#include "mmu.h"
class GPU
{
public:

	//Mode of the GPU
	uint8_t mode = 0;

	//Clock of the GPU
	uint16_t clock = 0;

	//Scanline counter
	uint8_t line = 0x0;

	//Steps of the GPU
	void step(CPU *gameboy, MMU *mmu);

private:

};
