#include "mmu.h"
using namespace std;

uint16_t MMU::read(uint16_t addr) {
	return memory[addr];
}

uint8_t MMU::read8(uint16_t addr) {
	return (uint8_t)memory[addr];
}