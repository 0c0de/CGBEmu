#include "GPU.h"

bool GPU::isKthBitSet(uint8_t n, uint8_t k) {
	if (n & (1 << (k - 1))) {
		return true;
	}
	else {
		return false;
	}
}

int GPU::SDL_CalculatePitch(Uint32 format, int width)
{
	int pitch;

	if (SDL_ISPIXELFORMAT_FOURCC(format) || SDL_BITSPERPIXEL(format) >= 8) {
		pitch = (width * SDL_BYTESPERPIXEL(format));
	}
	else {
		pitch = ((width * SDL_BITSPERPIXEL(format)) + 7) / 8;
	}
	pitch = (pitch + 3) & ~3;   /* 4-byte aligning for speed */
	return pitch;
}

uint8_t GPU::getSCX(MMU *mmu) {
	return mmu->read8(0xFF42);
}

uint8_t GPU::getSCY(MMU* mmu) {
	return mmu->read8(0xFF43);
}

bool GPU::isSpriteBig(MMU* mmu) {
	uint8_t n = mmu->read8(0xFF40);
	return isKthBitSet(n, 2);
}

bool GPU::bgUsed(MMU* mmu) {
	uint8_t n = mmu->read8(0xFF40);
	return isKthBitSet(n, 3);
}

void GPU::renderScan(MMU *mmu, SDL_Renderer* render) {
	uint16_t offset = 0;
	if (bgUsed(mmu)) {
		offset = 0x1C00;
	}
	else {
		offset = 0x1800;
	}

	offset += ((line + getSCY(mmu)) & 0xFF) >> 3;
	uint8_t lineOffset = (getSCX(mmu) >> 3);
	uint8_t y = (line + getSCY(mmu)) & 0x07;
	uint8_t x = getSCX(mmu) & 0x07;

	uint16_t renderOffset = line * 160;

	uint16_t color;
	uint16_t tileSelected = mmu->read8(offset + lineOffset);

	for (int i = 0; i < 160; i++) {
		//0x3
		if (isKthBitSet(mmu->read8(0x8000 + a), b) && isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
			SDL_SetRenderDrawColor(render, 0x1f, 0x33, 0x24, 0xff);
			//palette[a - 2][lineTemp] = 0x1f3324;
		}

		//0x2
		if (!isKthBitSet(mmu->read8(0x8000 + a), b) && isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
			SDL_SetRenderDrawColor(render, 0x3d, 0x66, 0x47, 0xFF);
			//palette[a - 2][lineTemp] = 0x3d6647;
			//palette[lineTemp * 160 + b] = 0x3d6647;
		}

		//0x1
		if (isKthBitSet(mmu->read8(0x8000 + a), b) && !isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
			SDL_SetRenderDrawColor(render, 0x5c, 0x99, 0x6b, 0xFF);
			//palette[a - 2][lineTemp] = 0x5c996b;
			//palette[lineTemp * 160 + b] = 0x5c996b;
		}

		//0x0
		if (!isKthBitSet(mmu->read8(0x8000 + a), b) && !isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
			SDL_SetRenderDrawColor(render, 0x7a, 0xcc, 0x8f, 0xff);
			//palette[a - 2][lineTemp] = 0x7acc8f;
			//palette[lineTemp * 160 + b] = 0x7acc8f;
		}
	}

	//PRINT ALL VALUES FROM 0x000 TO 0xFFF in VRAM
	/*for (uint16_t a = 0; a < 0xFFF; a+=2) {
		for (uint16_t b = 0; b < 8; b++) {
			SDL_Rect rect;
			rect.h = 1;
			rect.w = 1;

			rect.x = (a * lineTemp) + b;
			rect.y = lineTemp;
			//0x3
			if (isKthBitSet(mmu->read8(0x8000 + a), b) && isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
				SDL_SetRenderDrawColor(render, 0x1f, 0x33, 0x24, 0xff);
				//palette[a - 2][lineTemp] = 0x1f3324;
			}

			//0x2
			if (!isKthBitSet(mmu->read8(0x8000 + a), b) && isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
				SDL_SetRenderDrawColor(render, 0x3d, 0x66, 0x47, 0xFF);
				//palette[a - 2][lineTemp] = 0x3d6647;
				//palette[lineTemp * 160 + b] = 0x3d6647;
			}

			//0x1
			if (isKthBitSet(mmu->read8(0x8000 + a), b) && !isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
				SDL_SetRenderDrawColor(render, 0x5c, 0x99, 0x6b, 0xFF);
				//palette[a - 2][lineTemp] = 0x5c996b;
				//palette[lineTemp * 160 + b] = 0x5c996b;
			}

			//0x0
			if (!isKthBitSet(mmu->read8(0x8000 + a), b) && !isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
				SDL_SetRenderDrawColor(render, 0x7a, 0xcc, 0x8f, 0xff);
				//palette[a - 2][lineTemp] = 0x7acc8f;
				//palette[lineTemp * 160 + b] = 0x7acc8f;
			}
			SDL_RenderFillRect(render, &rect);
		}
		if (a % 160 == 0) {
			lineTemp++;
		}
	}*/

}

void GPU::step(CPU *gameboy, MMU *mmu, SDL_Renderer* render, SDL_Texture *texture) {
	clock += gameboy->cicles;

	switch (mode)
	{
		case 0:
			//Horizontal Blanking
			//std::cout << "Entering Horizontal Blanking" << std::endl;
			if (clock >= 204) {
				clock = 0;

				line++;

				mmu->write8(0xFF44, line);

				if (line == 0x8F) {
					//Enter in Vertical Blanking Mode
					mode = 1;
					//TODO: Write a function that write data into the SDL Render
					std::cout << "Writing data into SDL Render" << std::endl;
					//uint16_t screenData[160*144*3];

					/*for (int x = 0; x < 160; x++) {
						for (int y = 0; y < 144; y++) {
							screenData[x * y] = palette[x][y];
						}
					}*/

					//SDL_UpdateTexture(texture, NULL, screenData, 160 * sizeof(Uint16));
					//SDL_RenderCopy(render, texture, NULL, NULL);
					SDL_RenderPresent(render);
				}
				else {
					mode = 2;
				}
			}
			break;
		case 1:
			//Vertical Blanking
			//std::cout << "Entering Vertical Blanking" << std::endl;
			if (clock >= 456) {
				clock = 0;
				line++;


				mmu->write8(0xFF44, line);

				if (line > 0x99) {
					mode = 2;
					line = 0;
				}
			}
			break;
		case 2:
			//Read OAM
			//std::cout << "Reading OAM Mode" << std::endl;
			if (clock >= 80) {
				clock = 0;
				mode = 3;
			}
			break;
		case 3:
			//Read VRAM for generate picture
			//std::cout << "Reading VRAM for generate picture" << std::endl;
			if (clock >= 172) {

				//Enter Horizontal Blanking
				clock = 0;
				mode = 0;

				//Write a line to framebuffer based in the VRAM
				//TODO: Write a function that does this
				renderScan(mmu, render);
			}
			break;
	}
}