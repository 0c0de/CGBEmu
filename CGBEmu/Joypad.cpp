#include "Joypad.h"

uint8_t setBit(uint8_t n, uint8_t a) {
	return n |= 1 << a;
}

uint8_t resetBit(uint8_t n, uint8_t a) {
	return n &= ~(1 << a);
}

bool Joypad::isKthBitSet(int n, int k)
{
	if (n & (1 << k)) {
		return true;
	}
	else {
		return false;
	}
}

void Joypad::setButton(uint8_t key, MMU *mmu, Interrupt *interr) {
	bool previouslyUnset = false;

	if (!isKthBitSet(joyPadState, key)) {
		previouslyUnset = true;
	}

	joyPadState = resetBit(joyPadState, key);

	bool button = true;
	bool requestInt = false;

	if (key < 3) {
		button == false;
	}

	uint8_t reqButtons = mmu->read8(0xFF00);

	if (button && isKthBitSet(reqButtons, 5)) {
		requestInt = true;	
	}

	if (!button && isKthBitSet(reqButtons, 4)) {
		requestInt = true;
	}

	if (requestInt && !previouslyUnset) {
		interr->requestInterrupt(mmu, 4);
	}
}

void Joypad::releaseButton(uint8_t key, MMU *mmu, Interrupt *interr) {
	joyPadState = setBit(joyPadState, key);
}


uint8_t Joypad::GetJoypadState(MMU *mmu) {
	uint8_t res = mmu->read8(0xFF00);

	//Flip all bits
	res ^= 0xFF;

	//Check for standard buttons
	if (isKthBitSet(res, 5)) {
		uint8_t topJoypad = joyPadState >> 4;
		topJoypad |= 0xF0;
		res &= topJoypad;
	}

	if (isKthBitSet(res, 4)) {
		uint8_t lowJoypad = joyPadState & 0xF;
		lowJoypad |= 0xF0;
		res &= lowJoypad;
	}

	return res;
}