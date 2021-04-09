#include "mmu.h"
#include "CPU.h"
#include <SDL.h>

using namespace std;

//CPU cpus;

bool isKthBitSet(uint8_t n, uint8_t k) {
	if (n & (1 << k)) {
		return true;
	}
	else {
		return false;
	}
}

uint16_t MMU::read(uint16_t addr) {
	return read8(addr) + (read8(addr + 1) << 8);
}

uint8_t MMU::read8(uint16_t addr) {
	switch (addr & 0xf000)
	{
	case 0x0000:
		if (isInBIOS) {
			return bios[addr];
		}

		return rom[addr];
	case 0x1000:
	case 0x2000:
	case 0x3000:
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return rom[addr];
	case 0x8000:
	case 0x9000:
		//cout << "Original address: " << hex << static_cast<unsigned>(addr) << " new address: " << hex << (addr - 0x8000) << endl;
		return vram[addr - 0x8000];
	case 0xA000:
	case 0xB000:
		return ram[addr - 0xA000];
	case 0xC000:
	case 0xD000:
		return wram[addr - 0xC000];
	case 0xE000:
		return wram[addr - 0xE000];
	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0000: case 0x0100: case 0x0200: case 0x0300: case 0x0400: case 0x0500: case 0x0600:
		case 0x0700: case 0x0800: case 0x0900: case 0x0A00: case 0x0B00: case 0x0C00: case 0x0D00:
			return wram[addr - 0xE000];
			break;
		case 0xE00:
			if (addr >= 0xFE00 && addr < 0xfea0) {
				return sprite_attrib[addr - 0xfe00];
			}
			else {
				return 0;
			}
		case 0xF00:

			if (addr >= 0xff80 && addr < 0xFFFE) {

				return internal_ram[addr - 0xff80];
				break;
			}

			if (addr >= 0xFF00 && addr < 0xFF80) {
			
				if (addr == 0xFF00) {
					return returnJoyPadState();
				}

				return io[addr - 0xff00];
				break;
			}
			break;
			
		}
		break;
	default:
		cout << "Read Function: Memory location is not mapped: " << hex << addr << endl;
		break;
	}
}


void MMU::write(uint16_t addr, uint16_t value) {
	write8(addr, value >> 8);
	write8(addr+1, (uint8_t)value);


	if (addr == 0xFF46) {
		//std::cout << "Doing DMA transfer" << std::endl;
		DMATransfer(value);
	}
}


void MMU::write8(uint16_t addr, uint8_t value) {
	switch (addr & 0xf000)
	{
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		rom[addr] = value;
		break;
	case 0x8000:
	case 0x9000:
		vram[addr - 0x8000] = value;
		break;
	case 0xA000:
	case 0xB000:
		ram[addr - 0xA000] = value;
		break;
	case 0xC000:
	case 0xD000:
		wram[addr - 0xC000] = value;
		break;
	case 0xE000:
		wram[addr - 0xE000] = value;
		break;
	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0000: case 0x0100: case 0x0200: case 0x0300: case 0x0400: case 0x0500: case 0x0600:
		case 0x0700: case 0x0800: case 0x0900: case 0x0A00: case 0x0B00: case 0x0C00: case 0x0D00:
			wram[addr - 0xE000] = value;
			break;
		case 0xE00:
			if (addr >= 0xFE00 && addr <= 0xFE9F) {
				sprite_attrib[addr - 0xFE00] = value;
				//std::cout << "Writing value in address: " << std::hex << static_cast<unsigned>(addr) << " Real is: " << static_cast<unsigned>(addr - 0xFE00) << " Value written: " << static_cast<unsigned>(value) << std::endl;
				break;
			}
			break;
		case 0xF00:

			if (addr >= 0xFF00 && addr < 0xFF80) {

				if (addr == 0xFF46) {
					//std::cout << "Doing DMA transfer" << std::endl;
					DMATransfer(value);
					break;
				}

				//Handle IO Things
				io[addr - 0xff00] = value;
				break;
			}

			if (addr >= 0xff80 && addr < 0xFFFE) {

				if (addr == 0xFF80) {
					std::cout << "Writing in 0xFF80 checking for the game" << std::endl;
					char tempGameName[0xF];
					for (int a = 0x0; a < 0xF; a++) {
						uint8_t valueRet = read8(0x134 + a);
						tempGameName[a] = valueRet;
					}
					
					std::string gameName = tempGameName;
					if (gameName == "TETRIS") {
						std::cout << "Detected tetris applying patch..." << std::endl;
						return;
						break;
					}
				}

				internal_ram[addr - 0xff80] = value;
				break;
			}
			break;
			
		}
		break;
	default:
		cout << "Write Function: Memory location is not mapped: " << hex << addr << endl;
		break;
	}
}

void MMU::DMATransfer(uint8_t value) {
	uint16_t address = value << 8; //Multiply value by 100 but instead of multiply, we shift left 8 positions

	for (int x = 0; x < 0xA0; x++) {
		write8(0xFE00 + x, read8(address + x));
		//std::cout << "Writing in " << 0xFE00 + x << " value: " << static_cast<unsigned>(read8(address + x)) << std::endl;
	}
}

void MMU::push(uint16_t value) {
	sp--;
	write8(sp, value >> 8);
	sp--;
	write8(sp, (uint8_t)value);
}

void MMU::pop(uint16_t *value) {
	uint8_t n = read8(sp);
	sp++;
	uint8_t nn = read8(sp);
	sp++;
	*value = n | nn << 8;
}

void MMU::setRegisters16Bit(GameboyRegisters *reg, const char *regName, uint16_t valueToSet, GameboyFlags* flags) {
	if (regName == "AF") {

		if (isKthBitSet((uint8_t)valueToSet, 7)) {
			flags->Z = true;
		}
		else {
			flags->Z = false;
		}

		if (isKthBitSet((uint8_t)valueToSet, 6)) {
			flags->N = true;
		}
		else {
			flags->N = false;
		}

		if (isKthBitSet((uint8_t)valueToSet, 5)) {
			flags->H = true;
		}
		else {
			flags->H = false;
		}

		if (isKthBitSet((uint8_t)valueToSet, 4)) {
			flags->C = true;
		}
		else {
			flags->C = false;
		}

		reg->A = (valueToSet >> 8);
		reg->F = (uint8_t)valueToSet;
		reg->AF = valueToSet;
	} else if (regName == "BC") {
		reg->B = (valueToSet >> 8);
		reg->C = (uint8_t)valueToSet;
		reg->BC = valueToSet;
	}else if (regName == "DE") {
		reg->D = (valueToSet >> 8);
		reg->E = (uint8_t)valueToSet;
		reg->DE = valueToSet;
	}else if (regName == "HL") {
		reg->H = (valueToSet >> 8);
		reg->L = (uint8_t)valueToSet;
		reg->HL = valueToSet;
	}else {
		std::cout << "No register found for " << regName << std::endl;
	}
}

void MMU::setRegisters8Bit(GameboyRegisters *reg, const char *regName, uint8_t valueToSet, GameboyFlags* flags) {
	if (regName == "A") {
		reg->A = valueToSet;
		reg->AF = ((reg->A << 8) | (reg->F));
	}
	else if (regName == "F") {
		reg->F = valueToSet;
		reg->AF = ((reg->A << 8) | (reg->F));
	}
	else if (regName == "B") {
		reg->B = valueToSet;
		reg->BC = ((reg->B << 8) | (reg->C));
	}
	else if (regName == "C") {
		reg->C = valueToSet;
		reg->BC = ((reg->B << 8) | (reg->C));
	}
	else if (regName == "D") {
		reg->D = valueToSet;
		reg->DE = ((reg->D << 8) | (reg->E));
	}
	else if (regName == "E") {
		reg->E = valueToSet;
		reg->DE = ((reg->D << 8) | (reg->E));
	}
	else if (regName == "H") {
		reg->H = valueToSet;
		reg->HL = ((reg->H << 8) | (reg->L));
	}
	else if (regName == "L") {
		reg->L = valueToSet;
		reg->HL = ((reg->H << 8) | (reg->L));
	}
	else {
		std::cout << "No register found for " << regName << std::endl;
	}
}