#include "GPU.h"

/*
0xFF40 Memory Address:
Bit 7 - LCD Display Enable(0 = Off, 1 = On)
Bit 6 - Window Tile Map Display Select(0 = 9800 - 9BFF, 1 = 9C00 - 9FFF)
Bit 5 - Window Display Enable(0 = Off, 1 = On)
Bit 4 - BG & Window Tile Data Select(0 = 8800 - 97FF, 1 = 8000 - 8FFF)
Bit 3 - BG Tile Map Display Select(0 = 9800 - 9BFF, 1 = 9C00 - 9FFF)
Bit 2 - OBJ(Sprite) Size(0 = 8x8, 1 = 8x16)
Bit 1 - OBJ(Sprite) Display Enable(0 = Off, 1 = On)
Bit 0 - BG Display(for CGB see below) (0 = Off, 1 = On)
*/

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

bool GPU::isScreenEnabled(MMU *mmu) {
	uint8_t n = mmu->read8(0xFF40);
	return isKthBitSet(n, 7);
}

bool GPU::bgUsed(MMU* mmu) {
	uint8_t n = mmu->read8(0xFF40);
	return isKthBitSet(n, 3);
}

void GPU::DrawScreen(MMU *mmu) {
	for (int i = 0; i < 0x100; i++) {
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				//0x3
				if (palette[i][x][y] == 3) {
					//SDL_SetRenderDrawColor(render, 0x3e, 0x08, 0x00, SDL_ALPHA_OPAQUE);
					//SDL_RenderFillRect(render, &rect);
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0x3e0800;
				}

				//0x2
				if (palette[i][x][y] == 2) {
					//SDL_SetRenderDrawColor(render, 0xa3, 0x3e, 0x31, SDL_ALPHA_OPAQUE);
					//SDL_RenderFillRect(render, &rect);
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0xa33e31;
				}

				//0x1
				if (palette[i][x][y] == 1) {
					//SDL_SetRenderDrawColor(render, 0xd7, 0x81, 0x69, SDL_ALPHA_OPAQUE);
					//(render, &rect);
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0xd78169;
				}

				//0x0
				if (palette[i][x][y] == 0) {
					//SDL_SetRenderDrawColor(render, 0xf3, 0xc5, 0xb1, SDL_ALPHA_OPAQUE);
					//SDL_RenderFillRect(render, &rect);
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0xf3c5b1;
				}

			}
		}
	}
}

void GPU::updateVRAM(MMU* mmu) {
	for (int i = 0; i < 0x100; i++) {
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				uint16_t n = i * 16 + (y * 2);
				/*if (i > 0xF) {
					//std::cout << "Reading address: " << hex << static_cast<unsigned>(n) << " y: " << y << " x: " << x << " i: " << i << std::endl;
				}*/
				//0x3
				if (isKthBitSet(mmu->read8(0x8000 + n), x) && isKthBitSet(mmu->read8(0x8000 + (n + 1)), x)) {
					uint8_t tt = i;
					palette[i][x][y] = 3;
				}

				//0x2
				if (!isKthBitSet(mmu->read8(0x8000 + n), x) && isKthBitSet(mmu->read8(0x8000 + (n + 1)), x)) {
					uint8_t tt = i;
					palette[i][x][y] = 2;
				}

				//0x1
				if (isKthBitSet(mmu->read8(0x8000 + n), x) && !isKthBitSet(mmu->read8(0x8000 + (n + 1)), x)) {
					uint8_t tt = i;
					palette[i][x][y] = 1;
				}
			}
		}

	}
}

void GPU::DrawScanline(MMU* mmu) {
	uint8_t n = mmu->read8(0xFF40);
	//Can draw things
	if (isKthBitSet(n, 7)) {

		if (isKthBitSet(n, 0)) {
			//Display Background
			renderBackground(mmu);
		}

		if (isKthBitSet(n, 1)) {
			//Display Sprites
		}
	}
}

void GPU::renderBackground(MMU *mmu) {

	//Scroll X and Y for getting the position of the tiles later
	uint8_t scrollX = getSCX(mmu);
	uint8_t scrollY = getSCY(mmu);

	//Window Positions
	uint8_t windowX = mmu->read8(0xFF4A);
	uint8_t windowY = mmu->read8(0xFF4B) - 7;

	//Is using window?
	bool isWindowUsed = false;

	//Get scanline
	uint8_t y = mmu->read8(0xFF44);

	//Is using unsigned tiles?
	bool isUnsigned = true;

	//Get which part of data we are going to use
	uint16_t backgroundDataSelectOffset = isKthBitSet(mmu->read8(0xFF40), 4) ? 0x8800 : 0x8000;

	//Get BG Tile Map Display offset
	uint16_t backgroundMem = 0;

	//Get Window Tile Map Display offset
	uint16_t tileMapOffset = isKthBitSet(mmu->read8(0xFF40), 6) ? 0x9800 : 0x9C00;

	if (isKthBitSet(mmu->read8(0xFF40), 5)) {
		if (windowY <= y) {
			isWindowUsed = true;
		}
	}

	if (backgroundDataSelectOffset == 0x8800) {
		isUnsigned = false;
	}

	if (isWindowUsed) {
		if (isKthBitSet(mmu->read8(0xFF40), 3)) 
		{
			backgroundMem = 0x9800;
		}
		else 
		{
			backgroundMem = 0x9C00;
		}
	}
	else 
	{
		if (isKthBitSet(mmu->read8(0xFF40), 6))
		{
			backgroundMem = 0x9800;
		}
		else
		{
			backgroundMem = 0x9C00;
		}
	}

	uint8_t yPos = 0;

	if (!isWindowUsed) 
	{
		yPos = scrollY + y;
	}
	else 
	{
		yPos = y - windowY;
	}

	uint16_t tileRow = (((uint8_t)(yPos / 8)) * 32);

	//Start drawing 160 pixels on the X position
	for (int x = 0; x < 160; x++) {
		uint8_t xPos = x + scrollX;

		if (isWindowUsed) {
			if (x >= windowX) {
				xPos = x - windowX;
			}
		}

		uint16_t tileCol = (xPos / 8);

		int8_t tileNum;

		uint16_t tileAddress = backgroundMem + tileRow + tileCol;

		if (isUnsigned) {
			tileNum = (uint8_t)mmu->read8(tileAddress);
		}
		else {
			tileNum = (int8_t)mmu->read8(tileAddress);
		}

		uint16_t tileLocation = backgroundDataSelectOffset;

		if (isUnsigned) {
			tileLocation += tileNum * 16;
		}
		else {
			tileLocation += (tileNum + 128) * 16;
		}

		uint8_t lineWeAre = yPos % 8;
		lineWeAre *= 2;

		uint8_t data1 = mmu->read8(tileLocation + line);
		uint8_t data2 = mmu->read8(tileLocation + line + 1);


	}

	//Read Background palette
	uint8_t backgroundPalette = mmu->read8(0xFF47);

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

					updateVRAM(mmu);
					DrawScreen(mmu);
					SDL_UpdateTexture(texture, NULL, framebuffer, SDL_CalculatePitch(SDL_PIXELFORMAT_ABGR8888, 160));
					SDL_RenderCopy(render, texture, NULL, NULL);
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
				//renderScanBackground(mmu, line);
			}
			break;
	}
}