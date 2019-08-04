#include "mmu.h"
#include "CPU.h"
using namespace std;

CPU cpus;

uint16_t MMU::read(uint16_t addr) {
	return read8(addr) + (read8(addr+1) << 8);
}

void MMU::push(uint16_t sp,uint16_t addr) {
	ram[sp] = addr;
	sp++;
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
			return vram[addr];
		case 0xA000:
		case 0xB000:
			return ram[addr];
		case 0xC000:
		case 0xD000:
			return wram[addr];
		case 0xE000:
			return echo_ram[addr & 0x1FFF];
		case 0xF000:
			switch (addr & 0x0F00)
			{
			case 0x0000: case 0x0100: case 0x0200: case 0x0300: case 0x0400: case 0x0500: case 0x0600:
			case 0x0700: case 0x0800: case 0x0900: case 0x0A00: case 0x0B00: case 0x0C00: case 0x0D00:
				return echo_ram[addr & 0x1FFF];
				break;
			case 0xE00:
				if (addr >= 0xfea0) {
					return sprite_attrib[addr & 0xFF];
				}
				else {
					return 0;
				}
			case 0xF00:
				if(addr >= 0xff80){
					return internal_ram[addr & 0xFF];
				}
				else {
					//Handle IO Things
					return 0;
				}
			default:
				break;
			}
	default:
		cout << "That memory location is not mapped: " << addr << endl;
		break;
	}
}

void MMU::write(uint16_t addr, uint16_t value) {

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
	case 0x8000:
	case 0x9000:
		vram[addr & 0x1FFF] = value;
	case 0xA000:
	case 0xB000:
		ram[addr & 0x1FFF] = value;
	case 0xC000:
	case 0xD000:
		wram[addr & 0x1FFF] = value;
	case 0xE000:
		echo_ram[addr & 0x1FFF] = value;
	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0000: case 0x0100: case 0x0200: case 0x0300: case 0x0400: case 0x0500: case 0x0600:
		case 0x0700: case 0x0800: case 0x0900: case 0x0A00: case 0x0B00: case 0x0C00: case 0x0D00:
			echo_ram[addr & 0x1FFF] = value;
			break;
		case 0xE00:
			if (addr >= 0xfea0) {
				sprite_attrib[addr & 0xFF] = value;
			}
			break;
		case 0xF00:
			if (addr >= 0xff80) {
				internal_ram[addr & 0xFF] = value;
			}
			break;
		default:
			break;
		}
	default:
		cout << "That memory location is not mapped: " << addr << endl;
		break;
	}
}