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

bool GPU::isScreenEnabled(MMU *mmu) {
	uint8_t n = mmu->read8(0xFF40);
	return isKthBitSet(n, 7);
}

bool GPU::bgUsed(MMU* mmu) {
	uint8_t n = mmu->read8(0xFF40);
	return isKthBitSet(n, 3);
}

void GPU::updateVRAM(MMU* mmu) {
	for (int i = 0; i < 0xFFF; i++) {
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				uint8_t n = i + y * 2;
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

				//0x0
				if (!isKthBitSet(mmu->read8(0x8000 + n), x) && !isKthBitSet(mmu->read8(0x8000 + (n + 1)), x)) {
					uint8_t tt = i;
					palette[i][x][y] = 0;
				}


				//std::cout << palette[i] << std::endl;
			}
		}
	}
}

void GPU::renderScanBackground(MMU *mmu, uint8_t currentLine) {
	/*uint16_t offset = 0;
	if (bgUsed(mmu)) {
		offset = 0x1C00;
	}
	else {
		offset = 0x1800;
	}

	uint8_t scrollX = getSCX(mmu);
	uint8_t scrollY = getSCY(mmu);

	uint8_t y = currentLine;*/

	/*for (uint8_t x = 0; x < 160; x++) {
		uint8_t realX = scrollX + x;
		uint8_t realY = scrollY + y;

		if (realX > 0xFF) {
			realX -= 0xFF;
		}

		if (realY > 0xFF) {
			realY -= 0xFF;
		}
	}*/
	/*for(int i = 0; i < 8; i++){
		for (int x = 0; x < 8;x++) {
			for (int y = 0; y < 8; y++) {
				//0x3
				if (palette[i * 8 + x + y * 160] == 3) {
					framebuffer[i * 8 + x + y * 160] = 0x3e0800;
				}

				//0x2
				if (palette[i * 8 + x + y * 160] == 2) {
					framebuffer[i * 8 + x + y * 160] = 0xa33e31;
				}

				//0x1
				if (palette[i * 8 + x + y * 160] == 1) {
					framebuffer[i * 8 + x + y * 160] = 0xd78169;
				}

				//0x0
				if (palette[i * 8 + x + y * 160] == 0) {
					framebuffer[i * 8 + x + y * 160] = 0xf3c5b1;
				}

			}
		}
	}*/

	/*
	offset += ((line + getSCY(mmu)) & 0xFF) >> 3;
	uint8_t lineOffset = (getSCX(mmu) >> 3);
	uint8_t y = (line + getSCY(mmu)) & 0x07;
	uint8_t x = getSCX(mmu) & 0x07;

	//PRINT ALL VALUES FROM 0x000 TO 0xFFF in VRAM
	for (int a = 0; a < 0xFFF; a+=2) {
		for (int b = 0; b < 8; b++) {
			for (int x = 0; x < 160; x++) {
				for (int y = 0; y < 144; y++) {
					//0x3
					if (isKthBitSet(mmu->read8(0x8000 + a), b) && isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
						palette[x][y][0] = 0x1f;
						palette[x][y][1] = 0x33;
						palette[x][y][2] = 0x24;
						palette[x][y][3] = SDL_ALPHA_OPAQUE;
					}

					//0x2
					if (!isKthBitSet(mmu->read8(0x8000 + a), b) && isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
						palette[x][y][0] = 0x3d;
						palette[x][y][1] = 0x66;
						palette[x][y][2] = 0x47;
						palette[x][y][3] = SDL_ALPHA_OPAQUE;
					}

					//0x1
					if (isKthBitSet(mmu->read8(0x8000 + a), b) && !isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
						palette[x][y][0] = 0x5c;
						palette[x][y][1] = 0x99;
						palette[x][y][2] = 0x6b;
						palette[x][y][3] = SDL_ALPHA_OPAQUE;
					}

					//0x0
					if (!isKthBitSet(mmu->read8(0x8000 + a), b) && !isKthBitSet(mmu->read8(0x8000 + (a + 1)), b)) {
						palette[x][y][0] = 0x7a;
						palette[x][y][1] = 0xcc;
						palette[x][y][2] = 0x8f;
						palette[x][y][3] = SDL_ALPHA_OPAQUE;
					}
				}

			}
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

				if (line == 0x90) {
					//Enter in Vertical Blanking Mode
					mode = 1;
					//TODO: Write a function that write data into the SDL Render
					SDL_UpdateTexture(texture, NULL, framebuffer, SDL_CalculatePitch(SDL_PIXELFORMAT_ABGR8888, 160));
					SDL_RenderCopy(render, texture, NULL, NULL);
					//SDL_RenderCopyEx(render, texture, NULL, NULL, -45, NULL, SDL_FLIP_NONE);
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
				updateVRAM(mmu);
				//renderScanBackground(mmu, line);

				for (int i = 0; i < 0xFFF; i++) {
					for (int x = 0; x < 8;x++) {
						for (int y = 0; y < 8; y++) {

							/*SDL_Rect rect;
							rect.x = i * 8 + x;
							rect.y = y;
							rect.w = 1;
							rect.h = 1;*/

							//std::cout << "Positioning at X: " << rect.x << " and Y: " << rect.y << std::endl;

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
			break;
	}
}