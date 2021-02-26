#pragma once
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include "mmu.h"

class Interrupt
{
public:
	bool isKthBitSet(int n, int k);
	void requestInterrupt(MMU *mmu, uint8_t id);
	void checkForInterrupts(MMU *mmu, bool *isHaltedInterr, bool *IMEInter, uint16_t *pcInterr);
	void doInterrupt(MMU *mmu, uint8_t bitToSearch, bool *isHaltedInterr, bool *IMEInter, uint16_t *pcInterr);

private:

};
