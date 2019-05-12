#include "mmu.h"
#include "CPU.h"
using namespace std;

CPU cpus;

uint16_t MMU::read(uint16_t addr) {
	return (read8(addr) + read8(addr + 1) << 8);
}

uint8_t MMU::read8(uint16_t addr) {
	switch (addr & 0xf000)
	{
		case 0x0000:
			//Bios
			if (isInBIOS) {
				if (addr < 0x100) {
					return bios[addr];
				}
				else if (cpus.pc == 0x100) {
					isInBIOS = false;
				}
			}
			return rom[addr];
			break;
		case 0x1000:
		case 0x2000:
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
		case 0x8000:
		case 0x9000:
		case 0xA000:
		case 0xB000:
		case 0xC000:
		case 0xD000:
		case 0xE000:
		case 0xF000:
	default:
		cout << "That memory location is not mapped: " << addr << endl;
		break;
	}
}

void MMU::PUSH(uint16_t addrToPush) {
	//memory[sp] = addrToPush;
	//sp--;
}