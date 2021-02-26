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
	if (n & (1 << k)) {
		return true;
	}
	else {
		return false;
	}
}

uint8_t GPU::BitGetVal(uint8_t valueToGet, uint8_t bitToDisplace) {
	uint8_t lMsk = 1 << bitToDisplace;
	return (valueToGet & lMsk) ? 1 : 0;
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

/*void GPU::DrawScreen(MMU *mmu) {
	for (int i = 0; i < 160; i++) {
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				//0x3
				if (palette[i][x][y] == 3) {
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0x3e0800;
				}

				//0x2
				if (palette[i][x][y] == 2) {
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0xa33e31;
				}

				//0x1
				if (palette[i][x][y] == 1) {
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0xd78169;
				}

				//0x0
				if (palette[i][x][y] == 0) {
					framebuffer[(i * 8 % 160) - x + (y + i * 8 / 160 * 8) * 160] = 0xf3c5b1;
				}

			}
		}
	}
}*/

void GPU::updateVRAM(MMU *mmu, uint8_t idx, uint16_t address1, uint16_t address2, uint8_t currLine, bool isBG) {
	if (!isBG) {
		for (int x = 0; x < 8; x++) {
			//0x3
			if (isKthBitSet(mmu->read8(address1), x) && isKthBitSet(mmu->read8(address2), x)) {
				uint8_t tt = idx;
				palette[idx][x][currLine] = 3;
			}

			//0x2
			if (!isKthBitSet(mmu->read8(address1), x) && isKthBitSet(mmu->read8(address2), x)) {
				uint8_t tt = idx;
				palette[idx][x][currLine] = 2;
			}

			//0x1
			if (isKthBitSet(mmu->read8(address1), x) && !isKthBitSet(mmu->read8(address2), x)) {
				uint8_t tt = idx;
				palette[idx][x][currLine] = 1;
			}
		}
	}
	else {
		
	}
}

uint8_t GPU::clearBit(uint8_t value, uint8_t bitToReset) {
	uint8_t bitCleared = value & ~(1 << bitToReset);

	return bitCleared;
}

uint8_t GPU::setBit(uint8_t value, uint8_t bitToSet) {
	uint8_t bitSet = value | (1 << bitToSet);

	return bitSet;
}

void GPU::changeModeGPU(MMU *mmu, uint8_t gpuMode) {
	mode = gpuMode;
	uint16_t lcdStat = 0xFF41;

	uint8_t lcdStatValue = mmu->read8(lcdStat);

	uint8_t bit1 = lcdStatValue;
	uint8_t bit2 = 0;
	/*
	switch (gpuMode)
	{
	case 0: //HBlank
		bit1 = clearBit(lcdStatValue, 1);
		bit2 = clearBit(lcdStatValue, 0);
		mmu->write8(0xFF41, bit2);
		break;

	case 1: //VBlank
		bit1 = clearBit(lcdStatValue, 1);
		bit2 = setBit(lcdStatValue, 0);
		mmu->write8(0xFF41, bit2);
		break;

	case 2: //OAM
		bit1 = setBit(lcdStatValue, 1);
		bit2 = clearBit(lcdStatValue, 0);
		mmu->write8(0xFF41, bit2);
		break;

	case 3: //LCD Transfer
		bit1 = setBit(lcdStatValue, 1);
		bit2 = setBit(lcdStatValue, 0);
		mmu->write8(0xFF41, bit2);
		break;
	default:
		break;
	}
	*/

}

/*
EASTER EGG:
Este es un comentario en ESPAÑOL para los putos guiris que intente copiar el código al motelu el pajas VR
*/

void GPU::renderFramebuffer(SDL_Renderer *render) {
	/*GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width = 160;
	int height = 144;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
	//glGenerateMipmap(GL_TEXTURE_2D);

	*textureRetruned = texture;*/

	for (int x = 0; x < 160; x++) {
		for (int y = 0; y < 144; y++) {
			uint8_t red = framebuffer[x][y][0];
			uint8_t green = framebuffer[x][y][1];
			uint8_t blue = framebuffer[x][y][2];

			SDL_SetRenderDrawColor(render, red, green, blue, 0xFF);
			SDL_RenderDrawPoint(render, x, y);
		}
	}
}

uint8_t GPU::getColour(uint8_t colourNum, uint16_t address, MMU *mmu) {
	uint8_t res = 0; //Full white
	uint8_t palette = mmu->read8(address);
	int hi = 0;
	int lo = 0;

	// which bits of the colour palette does the colour id map to?
	switch (colourNum)
	{
	case 0: hi = 1; lo = 0; break;
	case 1: hi = 3; lo = 2; break;
	case 2: hi = 5; lo = 4; break;
	case 3: hi = 7; lo = 6; break;
	}

	// use the palette to get the colour
	int colour = 0;
	colour = BitGetVal(palette, hi) << 1;
	colour |= BitGetVal(palette, lo);

	// convert the game colour to emulator colour
	
	res = colour;

	return res;

}

void GPU::DrawScanline(MMU* mmu) {
	uint8_t n = mmu->read8(0xFF40);
	//Can draw things
	if (isKthBitSet(n, 7)) {
		if (isKthBitSet(n, 0)) {
			//Display Background
			//std::cout << "Rendering background" << std::endl;
			renderBackground(mmu);
			
		}
		else {
			//Display Sprites
			//std::cout << "Rendering sprites" << std::endl;
			renderSprites(mmu);
		}
	}
}

void GPU::renderBackground(MMU *mmu) {

	uint16_t tileData = 0;
	uint16_t backgroundMemory = 0;
	uint8_t lcdControl = 0xFF40;
	bool unsig = true;

	// where to draw the visual area and the window
	uint8_t scrollY = mmu->read8(0xFF42);
	uint8_t scrollX = mmu->read8(0xFF43);
	uint8_t windowY = mmu->read8(0xFF4A);
	uint8_t windowX = mmu->read8(0xFF4B) - 7;

	bool usingWindow = false;

	// is the window enabled?
	if (isKthBitSet(lcdControl, 5))
	{
		// is the current scanline we're drawing
		// within the windows Y pos?,
		if (windowY <= mmu->read8(0xFF44))
			usingWindow = true;
	}

	// which tile data are we using?
	if (isKthBitSet(lcdControl, 4))
	{
		tileData = 0x8000;
	}
	else
	{
		// IMPORTANT: This memory region uses signed
		// bytes as tile identifiers
		tileData = 0x8800;
		unsig = false;
	}

	// which background mem?
	if (false == usingWindow)
	{
		if (isKthBitSet(lcdControl, 3))
			backgroundMemory = 0x9C00;
		else
			backgroundMemory = 0x9800;
	}
	else
	{
		// which window memory?
		if (isKthBitSet(lcdControl, 6))
			backgroundMemory = 0x9C00;
		else
			backgroundMemory = 0x9800;
	}

	uint8_t yPos = 0;

	// yPos is used to calculate which of 32 vertical tiles the
	// current scanline is drawing
	if (!usingWindow)
		yPos = scrollY + mmu->read8(0xFF44);
	else
		yPos = mmu->read8(0xFF44) - windowY;

	// which of the 8 vertical pixels of the current
	// tile is the scanline on?
	uint16_t tileRow = (((uint8_t)(yPos / 8)) * 32);

	// time to start drawing the 160 horizontal pixels
	// for this scanline
	for (int pixel = 0; pixel < 160; pixel++)
	{
		uint8_t xPos = pixel + scrollX;

		// translate the current x pos to window space if necessary
		if (usingWindow)
		{
			if (pixel >= windowX)
			{
				xPos = pixel - windowX;
			}
		}

		// which of the 32 horizontal tiles does this xPos fall within?
		uint16_t tileCol = (xPos / 8);
		int16_t tileNum;

		// get the tile identity number. Remember it can be signed
		// or unsigned
		uint16_t tileAddrss = backgroundMemory + tileRow + tileCol;
		if (unsig)
			tileNum = (uint8_t)mmu->read8(tileAddrss);
		else
			tileNum = (int8_t)mmu->read8(tileAddrss);

		// deduce where this tile identifier is in memory. Remember i
		// shown this algorithm earlier
		uint16_t tileLocation = tileData;

		if (unsig)
			tileLocation += (tileNum * 16);
		else
			tileLocation += ((tileNum + 128) * 16);

		// find the correct vertical line we're on of the
		// tile to get the tile data
		//from in memory
		uint8_t line = yPos % 8;
		line *= 2; // each vertical line takes up two bytes of memory
		uint8_t data1 = mmu->read8(tileLocation + line);
		uint8_t data2 = mmu->read8(tileLocation + line + 1);

		// pixel 0 in the tile is it 7 of data 1 and data2.
		// Pixel 1 is bit 6 etc..
		int colourBit = xPos % 8;
		colourBit -= 7;
		colourBit *= -1;

		// combine data 2 and data 1 to get the colour id for this pixel
		// in the tile
		int colourNum = BitGetVal(data2, colourBit);
		colourNum <<= 1;
		colourNum |= BitGetVal(data1, colourBit);

		// now we have the colour id get the actual
		// colour from palette 0xFF47
		uint8_t col = getColour(colourNum, 0xFF47, mmu);
		int red = 0;
		int green = 0;
		int blue = 0;

		// setup the RGB values
		switch (col)
		{
		case 0: red = 255; green = 255; blue = 255; break;
		case 1:red = 0xCC; green = 0xCC; blue = 0xCC; break;
		case 2: red = 0x77; green = 0x77; blue = 0x77; break;
		}

		int finaly = mmu->read8(0xFF44);

		// safety check to make sure what im about
		// to set is int the 160x144 bounds
		if ((finaly < 0) || (finaly > 143) || (pixel < 0) || (pixel > 159))
		{
			continue;
		}

		framebuffer[pixel][finaly][0] = red;
		framebuffer[pixel][finaly][1] = green;
		framebuffer[pixel][finaly][2] = blue;
	}

}

void GPU::renderSprites(MMU *mmu) {

	bool use8x16 = false;
	uint8_t lcdControl = 0xFF40;

	if (isKthBitSet(lcdControl, 2))
		use8x16 = true;

	for (int sprite = 0; sprite < 40; sprite++)
	{
		// sprite occupies 4 bytes in the sprite attributes table
		uint8_t index = sprite * 4;
		uint8_t yPos = mmu->read8(0xFE00 + index) - 16;
		uint8_t xPos = mmu->read8(0xFE00 + index + 1) - 8;
		//std::cout << "Index sprite: " << sprite << "Pixel value: " << static_cast<unsigned>(xPos) << std::endl;
		uint8_t tileLocation = mmu->read8(0xFE00 + index + 2);
		uint8_t attributes = mmu->read8(0xFE00 + index + 3);

		bool yFlip = isKthBitSet(attributes, 6);
		bool xFlip = isKthBitSet(attributes, 5);

		int scanline = mmu->read8(0xFF44);

		int ysize = 8;
		if (use8x16)
			ysize = 16;

			int line = scanline - yPos;

			// read the sprite in backwards in the y axis
			if (yFlip)
			{
				line -= ysize;
				line *= -1;
			}

			line *= 2; // same as for tiles
			uint16_t dataAddress = (0x8000 + (tileLocation * 16)) + line;
			//std::cout << std::hex << "Tile Location: 0x" << static_cast<unsigned>(tileLocation) << "Line is: 0x" << static_cast<unsigned>(line) << std::endl;
			uint8_t data1 = mmu->read8(dataAddress);
			uint8_t data2 = mmu->read8(dataAddress + 1);

			// its easier to read in from right to left as pixel 0 is
			// bit 7 in the colour data, pixel 1 is bit 6 etc...
			for (int tilePixel = 7; tilePixel >= 0; tilePixel--)
			{
				int colourbit = tilePixel;
				// read the sprite in backwards for the x axis
				if (xFlip)
				{
					colourbit -= 7;
					colourbit *= -1;
				}

				// the rest is the same as for tiles
				int colourNum = BitGetVal(data2, colourbit);
				colourNum <<= 1;
				colourNum |= BitGetVal(data1, colourbit);

				uint16_t colourAddress = isKthBitSet(attributes, 4) ? 0xFF49 : 0xFF48;
				uint8_t col = getColour(colourNum, colourAddress, mmu);

				// white is transparent for sprites.
				if (col == 0)
					continue;

				int red = 0;
				int green = 0;
				int blue = 0;

				switch (col)
				{
				case 0: red = 255; green = 255; blue = 255; break;
				case 1:red = 0xCC; green = 0xCC; blue = 0xCC; break;
				case 2:red = 0x77; green = 0x77; blue = 0x77; break;
				}

				int xPix = 0 - tilePixel;
				xPix += 7;

				int pixel = xPos + xPix;

				// sanity check
				if ((scanline < 0) || (scanline > 143) || (pixel < 0) || (pixel > 159))
				{
					continue;
				}

				framebuffer[pixel][scanline][0] = red;
				framebuffer[pixel][scanline][1] = green;
				framebuffer[pixel][scanline][2] = blue;
			}
		
	}

}

void GPU::step(uint16_t cycles, MMU *mmu, SDL_Renderer *render, Interrupt *interr) {
	if (isScreenEnabled(mmu)) {
		clock += cycles;
		switch (mode)
		{
		case 0:
			//Horizontal Blanking
			//std::cout << "Entering Horizontal Blanking" << std::endl;
			if (clock >= 204) {
				clock = 0;

				line++;

				mmu->write8(0xFF44, line);

				if (line == 143) {
					//Enter in Vertical Blanking Mode
					changeModeGPU(mmu, 1);
					
					interr->requestInterrupt(mmu, 0);
					//gameboy->requestInterrupt(mmu, 0);
					//TODO: Write a function that write data into the SDL Render
					renderFramebuffer(render);
					SDL_RenderPresent(render);
					std::cout << "Writing data from framebuffer" << std::endl;
				}
				else {
					changeModeGPU(mmu, 2);
				}
			}
			break;
		case 1:
			//Vertical Blanking
			//std::cout << "Entering Vertical Blanking" << std::endl;
			if (clock >= 456) {

				clock = 0;

				if (line == 0) {
					changeModeGPU(mmu, 2);
				}
				else {
					line++;

					mmu->write8(0xFF44, line);

					if (line == 0) {
						changeModeGPU(mmu, 2);
					}
				}
			}
			break;
		case 2:
			//Read OAM
			//std::cout << "Reading OAM Mode" << std::endl;
			if (clock >= 80) {
				clock = 0;
				changeModeGPU(mmu, 3);
			}
			break;
		case 3:
			//Read VRAM for generate picture
			//std::cout << "Reading VRAM for generate picture" << std::endl;
			if (clock >= 172) {

				//Enter Horizontal Blanking
				clock = 0;
				changeModeGPU(mmu, 0);

				//Write a line to framebuffer based in the VRAM
				//TODO: Write a function that does this
				//renderScanBackground(mmu, line);

				DrawScanline(mmu);
				//gameboy->requestInterrupt(mmu, 1);
				//renderSprites(mmu);
			}
			break;
		}
	}
}