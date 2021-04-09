#include "Interrupts.h"

bool Interrupt::isKthBitSet(int n, int k)
{
	if (n & (1 << k)) {
		return true;
	}
	else {
		return false;
	}
}

void Interrupt::requestInterrupt(MMU *mmu, uint8_t id) {
	uint8_t IF_register = mmu->read8(0xFF0F); // Get value of IF Register
	IF_register |= (1 << id); //Set bit for the interruption
	mmu->write8(0xFF0F, IF_register); //Set again the address 0xFF0F which is the IF Register

	//std::cout << "Requesting interrupt with id: " << static_cast<unsigned>(id) << std::endl;
}

void Interrupt::checkForInterrupts(MMU *mmu, bool *isHaltedInterr, bool *IMEInter, uint16_t *pcInterr) {
	if (*IMEInter) {
		uint8_t req = mmu->read8(0xFF0F);
		uint8_t enabledInterrupt = mmu->read8(0xFFFF);

		if (req > 0) {
			//VBlank interrupt
			if (isKthBitSet(req, 0)) {
				if (isKthBitSet(enabledInterrupt, 0)) {
					doInterrupt(mmu, 0, isHaltedInterr, IMEInter, pcInterr);
				}
			}

			//LCD Interrupt
			/*if (isKthBitSet(IF_register, 1)) {
				if (isKthBitSet(enabledInterrupt, 1)) {
					doInterrupt(mmu, 1, isHaltedInterr, IMEInter, pcInterr);
				}
			}

			//TIMER Interrupt
			if (isKthBitSet(IF_register, 2)) {
				if (isKthBitSet(enabledInterrupt, 2)) {
					doInterrupt(mmu, 2, isHaltedInterr, IMEInter, pcInterr);
				}
			}

			//Serial interrupt
			if (isKthBitSet(IF_register, 3)) {
				if (isKthBitSet(enabledInterrupt, 3)) {
					doInterrupt(mmu, 3, isHaltedInterr, IMEInter, pcInterr);
				}
			}*/

			//Joypad interrupt
			if (isKthBitSet(req, 4)) {
				if (isKthBitSet(enabledInterrupt, 4)) {
					doInterrupt(mmu, 4, isHaltedInterr, IMEInter, pcInterr);
				}
			}
		}
	}
}

void Interrupt::doInterrupt(MMU *mmu, uint8_t bitToSearch, bool *isHaltedInterr, bool *IMEInter, uint16_t *pcInterr) {
	if (*isHaltedInterr) {
		*isHaltedInterr = false;
	}
	//Disable the IME
	*IMEInter = false;

	//Reset the bit of that interrupt
	uint8_t requestInterrupt = mmu->read8(0xFF0F);
	requestInterrupt |= (1 << bitToSearch); //Reset a bit
	mmu->write8(0xFF0F, requestInterrupt);


	//We do a push of the actual PC
	mmu->push(*pcInterr);

	//Go to an address depending of the bit
	switch (bitToSearch)
	{
	case 0:
		*pcInterr = 0x40;
		//std::cout << "V-Blank interrupt" << std::endl;
		break;
	case 1:
		*pcInterr = 0x48;
		//std::cout << "LCD interrupt" << std::endl;
		break;
	case 2:
		*pcInterr = 0x50;
		//std::cout << "Timer interrupt" << std::endl;
		break;
	case 3:
		*pcInterr = 0x58;
		break;
	case 4:
		*pcInterr = 0x60;
		std::cout << "Joypad interrupt" << std::endl;
		break;
	default:
		break;
	}
}