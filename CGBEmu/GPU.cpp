#include "GPU.h"

void GPU::step(CPU *gameboy, MMU *mmu) {
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

				if (line == 0x89) {
					//Enter in Vertical Blanking Mode
					mode = 1;
					//TODO: Write a function that write data into the SDL Render
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
			}
			break;
	}
}