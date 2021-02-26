#pragma once
#include "glew.h"
#include "Interrupts.h"
#include <SDL.h>
#include "SDL_opengles2.h"

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

	int framebuffer[160][144][3];

	//Steps of the GPU
	void step(uint16_t cycles, MMU *mmu, SDL_Renderer *render, Interrupt *interr);

	//Get SCY
	uint8_t getSCY(MMU *mmu);

	//Get SCX
	uint8_t getSCX(MMU *mmu);

	//Is Sprite Big
	bool isSpriteBig(MMU *mmu);

	//Get BGMap to be used
	bool bgUsed(MMU *mmu);

	void renderBackground(MMU *mmu);
	void renderSprites(MMU *mmu);

	//Draw the framebuffer into screen
	void DrawScreen(MMU* mmu);

	//Can enable screen?
	bool isScreenEnabled(MMU* mmu);

	//Internal way to track vram
	void updateVRAM(MMU *mmu, uint8_t idx, uint16_t address1, uint16_t address2, uint8_t currLine, bool isBG);

	uint8_t getColour(uint8_t colorNum, uint16_t address, MMU* mmu);

	void renderFramebuffer(SDL_Renderer *render);

	//Draw the scanline
	void DrawScanline(MMU* mmu);

private:
	bool isKthBitSet(uint8_t n, uint8_t k);

	uint8_t BitGetVal(uint8_t valueToGet, uint8_t bitToDisplace);

	void changeModeGPU(MMU *mmu, uint8_t gpuMode);

	uint8_t clearBit(uint8_t value, uint8_t bitToReset);

	uint8_t setBit(uint8_t value, uint8_t bitToSet);

	int SDL_CalculatePitch(Uint32 format, int width);
};
