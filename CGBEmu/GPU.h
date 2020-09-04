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
	int palette[160][8][8];

	int framebuffer[160 * 144 * 4];

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
	void renderBackground(MMU *mmu, uint8_t currentLine);

	//Draw the framebuffer into screen
	void DrawScreen(MMU* mmu);

	//Can enable screen?
	bool isScreenEnabled(MMU* mmu);

	//Internal way to track vram
	void updateVRAM(MMU* mmu);

private:
	bool isKthBitSet(uint8_t n, uint8_t k);

	int SDL_CalculatePitch(Uint32 format, int width);
};
