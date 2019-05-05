#pragma once
#include <stdio.h>
#include <cstdio>
#include <iostream>
class MMU {
	public:
		uint16_t memory[65536];

		uint16_t read(uint16_t addr);

		uint8_t read8(uint16_t addr);
};