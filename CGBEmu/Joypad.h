#pragma once

#include "mmu.h"
#include "Interrupts.h"


class Joypad {
public:
	uint8_t joyPadState;
	bool isKthBitSet(int n, int k);
	void setButton(uint8_t key, MMU *mmu, Interrupt *interr);
	void releaseButton(uint8_t key, MMU *mmu, Interrupt *interr);
	uint8_t GetJoypadState(MMU *mmu);
};