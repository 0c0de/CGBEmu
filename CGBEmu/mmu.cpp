#include "mmu.h"
#include "CPU.h"
using namespace std;

CPU cpus;

uint16_t MMU::read(uint16_t addr) {
	return read8(addr) + (read8(addr + 1) << 8);
}

uint8_t MMU::read8(uint16_t addr) {
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
		return rom[addr];
	case 0x8000:
	case 0x9000:
		return vram[addr - 0x8000];
	case 0xA000:
	case 0xB000:
		return ram[addr - 0xA000];
	case 0xC000:
	case 0xD000:
		return wram[addr - 0xC000];
	case 0xE000:
		return ram[addr - 0xE000];
	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0000: case 0x0100: case 0x0200: case 0x0300: case 0x0400: case 0x0500: case 0x0600:
		case 0x0700: case 0x0800: case 0x0900: case 0x0A00: case 0x0B00: case 0x0C00: case 0x0D00:
			return ram[addr - 0xE000];
			break;
		case 0xE00:
			if (addr >= 0xfea0) {
				return sprite_attrib[addr - 0xfea0];
			}
			else {
				return 0;
			}
		case 0xF00:
			if (addr >= 0xff80) {
				return internal_ram[addr - 0xff80];
			}
			else {
				//Handle IO Things
				return io[addr - 0xff00];
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
		ram[addr - 0xE000] = value;
		break;
	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0000: case 0x0100: case 0x0200: case 0x0300: case 0x0400: case 0x0500: case 0x0600:
		case 0x0700: case 0x0800: case 0x0900: case 0x0A00: case 0x0B00: case 0x0C00: case 0x0D00:
			ram[addr - 0xE000] = value;
			break;
		case 0xE00:
			if (addr >= 0xfea0) {
				sprite_attrib[addr - 0xFEA0] = value;
				break;
			}
			break;
		case 0xF00:
			if (addr >= 0xff80) {
				internal_ram[addr - 0xFF80] = value;
				break;
			}
			else {
				io[addr - 0xff00] = value;
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


void MMU::write8(uint16_t addr, uint8_t value) {
	//std::cout << "Writing 1 Byte in: " << hex << static_cast<unsigned>(addr) << " and a value of: " << hex << static_cast<unsigned>(value) << std::endl;
	//std::cout << "addr & 0xF000: " << hex << static_cast<unsigned>(addr & 0xF000) << std::endl;
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
		//std::cout << "Writing WRAM at : " << hex << static_cast<unsigned>(addr) << ", with a value of: " << hex << static_cast<unsigned>(value) << std::endl;
		wram[addr - 0xC000] = value;
		break;
	case 0xE000:
		ram[addr - 0xE000] = value;
		break;
	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0000: case 0x0100: case 0x0200: case 0x0300: case 0x0400: case 0x0500: case 0x0600:
		case 0x0700: case 0x0800: case 0x0900: case 0x0A00: case 0x0B00: case 0x0C00: case 0x0D00:
			ram[addr - 0xE000] = value;
			break;
		case 0xE00:
			if (addr >= 0xfea0) {
				sprite_attrib[addr - 0xFEA0] = value;
				break;
			}
			break;
		case 0xF00:
			if (addr >= 0xff80) {
				internal_ram[addr - 0xFF80] = value;
				break;
			}
			else {
				io[addr - 0xff00] = value;
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

void MMU::push(uint16_t value) {
	//std::cout << "Writing in RAM: " << std::hex << value << " at position: " << std::hex << sp << std::endl;
	sp--;
	write8(sp, value >> 8);
	sp--;
	write8(sp, (uint8_t)value);
}

void MMU::pop(uint16_t *value) {
	*value = (read8(sp + 1) << 8 | (read8(sp)));
	sp+=2;
}

void MMU::setRegisters16Bit(GameboyRegisters *reg, const char *regName, uint16_t valueToSet) {
	if (regName == "AF") {
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

void MMU::setRegisters8Bit(GameboyRegisters *reg, const char *regName, uint8_t valueToSet) {
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