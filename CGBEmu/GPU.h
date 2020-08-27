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

	//Palette of colours
	uint16_t palette[160][144] ;

	//Steps of the GPU
	void step(CPU *gameboy, MMU *mmu, SDL_Renderer* render, SDL_Texture *texture);

	//Get SCY
	uint8_t getSCY(MMU *mmu);

	//Get SCX
	uint8_t getSCX(MMU *mmu);

	//Is Sprite Big
	bool isSpriteBig(MMU *mmu);

	//Get BGMap to be used
	bool bgUsed(MMU *mmu);

	//Render scan
	void renderScan(MMU *mmu, SDL_Renderer *render);

	//Draw the framebuffer into screen
	void DrawScreen(MMU* mmu);

private:
	bool isKthBitSet(uint8_t n, uint8_t k);

	int SDL_CalculatePitch(Uint32 format, int width);
};
