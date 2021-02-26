#include "CPU.h"

MMU mmu;
Interrupt interrupt;
GameboyRegisters reg;
GameboyFlags flags;


uint8_t registerF = 0x00;

int MAXCYCLES = 70224;
float FPS = 59.73f;
float DELAY_TIME = 1000.0f / FPS;

void CPU::init() {
	pc = 0x100;
	//pc = 0x0;
	mmu.setRegisters16Bit(&reg, "AF", 0x01B0);
	mmu.setRegisters16Bit(&reg, "BC", 0x0013);
	mmu.setRegisters16Bit(&reg, "DE", 0x00D8);
	mmu.setRegisters16Bit(&reg, "HL", 0x014D);

	mmu.sp = 0xE000;

	mmu.write8(0xFF00, 0x00);
	mmu.write8(0xFF05, 0x00);
	mmu.write8(0xFF06, 0x00);
	mmu.write8(0xFF07, 0x00);
	mmu.write8(0xFF10, 0x80);
	mmu.write8(0xFF11, 0xBF);
	mmu.write8(0xFF12, 0xF3);
	mmu.write8(0xFF14, 0xBF);
	mmu.write8(0xFF16, 0x3F);
	mmu.write8(0xFF17, 0x00);
	mmu.write8(0xFF19, 0xBF);
	mmu.write8(0xFF1A, 0x7F);
	mmu.write8(0xFF1B, 0xFF);
	mmu.write8(0xFF1C, 0x9F);
	mmu.write8(0xFF1E, 0xBF);
	mmu.write8(0xFF20, 0xFF);
	mmu.write8(0xFF21, 0x00);
	mmu.write8(0xFF22, 0x00);
	mmu.write8(0xFF23, 0xBF);
	mmu.write8(0xFF24, 0x77);
	mmu.write8(0xFF25, 0xF3);
	mmu.write8(0xFF26, 0xF1);
	mmu.write8(0xFF40, 0x91);
	mmu.write8(0xFF42, 0x00);
	mmu.write8(0xFF43, 0x00);
	mmu.write8(0xFF45, 0x00);
	mmu.write8(0xFF47, 0xFC);
	mmu.write8(0xFF48, 0xFF);
	mmu.write8(0xFF49, 0xFF);
	mmu.write8(0xFF4A, 0x00);
	mmu.write8(0xFF4B, 0x00);
	mmu.write8(0xFFFF, 0x00);
}

uint8_t swapNibbles(uint8_t x)
{
	return ((x & 0x0F) << 4 | (x & 0xF0) >> 4);
}

void CPU::addCycles(int ciclesToAdd) {
	cicles += ciclesToAdd;
	//std::cout << static_cast<unsigned>(cicles) << std::endl;
}

void CPU::clearCycles() {
	cicles = 0;
}

bool isKthBitSet(int n, int k)
{
	if (n & (1 << (k - 1))) {
		return true;
	}
	else {
		return false;
	}
}

MMU *CPU::getMMUValues() {
	return &mmu;
}

void CPU::runCPU(GPU *gpu, SDL_Renderer *render) {
	uint8_t cb_opcode;
	uint8_t opcode;
	bool isSlow = false;
	bool isRuning = true;
	clearCycles();
	while (isRuning)
	{
		if (cicles < MAXCYCLES) {

			if (isHalted) {
				interrupt.checkForInterrupts(&mmu, &isHalted, &IME, &pc);
			}
			else {
				interrupt.checkForInterrupts(&mmu, &isHalted, &IME, &pc);
				opcode = mmu.read8(pc);
				//std::cout << "Opcode: " << std::hex << static_cast<unsigned>(opcode) << std::endl;

				//Set the flags 
				if (flags.Z) {
					mmu.setRegisters8Bit(&reg, "F", registerF |= (1 << 7));
				}
				else {
					mmu.setRegisters8Bit(&reg, "F", registerF &= ~(1 << 7));
				}

				if (flags.N) {
					mmu.setRegisters8Bit(&reg, "F", registerF |= (1 << 6));
				}
				else {
					mmu.setRegisters8Bit(&reg, "F", registerF &= ~(1 << 6));
				}

				if (flags.H) {
					mmu.setRegisters8Bit(&reg, "F", registerF |= (1 << 5));
				}
				else {
					mmu.setRegisters8Bit(&reg, "F", registerF &= ~(1 << 5));
				}

				if (flags.C) {
					mmu.setRegisters8Bit(&reg, "F", registerF |= (1 << 4));
				}
				else {
					mmu.setRegisters8Bit(&reg, "F", registerF &= ~(1 << 4));
				}

				/*if (pc == 0x29B0) {
					//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Patch Tetris Aplied", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					mmu.write8(0xFF00, 0x00);
					//isSlow = true;
				}

				if (pc == 0x29BC) {
					//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Patch Tetris Aplied", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					mmu.write8(0xFF00, 0x00);
					//isSlow = true;
				}

				if (pc == 0x02cd) {
					//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Patch Tetris Aplied", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					mmu.write8(0xFF00, 0x00);
					//isSlow = true;
				}*/
				
				/*if (pc == 0x6aa5) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x650f) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x651f) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x6520) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x6521) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x6522) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x6523) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}


				if (pc == 0x6aa7) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x6a69) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}

				if (pc == 0x6a24) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Gameboy Emulator C++", "Breakpoint reached please look gui", NULL);
					cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |  Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
					isSlow = true;
				}*/




				switch (opcode) {
				case 0x00:
					NOP();
					break;
				case 0x10:
					STOP();
					break;
				case 0x1F:
					RRA();
					break;
				case 0x7F: case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E:
				case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x48:
				case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x50: case 0x51:
				case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x58: case 0x59: case 0x36:
				case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x60: case 0x61: case 0x62:
				case 0x63: case 0x64: case 0x65: case 0x66: case 0x68: case 0x69: case 0x6A: case 0x6B:
				case 0x6C: case 0x6D: case 0x6E: case 0x70: case 0x71: case 0x72: case 0x73: case 0x74:
				case 0x75:
					LD_r1_r2(opcode);
					break;
				case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E:
					LD_NN_N(opcode);
					break;
				case 0x0A: case 0x1A: case 0x3E: case 0xFA:
					LD_A_N(opcode);
					break;
				case 0x47: case 0x4F: case 0x57: case 0x5F: case 0x67: case 0x6F: case 0x02: case 0x12:
				case 0x77: case 0xEA:
					LD_N_A(opcode);
					break;
				case 0xC3:
					JP_NN(opcode);
					break;
				case 0xF3:
					DI();
					break;
				case 0xFB:
					EI();
					break;
				case 0xCD:
					CALL_NN(opcode);
					break;
				case 0xC4: case 0xCC: case 0xD4: case 0xDC:
					CALL_CC_NN(opcode);
					break;
				case 0xFE: case 0xBF: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE:
					CP_N(opcode);
					break;
				case 0x20: case 0x28: case 0x30: case 0x38:
					JR_CC_N(opcode);
					break;
				case 0xCA: case 0xC2: case 0xDA: case 0xD2:
					JP_CC_NN(opcode);
					break;
				case 0xE9:
					JP_regHL();
					break;
				case 0x18:
					JR_N();
					break;
				case 0x01: case 0x11: case 0x21: case 0x31:
					LD_N_NN(opcode);
					break;
				case 0x2a:
					LDI_A_regHL();
					break;
				case 0x22:
					LDI_regHL_A();
					break;
				case 0x3c: case 0x34: case 0x04: case 0x0c: case 0x14: case 0x1c: case 0x24: case 0x2c:
					INC_N(opcode);
					break;
				case 0x3d: case 0x35: case 0x05: case 0x0d: case 0x15: case 0x1d: case 0x25: case 0x2d:
					DEC_N(opcode);
					break;
				case 0xc9:
					RET();
					break;
				case 0xd9:
					RETI();
					break;
				case 0x76:
					HALT();
					break;
				case 0xC0: case 0xC8: case 0xD0: case 0xD8:
					RET_CC(opcode);
					break;
				case 0xF0:
					LDH_A_N();
					//std::cout << "LDH A, N  -> PC: " << hex << static_cast<unsigned>(pc) << std::endl;
					break;
				case 0xE0:
					LDH_N_A();
					//std::cout << "LDH A, N  -> PC: " << hex << static_cast<unsigned>(pc) << std::endl;
					break;
				case 0x3A:
					LDD_A_regHL();
					break;
				case 0x32:
					LDD_regHL_A();
					break;
				case 0xAF: case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE: case 0xEE:
					XOR_N(opcode);
					break;
				case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: case 0xF6:
					OR_N(opcode);
					break;
				case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xE6:
					AND_N(opcode);
					break;
				case 0xC7: case 0xCF: case 0xD7: case 0xDF: case 0xE7: case 0xEF: case 0xF7: case 0xFF:
					RST_N(opcode);
					break;
				case 0xE2:
					LD_regC_A();
					break;
				case 0x0B: case 0x1B: case 0x2B: case 0x3B:
					DEC_NN(opcode);
					break;
				case 0x2F:
					CPL();
					break;
				case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0xC6:
					ADD_A_N(opcode);
					break;
				case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: case 0xCE:
					ADC_A_N(opcode);
					break;
				case 0xF1: case 0xD1: case 0xC1: case 0xE1:
					POP_NN(opcode);
					break;
				case 0xF5: case 0xD5: case 0xC5: case 0xE5:
					PUSH_NN(opcode);
					break;
				case 0xF9:
					LD_SP_HL();
					break;
				case 0x08:
					LD_NN_SP();
					break;
				case 0x09: case 0x19: case 0x29: case 0x39:
					ADD_HL_N(opcode);
					break;
				case 0xE8:
					ADD_SP_N();
					break;
				case 0x03: case 0x13: case 0x23: case 0x33:
					INC_NN(opcode);
					break;
				case 0xCB:
					cb_opcode = mmu.read8(pc + 1);
					switch (cb_opcode)
					{
					case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
						CB_SWAP_N(cb_opcode);
						break;
					case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F:
					case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F:
					case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE: case 0xAF:
					case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF:
						RES_B_R(cb_opcode);
						break;
					default:
						cout << "Opcode CB " << hex << static_cast<unsigned>(cb_opcode) << " not implemented, PC: " << hex << static_cast<unsigned>(pc) << endl;
						isRuning = false;
						break;
					}
					break;
				default:
					cout << "Opcode: " << hex << static_cast<unsigned>(opcode) << " not implemented, PC: " << hex << static_cast<unsigned>(pc) << endl;
					isRuning = false;
					break;
				}

				if (isSlow) {
					SDL_Delay(100);
				}

			}
				//getchar();

			gpu->step(cicles, &mmu, render, &interrupt);
		}
		else {
		clearCycles();
		}
	}
}

bool isCarry8bit(int n) {
	return n > 0xFF;
}

bool isCarry16bit(int n) {
	return n > 0xFFFF;
}

bool CPU::isHalfCarry(uint8_t a, uint8_t b, std::string type) {
	if (type == "ADD") {
		return ((a & 0x0F) + (b & 0x0F)) & 0x10 == 0x10;
	}
	else {
		return ((a & 0x0F) - (b & 0x0F)) < 0;
	}
}

bool CPU::isHalfCarry16Bit(uint16_t a, uint16_t b, std::string type) {
	if (type == "ADD") {
		return ((a & 0x00FF) + (b & 0x00FF)) & 0x0100 == 0x0100;
	}
	else {
		return (a & 0x00FF) - (b & 0x00FF) < 0;
	}
}

void CPU::HALT() {
	isHalted = true;
	addCycles(4);
}

void CPU::BIT(uint16_t opcode) {
	switch (opcode)
	{
	case 0x47:
		break;
	case 0x41:
		break;
	case 0x42:
		break;
	case 0x43:
		break;
	case 0x44:
		break;
	case 0x45:
		break;
	case 0x46:
		break;
	default:
		break;
	}
}

void CPU::JP_CC_NN(uint16_t opcode) {
	uint16_t nextAddress = mmu.read(pc + 1);
	switch (opcode)
	{
		case 0xCA:
			if (flags.Z) {
				pc = nextAddress;
			}
			else {
				pc += 3;
			}

			addCycles(12);
			break;

		case 0xC2:
			if (!flags.Z) {
				pc = nextAddress;
			}
			else {
				pc += 3;
			}
			addCycles(12);
			break;

		case 0xDA:
			if (flags.C) {
				pc = nextAddress;
			}
			else {
				pc += 3;
			}
			addCycles(12);
			break;

		case 0xD2:
			if (!flags.C) {
				pc = nextAddress;
			}
			else {
				pc += 3;
			}
			addCycles(12);
			break;
	default:
		break;
	}
}

void CPU::JR_N() {
	int8_t n = mmu.read8(pc + 1);
	//uint16_t nextAddress = pc - (0xff - n) - 1;
	uint16_t nextAddress = pc + n + 2;
	//pc += 2;

	std::cout << std::hex << "Next address: " << static_cast<unsigned>(nextAddress) << std::endl;
	pc = nextAddress;
	addCycles(8);
}

void CPU::RRA() {
	uint8_t regA = reg.A;
	uint8_t a = regA >> 1;
	uint8_t b = regA << 7;
	uint8_t resultRotate = a | b;

	flags.Z = resultRotate == 0 ? true : false;
	flags.C = isKthBitSet(a, 1) ? true : false;
	flags.H = false;
	flags.N = false;

	addCycles(4);
	pc++;
}

void CPU::RET() {
	uint16_t addrPoped = 0;
	mmu.pop(&addrPoped);
	std::cout << "ADDR POPPED: " << std::hex << static_cast<unsigned>(addrPoped) << std::endl;
	addCycles(8);
	pc = addrPoped;
}

void CPU::RETI() {
	uint16_t addrPoped;
	mmu.pop(&addrPoped);
	addCycles(8);
	pc = addrPoped;
	IME = true;
}

void CPU::LDD_A_regHL() {
	mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.HL));
	mmu.setRegisters16Bit(&reg, "HL", reg.HL - 1);
	addCycles(8);
	pc++;
}

void CPU::LDD_regHL_A() {
	//std::cout << "Writing value " << hex << static_cast<unsigned>(reg.A) << ", in address: " << hex << static_cast<unsigned>(reg.HL) << std::endl;
	mmu.write8(reg.HL, reg.A);
	mmu.setRegisters16Bit(&reg, "HL", reg.HL - 1);
	addCycles(8);
	pc++;
}

void CPU::STOP() {
	pc+=2;
	addCycles(4);
}

void CPU::XOR_N(uint16_t opcode) {
	uint8_t n = 0;
	switch (opcode) {
		case 0xAF:
			n = reg.A ^ reg.A;
			//std::cout << "XOR A " << hex << static_cast<unsigned>(n) << std::endl;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n);
			addCycles(4);
			pc++;
			break; 
		case 0xA8:
			n = reg.B ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n); 
			addCycles(4);
			pc++;
			break;
		case 0xA9:
			n = reg.C ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n); 
			addCycles(4);
			pc++;
			break;
		case 0xAA:
			n = reg.D ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n);
			addCycles(4);
			pc++;
			break;
		case 0xAB:
			n = reg.E ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n);
			addCycles(4);
			pc++;
			break;
		case 0xAC:
			n = reg.H ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n);
			addCycles(4);
			pc++;
			break;
		case 0xAD:
			n = reg.L ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n);
			addCycles(4);
			pc++;
			break;
		case 0xAE:
			n = mmu.read8(reg.HL) ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n);
			addCycles(8);
			pc++;
			break;
		case 0xEE:
			n = mmu.read8(pc + 1) ^ reg.A;
			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			flags.C = false;
			flags.N = false;
			flags.H = false;
			mmu.setRegisters8Bit(&reg, "A", n);
			addCycles(8);
			pc++;
			break;
	}
}

void CPU::OR_N(uint16_t opcode) {
	uint8_t n;
	flags.C = false;
	flags.H = false;
	flags.N = false;
	switch (opcode)
	{
		case 0xB7:
			n = reg.A | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0xB0:
			n = reg.B | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0xB1:
			n = reg.C | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0xB2:
			n = reg.D | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0xB3:
			n = reg.E | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0xB4:
			n = reg.H | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0xB5:
			n = reg.L | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0xB6:
			n = mmu.read8(reg.HL) | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc++;
			break;
		case 0xF6:
			n = mmu.read8(pc+1) | reg.A;
			mmu.setRegisters8Bit(&reg, "A", n);

			if (n == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc+=2;
			break;
	default:
		break;
	}
}

void CPU::AND_N(uint16_t opcode) {
	uint8_t n;
	switch (opcode)
	{
	case 0xA7:
		n = reg.A & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0xA0:
		n = reg.B & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0xA1:
		n = reg.C & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0xA2:
		n = reg.D & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0xA3:
		n = reg.E & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0xA4:
		n = reg.H & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0xA5:
		n = reg.L & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0xA6:
		n = mmu.read8(reg.HL) & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(8);
		pc++;
		break;

	case 0xE6:
		n = mmu.read8(pc+1) & reg.A;
		mmu.setRegisters8Bit(&reg, "A", n);

		if (n == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(8);
		pc+=2;
		break;
	default:
		break;
	}

	flags.C = false;
	flags.H = true;
	flags.N = false;
}

void CPU::LD_NN_N(uint16_t opcode) {
	uint8_t n = mmu.read8(pc+1);

	switch (opcode)
	{

	case 0x06:
		//reg.B = n;
		mmu.setRegisters8Bit(&reg, "B", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x0E:
		//reg.C = n;
		mmu.setRegisters8Bit(&reg, "C", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x16:
		//reg.D = n;
		mmu.setRegisters8Bit(&reg, "D", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x1E:
		//reg.E = n;
		mmu.setRegisters8Bit(&reg, "E", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x26:
		//reg.H = n;
		mmu.setRegisters8Bit(&reg, "H", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x2E:
		//reg.L = n;
		mmu.setRegisters8Bit(&reg, "L", n);
		addCycles(8);
		pc+=2;
		break;

	default:
		pc += 2;
		break;
	}
}

void CPU::LD_r1_r2(uint16_t opcode) {
	uint8_t n = mmu.read8(pc + 1);
	switch (opcode)
	{
	case 0x7F:
		//reg.A = reg.A;
		mmu.setRegisters8Bit(&reg, "A", reg.A);
		addCycles(4);
		pc += 1;
		break;
	case 0x78:
		//reg.A = reg.B;
		mmu.setRegisters8Bit(&reg, "A", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x79:
		//reg.A = reg.C;
		mmu.setRegisters8Bit(&reg, "A", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x7a:
		//reg.A = reg.D;
		mmu.setRegisters8Bit(&reg, "A", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x7b:
		//reg.A = reg.E;
		mmu.setRegisters8Bit(&reg, "A", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x7c:
		//reg.A = reg.H;
		mmu.setRegisters8Bit(&reg, "A", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x7d:
		//reg.A = reg.L;
		mmu.setRegisters8Bit(&reg, "A", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x7e:
		//reg.A = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x40:
		//reg.B = reg.B;
		mmu.setRegisters8Bit(&reg, "B", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x41:
		//reg.B = reg.C;
		mmu.setRegisters8Bit(&reg, "B", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x42:
		//reg.B = reg.D;
		mmu.setRegisters8Bit(&reg, "B", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x43:
		//reg.B = reg.E;
		mmu.setRegisters8Bit(&reg, "B", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x44:
		//reg.B = reg.H;
		mmu.setRegisters8Bit(&reg, "B", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x45:
		//reg.B = reg.L;
		mmu.setRegisters8Bit(&reg, "B", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x46:
		//reg.B = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "B", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x48:
		//reg.C = reg.B;
		mmu.setRegisters8Bit(&reg, "C", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x49:
		//reg.C = reg.C;
		mmu.setRegisters8Bit(&reg, "C", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x4A:
		//reg.C = reg.D;
		mmu.setRegisters8Bit(&reg, "C", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x4B:
		//reg.C = reg.E;
		mmu.setRegisters8Bit(&reg, "C", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x4C:
		//reg.C = reg.H;
		mmu.setRegisters8Bit(&reg, "C", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x4D:
		//reg.C = reg.L;
		mmu.setRegisters8Bit(&reg, "C", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x4E:
		//reg.C = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "C", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x50:
		//reg.D = reg.B;
		mmu.setRegisters8Bit(&reg, "D", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x51:
		//reg.D = reg.C;
		mmu.setRegisters8Bit(&reg, "D", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x52:
		//reg.D = reg.D;
		mmu.setRegisters8Bit(&reg, "D", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x53:
		//reg.D = reg.E;
		mmu.setRegisters8Bit(&reg, "D", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x54:
		//reg.D = reg.H;
		mmu.setRegisters8Bit(&reg, "D", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x55:
		//reg.D = reg.L;
		mmu.setRegisters8Bit(&reg, "D", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x56:
		//reg.D = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "D", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x58:
		//reg.E = reg.B;
		mmu.setRegisters8Bit(&reg, "E", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x59:
		//reg.E = reg.C;
		mmu.setRegisters8Bit(&reg, "E", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x5A:
		//reg.E = reg.D;
		mmu.setRegisters8Bit(&reg, "E", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x5B:
		//reg.E = reg.E;
		mmu.setRegisters8Bit(&reg, "E", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x5C:
		//reg.E = reg.H;
		mmu.setRegisters8Bit(&reg, "E", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x5D:
		//reg.E = reg.L;
		mmu.setRegisters8Bit(&reg, "E", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x5E:
		//reg.E = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "E", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x60:
		//reg.H = reg.B;
		mmu.setRegisters8Bit(&reg, "H", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x61:
		//reg.H = reg.C;
		mmu.setRegisters8Bit(&reg, "H", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x62:
		//reg.H = reg.D;
		mmu.setRegisters8Bit(&reg, "H", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x63:
		//reg.H = reg.E;
		mmu.setRegisters8Bit(&reg, "H", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x64:
		//reg.H = reg.H;
		mmu.setRegisters8Bit(&reg, "H", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x65:
		//reg.H = reg.L;
		mmu.setRegisters8Bit(&reg, "H", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x66:
		//reg.H = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "H", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x68:
		//reg.L = reg.B;
		mmu.setRegisters8Bit(&reg, "L", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x69:
		//reg.L = reg.C;
		mmu.setRegisters8Bit(&reg, "L", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x6A:
		//reg.L = reg.D;
		mmu.setRegisters8Bit(&reg, "L", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x6B:
		//reg.L = reg.E;
		mmu.setRegisters8Bit(&reg, "L", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x6C:
		//reg.L = reg.H;
		mmu.setRegisters8Bit(&reg, "L", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x6D:
		//reg.L = reg.L;
		mmu.setRegisters8Bit(&reg, "L", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x6E:
		//reg.L = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "L", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x70:
		mmu.write8(reg.HL, reg.B);
		addCycles(8);
		pc += 1;
		break;
	case 0x71:
		mmu.write8(reg.HL, reg.C);
		addCycles(8);
		pc += 1;
		break;
	case 0x72:
		mmu.write8(reg.HL, reg.D);
		addCycles(8);
		pc += 1;
		break;
	case 0x73:
		mmu.write8(reg.HL, reg.E);
		addCycles(8);
		pc += 1;
		break;
	case 0x74:
		mmu.write8(reg.HL, reg.H);
		addCycles(8);
		pc += 1;
		break;
	case 0x75:
		mmu.write8(reg.HL, reg.L);
		addCycles(8);
		pc += 1;
		break;
	case 0x36:
		mmu.write8(reg.HL, n);
		addCycles(12);
		pc += 2;
		break;
	default:
		pc += 1;
		break;
	}
}

void CPU::LD_A_N(uint16_t opcode) {
	uint16_t nn = mmu.read(pc + 1);
	uint8_t n = mmu.read8(pc + 1);
	switch (opcode)
	{
	
	case 0x0A:
		//std::cout << "LD A, (BC)" << endl;
		//reg.A = mmu.read8(reg.BC);
		mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.BC));
		pc+=1;
		addCycles(8);
		break;
	case 0x1A:
		//std::cout << "LD A, (DE)" << endl;
		//reg.A = mmu.read8(reg.DE);
		mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.DE));
		pc+=1;
		addCycles(8);
		break;
	case 0xFA:
		//std::cout << "LD A, (" << hex << static_cast<unsigned>(nn) << "h)" << endl;
		//reg.A = mmu.read8(nn);
		mmu.setRegisters8Bit(&reg, "A", mmu.read(nn));
		pc += 3;
		addCycles(16);
		break;
	case 0x3E:
		//std::cout << "LD A, " << hex << static_cast<unsigned>(n) << "h)" << endl;
		//reg.A = mmu.read8(n);
		//std::cout << "Value to set in A: " << hex << static_cast<unsigned>(n) << std::endl;
		mmu.setRegisters8Bit(&reg, "A", n);
		pc+=2;
		addCycles(8);
		break;
	default:
		break;
	}
}

void CPU::LD_N_A(uint16_t opcode) {
	uint16_t nn = mmu.read(pc + 1);
	switch (opcode)
	{
	case 0x47:
		//cout << "LD B, A" << endl;
		//reg.B = reg.A;
		mmu.setRegisters8Bit(&reg, "B", reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x4F:
		//cout << "LD C, A" << endl;
		//reg.C = reg.A;
		mmu.setRegisters8Bit(&reg, "C", reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x57:
		//cout << "LD D, A" << endl;
		mmu.setRegisters8Bit(&reg, "D", reg.A);
		//reg.D = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x5F:
		//cout << "LD E, A" << endl;
		mmu.setRegisters8Bit(&reg, "E", reg.A);
		//reg.E = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x67:
		//cout << "LD H, A" << endl;
		mmu.setRegisters8Bit(&reg, "H", reg.A);
		//reg.H = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x6F:
		//cout << "LD L, A" << endl;
		mmu.setRegisters8Bit(&reg, "L", reg.A);
		//reg.L = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x02:
		//cout << "LD (BC), A" << endl;
		mmu.write8(reg.BC, reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x12:
		//cout << "LD (DE), A" << endl;
		mmu.write8(reg.DE, reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x77:
		//cout << "LD (HL), A" << endl;
		mmu.write8(reg.HL, reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0xEA:
		//cout << "LD (" << hex << static_cast<unsigned>(nn) << "h), A" << endl;
		//reg.B = reg.A;
		mmu.write8(nn, reg.A);
		pc += 3; 
		addCycles(4);
		break;
	}
}

void CPU::LD_N_NN(uint16_t opcode) {
	uint16_t nn = mmu.read(pc + 1);
	switch (opcode)
	{
	case 0x01:
		//reg.BC = nn;
		//cout << "LD BC, " << hex << nn << endl;
		mmu.setRegisters16Bit(&reg, "BC", nn);
		pc += 3;
		addCycles(12);
		break;
	case 0x11:
		//reg.DE = nn;
		//cout << "LD DE, " << hex << nn << endl;
		mmu.setRegisters16Bit(&reg, "DE", nn);
		pc += 3;
		addCycles(12);
		break;
	case 0x21:
		//reg.HL = nn;
		//cout << "LD HL, " << hex << nn << endl;
		mmu.setRegisters16Bit(&reg, "HL", nn);
		pc += 3;
		addCycles(12);
		break;
	case 0x31:
		mmu.sp = nn;
		//cout << "LD SP, " << hex << nn << endl;
		pc += 3;
		addCycles(12);
		break;
	}
}

void CPU::NOP() {
	pc++;
	//cout << "NOP" << endl;
	addCycles(4);
}

void CPU::JP_NN(uint16_t opcode) {
	pc = mmu.read(pc+1);
	//cout << "JP, " << hex << pc << "h" << endl;
	addCycles(12);
}

void CPU::JP_regHL() {
	pc = reg.HL;
	addCycles(4);
}

void CPU::DI() {
	IME = false;
	//cout << "DI" << endl;
	addCycles(4);
	pc++;
}

void CPU::EI() {
	IME = true;
	//cout << "EI" << endl;
	addCycles(4);
	pc++;
}

void CPU::CALL_CC_NN(uint16_t opcode) {
	switch (opcode)
	{
	case 0xC4:
		if (!flags.Z) {
			uint16_t nn = mmu.read(pc + 1);
			//cout << "CALL, " << static_cast<unsigned>(nn) << "h" << endl;
			mmu.push(pc + 3);
			//std::cout << "CALL, " << hex << nn << "h, PC: " << hex << static_cast<unsigned>(pc) << std::endl;
			pc = nn;
			addCycles(12);
		}
		else {
			pc += 3;
		}
		break;

	case 0xCC:
		if (flags.Z) {
			uint16_t nn = mmu.read(pc + 1);
			//cout << "CALL, " << static_cast<unsigned>(nn) << "h" << endl;
			mmu.push(pc + 3);
			//std::cout << "CALL, " << hex << nn << "h, PC: " << hex << static_cast<unsigned>(pc) << std::endl;
			pc = nn;
			addCycles(12);
		}
		else {
			pc += 3;
		}
		break;

	case 0xD4:
		if (!flags.C) {
			uint16_t nn = mmu.read(pc + 1);
			//cout << "CALL, " << static_cast<unsigned>(nn) << "h" << endl;
			mmu.push(pc + 3);
			//std::cout << "CALL, " << hex << nn << "h, PC: " << hex << static_cast<unsigned>(pc) << std::endl;
			pc = nn;
			addCycles(12);
		}
		else {
			pc += 3;
		}
		break;

	case 0xDC:
		if (flags.C) {
			uint16_t nn = mmu.read(pc + 1);
			//cout << "CALL, " << static_cast<unsigned>(nn) << "h" << endl;
			mmu.push(pc + 3);
			//std::cout << "CALL, " << hex << nn << "h, PC: " << hex << static_cast<unsigned>(pc) << std::endl;
			pc = nn;
			addCycles(12);
		}
		else {
			pc += 3;
		}
		break;

	default:
		break;
	}
}

void CPU::CALL_NN(uint16_t opcode) {
	uint16_t nn = mmu.read(pc+1);
	//cout << "CALL, " << static_cast<unsigned>(nn) << "h" << endl;
	mmu.push(pc+3);
	//std::cout << "CALL, " << hex << nn << "h, PC: " << hex << static_cast<unsigned>(pc) << std::endl;
	pc = nn;
	addCycles(12);
}

void CPU::LDH_N_A() {
	uint8_t n = mmu.read8(pc + 1);
	uint16_t memoryAdress = 0xFF00 + n;
	mmu.write8(memoryAdress, reg.A);
	addCycles(12);
	pc += 2;
}

void CPU::LDH_A_N() {
	uint8_t n = mmu.read8(pc + 1);
	uint16_t memoryAdress = 0xFF00 + n;
	//std::cout << "LDH A," << hex << " (" << hex << static_cast<unsigned>(memoryAdress) << "h) so A will be " << static_cast<unsigned>(mmu.read8(memoryAdress))<< std::endl;
	mmu.setRegisters8Bit(&reg, "A", mmu.read8(memoryAdress)); 
	addCycles(12);
	pc += 2;
}

void CPU::CP_N(uint16_t opcode) {
	uint8_t n;
	uint16_t nn;
	
	switch (opcode) {
	case 0xFE:
		n = mmu.read8(pc+1);
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}

		pc += 2;
		addCycles(8);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBF:
		n = reg.A;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xB8:
		n = reg.B;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xB9:
		n = reg.C;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBA:
		n = reg.D;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBB:
		n = reg.E;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBC:
		n = reg.H;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBD:
		n = reg.L;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		//cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBE:
		nn = mmu.read(reg.HL);
		if (reg.A == nn) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A < nn) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, nn, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(8);
		//cout << "CP, A " << hex << static_cast<unsigned>(nn) << "h" << endl;
		break;
	default:
		pc+=2;
	}

}

void CPU::JR_CC_N(uint16_t opcode) {
	int8_t n = mmu.read8(pc+1); 
	uint16_t nextAddress = pc + n + 2;
	//uint16_t nextAddress = pc - (0xFF - n) - 1;
	switch (opcode) {
		//JR NZ, N
	case 0x20:
		//std::cout << "Flag Z when calling JR NZ " << flags.Z << std::endl;
			if (!flags.Z) {

				pc = nextAddress;
				//std::cout << "PC: " << pc << endl;
			}
			else {
				pc += 2;
			}
			//cout << "JR NZ, " << hex << pc << endl;
			addCycles(8);
		
		break;
		//JR Z, N
	case 0x28: 
			if (flags.Z) {
				pc = nextAddress;
			}
			else {
				pc += 2;
			}
			addCycles(8);
			cout << "JR Z, " << hex << pc << endl;
		
		break;
		//JR NC, N
	case 0x30: 
			if (!flags.C) {
				pc = nextAddress;
			}
			else {
				pc += 2;
			}
			addCycles(8);
			//cout << "JR NC, " << hex << pc << endl;
		
		break;
		//JR C, N
	case 0x38: 
			if (flags.C) {
				pc = nextAddress;
			}
			else {
				pc += 2;
			}
			addCycles(8);
			//cout << "JR C, " << hex << pc << endl;
		
		break;

	}
}

void CPU::INC_N(uint16_t opcode) {
	flags.N = false;
	switch (opcode)
	{
		case 0x3C:
			//std::cout << "INC A" << endl;			
			if (isHalfCarry(reg.A, 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.setRegisters8Bit(&reg, "A", reg.A + 1);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0x04:
			//std::cout << "INC B" << endl;
			if (isHalfCarry(reg.B, 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.setRegisters8Bit(&reg, "B", reg.B + 1);
			if (reg.B == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0x0C:
			//std::cout << "INC C" << endl;
			if (isHalfCarry(reg.C, 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.setRegisters8Bit(&reg, "C", reg.C + 1);
			if (reg.C == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			addCycles(4);
			pc++;
			break;
		case 0x14:
			//std::cout << "INC D" << endl;			
			if (isHalfCarry(reg.D, 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.setRegisters8Bit(&reg, "D", reg.D + 1);
			if (reg.D == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0x1C:
			//std::cout << "INC E" << endl;
			if (isHalfCarry(reg.E, 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.setRegisters8Bit(&reg, "E", reg.E + 1);
			if (reg.E == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0x24:
			//std::cout << "INC H" << endl;			
			if (isHalfCarry(reg.H, 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.setRegisters8Bit(&reg, "H", reg.H + 1);
			if (reg.H == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0x2C:
			//std::cout << "INC L" << endl;
			if (isHalfCarry(reg.L, 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.setRegisters8Bit(&reg, "L", reg.L + 1);
			if (reg.L == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(4);
			pc++;
			break;
		case 0x34:
			//std::cout << "INC (HL)" << endl;
			if (isHalfCarry(mmu.read8(reg.HL), 1, "ADD")) {
				flags.H = true;
			}
			else {
				flags.H = false;
			}
			mmu.write8(reg.HL, mmu.read8(reg.HL) + 1);
			if (mmu.read8(reg.HL) == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(12);
			pc++;
			break;
		default:
			break;
	}
}

void CPU::DEC_N(uint16_t opcode) {

	flags.N = true;
	switch (opcode)
	{
	case 0x3D:
		//std::cout << "DEC A" << endl;
		if (isHalfCarry(reg.A, 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		mmu.setRegisters8Bit(&reg, "A", reg.A - 1);
		if (reg.A == 0) {
			flags.Z = true;
		}else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0x05:
		//std::cout << "DEC B" << endl;

		if (isHalfCarry(reg.B, 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		mmu.setRegisters8Bit(&reg, "B", reg.B - 1);
		if (reg.B == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0x0D:
		//std::cout << "DEC C" << endl;
		if (isHalfCarry(reg.C, 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		mmu.setRegisters8Bit(&reg, "C", reg.C - 1);
		if (reg.C == 0) {
			flags.Z = true;
		}else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0x15:
		//std::cout << "DEC D" << endl;
		if (isHalfCarry(reg.D, 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		mmu.setRegisters8Bit(&reg, "D", reg.D - 1);
		if (reg.D == 0) {
			flags.Z = true;
		}else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0x1D:
		//std::cout << "DEC E" << endl;
		if (isHalfCarry(reg.E, 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		mmu.setRegisters8Bit(&reg, "E", reg.E - 1);
		if (reg.E == 0) {
			flags.Z = true;
		}else {
			flags.Z = false;
		}

		addCycles(4);
		pc++;
		break;
	case 0x25:
		//std::cout << "DEC H" << endl;
		if (isHalfCarry(reg.H, 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}

		mmu.setRegisters8Bit(&reg, "H", reg.H - 1);
		if (reg.H == 0) {
			flags.Z = true;
		}else {
			flags.Z = false;
		}

		addCycles(4);
		pc++;
		break;
	case 0x2D:
		//std::cout << "DEC L" << endl;

		if (isHalfCarry(reg.L, 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}

		mmu.setRegisters8Bit(&reg, "L", reg.L - 1);
		if (reg.L == 0) {
			flags.Z = true;
		}else {
			flags.Z = false;
		}
		addCycles(4);
		pc++;
		break;
	case 0x35:
		//std::cout << "DEC (HL)" << endl;		
		if (isHalfCarry(mmu.read8(reg.HL), 1, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		mmu.write8(reg.HL, mmu.read8(reg.HL) - 1);
		if (mmu.read8(reg.HL) == 0) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}
		addCycles(12);
		pc++;
		break;
	default:
		break;
	}
}

void CPU::LDI_regHL_A() {
	//std::cout << "LDI (HL), A" << endl;
	mmu.write8(reg.HL, reg.A);
	mmu.setRegisters16Bit(&reg, "HL", reg.HL + 1);
	addCycles(8);
	pc++;
}

void CPU::LDI_A_regHL() {
	//std::cout << "LDI A, (HL)" << endl;
	mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.HL));
	mmu.setRegisters16Bit(&reg, "HL", reg.HL + 1);
	addCycles(8);
	pc++;
}

void CPU::RST_N(uint16_t opcode) {
	switch (opcode)
	{
		case 0xC7:
			mmu.push(pc+1);
			pc = 0x0000 + 0x0000;
			addCycles(32);
			break;
		case 0xCF:
			mmu.push(pc + 1);
			pc = 0x0000 + 0x0008;
			addCycles(32);
			break;
		case 0xD7:
			mmu.push(pc + 1);
			pc = 0x0000 + 0x0010;
			addCycles(32);
			break;
		case 0xDF:
			mmu.push(pc + 1);
			pc = 0x0000 + 0x0018;
			addCycles(32);
			break;
		case 0xE7:
			mmu.push(pc + 1);
			pc = 0x0000 + 0x0020;
			addCycles(32);
			break;
		case 0xEF:
			mmu.push(pc + 1);
			pc = 0x0000 + 0x0028;
			addCycles(32);
			break;
		case 0xF7:
			mmu.push(pc + 1);
			pc = 0x0000 + 0x0030;
			addCycles(32);
			break;
		case 0xFF:
			mmu.push(pc + 1);
			pc = 0x0000 + 0x0038;
			addCycles(32);
			break;
	default:
		break;
	}
}

void CPU::LD_regC_A() {
	uint16_t n = 0xFF00 + reg.C;
	mmu.write8(n, reg.A);
	pc++;
	addCycles(8);
}

void CPU::DEC_NN(uint16_t opcode) {
	switch (opcode)
	{
		case 0x0B:
			mmu.setRegisters16Bit(&reg, "BC", reg.BC - 1);
			pc++;
			addCycles(8);
			break;
		case 0x1B:
			mmu.setRegisters16Bit(&reg, "DE", reg.DE - 1);
			pc++;
			addCycles(8);
			break;
		case 0x2B:
			mmu.setRegisters16Bit(&reg, "HL", reg.HL - 1);
			pc++;
			addCycles(8);
			break;
		case 0x3B:
			mmu.sp--;
			pc++;
			addCycles(8);
			break;
	default:
		break;
	}
}

void CPU::CPL() {
	uint8_t registerA = reg.A;
	uint8_t regAFlipped = ~registerA;
	mmu.setRegisters8Bit(&reg, "A", regAFlipped);	
	addCycles(4);
	flags.N = true;
	flags.H = true;
	pc++;
}

void CPU::CB_SWAP_N(uint16_t opcode) {
	uint8_t swappedNibles;

	flags.N = false;
	flags.H = false;
	flags.C = false;

	switch (opcode)
	{
		case 0x37:
			swappedNibles = swapNibbles(reg.A);
			mmu.setRegisters8Bit(&reg, "A", swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc+=2;
			break;
		case 0x30:
			swappedNibles = swapNibbles(reg.B);
			mmu.setRegisters8Bit(&reg, "B", swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc += 2;
			break;
		case 0x31:
			swappedNibles = swapNibbles(reg.C);
			mmu.setRegisters8Bit(&reg, "C", swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc += 2;
			break;
		case 0x32:
			swappedNibles = swapNibbles(reg.D);
			mmu.setRegisters8Bit(&reg, "D", swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc += 2;
			break;
		case 0x33:
			swappedNibles = swapNibbles(reg.E);
			mmu.setRegisters8Bit(&reg, "E", swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc += 2;
			break;
		case 0x34:
			swappedNibles = swapNibbles(reg.H);
			mmu.setRegisters8Bit(&reg, "H", swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc += 2;
			break;
		case 0x35:
			swappedNibles = swapNibbles(reg.L);
			mmu.setRegisters8Bit(&reg, "L", swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(8);
			pc += 2;
			break;
		case 0x36:
			swappedNibles = swapNibbles(mmu.read8(reg.HL));
			mmu.write8(reg.HL, swappedNibles);
			if (swappedNibles == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}
			addCycles(16);
			pc += 2;
			break;
	default:
		break;
	}
}

void CPU::ADD_A_N(uint16_t opcode){
	uint8_t n;
	switch (opcode)
	{
		case 0x80:
			flags.H = isHalfCarry(reg.A, reg.B, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + reg.B);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x81:
			flags.H = isHalfCarry(reg.A, reg.C, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + reg.C);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x82:
			flags.H = isHalfCarry(reg.A, reg.D, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + reg.D);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x83:
			flags.H = isHalfCarry(reg.A, reg.E, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + reg.E);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x84:
			flags.H = isHalfCarry(reg.A, reg.H, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + reg.H);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x85:
			flags.H = isHalfCarry(reg.A, reg.L, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + reg.L);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x86:
			flags.H = isHalfCarry(reg.A, mmu.read8(reg.HL), "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + mmu.read8(reg.HL));
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(8);
			pc++;
			break;
		case 0x87:
			flags.H = isHalfCarry(reg.A, reg.A, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + reg.A);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0xC6:
			n = mmu.read8(pc + 1);
			flags.H = isHalfCarry(reg.A, n, "ADD");
			mmu.setRegisters8Bit(&reg, "A", reg.A + n);
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(8);
			pc+=2;
			break;
	default:
		break;
	}
}

void CPU::ADC_A_N(uint16_t opcode) {
	uint8_t n;
	switch (opcode)
	{
		case 0x88:
			n = reg.A + reg.B;
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, reg.B + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, reg.B, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x89:
			n = reg.A + reg.C;
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, reg.C + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, reg.C, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x8A:
			n = reg.A + reg.D;
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, reg.D + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, reg.D, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x8B:
			n = reg.A + reg.E;
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, reg.E + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, reg.E, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x8C:
			n = reg.A + reg.H;
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, reg.H + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, reg.H, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x8D:
			n = reg.A + reg.L;
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, reg.L + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, reg.L, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x8E:
			n = reg.A + mmu.read8(reg.HL);
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, mmu.read8(reg.HL) + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, mmu.read8(reg.HL), "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(4);
			pc++;
			break;
		case 0x8F:
			n = reg.A + reg.A;
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, reg.A + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, reg.A, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(8);
			pc++;
			break;
		case 0xCE:
			n = reg.A + mmu.read8(pc+1);
			if (isCarry8bit(n)) {
				flags.H = isHalfCarry(reg.A, mmu.read8(pc + 1) + 1, "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 1);
			}
			else {
				flags.H = isHalfCarry(reg.A, mmu.read8(pc + 1), "ADD");
				mmu.setRegisters8Bit(&reg, "A", n + 0);
			}
			if (reg.A == 0) {
				flags.Z = true;
			}
			else {
				flags.Z = false;
			}

			flags.C = isCarry8bit(reg.A);
			flags.N = false;
			addCycles(8);
			pc+=2;
			break;
	default:
		break;
	}
}

void CPU::POP_NN(uint16_t opcode) {
	uint16_t nn;
	switch (opcode)
	{
		case 0xF1:
			mmu.pop(&nn);
			mmu.setRegisters16Bit(&reg, "AF", nn);
			pc++;
			addCycles(12);
			break;
		case 0xC1:
			mmu.pop(&nn);
			mmu.setRegisters16Bit(&reg, "BC", nn);
			pc++;
			addCycles(12);
			break;
		case 0xD1:
			mmu.pop(&nn);
			mmu.setRegisters16Bit(&reg, "DE", nn);
			//std::cout << "POP HL, PC value is: " << hex << static_cast<unsigned>(pc) << ", Value is: " << hex << static_cast<unsigned>(nn) << std::endl;
			pc++;
			addCycles(12);
			break;
		case 0xE1:
			mmu.pop(&nn);
			mmu.setRegisters16Bit(&reg, "HL", nn);
			//std::cout << "POP HL, PC value is: " << hex << static_cast<unsigned>(pc) << ", Value is: " << hex << static_cast<unsigned>(nn) << std::endl;
			pc++;
			addCycles(12);
			break;
	default:
		break;
	}
}

void CPU::PUSH_NN(uint16_t opcode) {
	uint16_t nn;
	switch (opcode)
	{
	case 0xF5:
		//std::cout << "Pushing AF: " << hex << static_cast<unsigned>(reg.AF) << std::endl;
		mmu.push(reg.AF);
		pc++;
		addCycles(16);
		break;
	case 0xC5:
		//std::cout << "Pushing BC: " << hex << static_cast<unsigned>(reg.BC) << std::endl;
		mmu.push(reg.BC);
		pc++;
		addCycles(16);
		break;
	case 0xD5:
		//std::cout << "Pushing DE: " << hex << static_cast<unsigned>(reg.DE) << std::endl;
		mmu.push(reg.DE);
		pc++;
		addCycles(16);
		break;
	case 0xE5:
		//std::cout << "Pushing HL: " << hex << static_cast<unsigned>(reg.HL) << std::endl;
		mmu.push(reg.HL);
		pc++;
		addCycles(16);
		break;
	default:
		break;
	}
}

void CPU::LD_SP_HL() {
	mmu.sp = reg.HL;
	pc++;
	addCycles(8);
}

void CPU::LD_NN_SP() {
	uint16_t nn = mmu.read(pc + 1);
	mmu.write(nn, mmu.sp);
	pc += 3;
	addCycles(20);
}

void CPU::ADD_HL_N(uint16_t opcode) {
	uint16_t nn;
	switch (opcode)
	{
		case 0x09:
			nn = reg.HL + reg.BC;
			flags.H = isHalfCarry16Bit(reg.HL, reg.BC, "ADD");
			mmu.setRegisters16Bit(&reg, "HL", nn);
			flags.C = isCarry16bit(reg.HL);
			flags.N = false;
			addCycles(8);
			pc++;
			break;
		case 0x19:
			nn = reg.HL + reg.DE;
			flags.H = isHalfCarry16Bit(reg.HL, reg.DE, "ADD");
			mmu.setRegisters16Bit(&reg, "HL", nn);
			flags.C = isCarry16bit(reg.HL);
			flags.N = false;
			addCycles(8);
			pc++;
			break;
		case 0x29:
			nn = reg.HL + reg.HL;
			flags.H = isHalfCarry16Bit(reg.HL, reg.HL, "ADD");
			mmu.setRegisters16Bit(&reg, "HL", nn);
			flags.C = isCarry16bit(reg.HL);
			flags.N = false;
			addCycles(8);
			pc++;
			break;
		case 0x39:
			nn = reg.HL + mmu.sp;
			flags.H = isHalfCarry16Bit(reg.HL, mmu.sp, "ADD");
			mmu.setRegisters16Bit(&reg, "HL", nn);
			flags.C = isCarry16bit(reg.HL);
			flags.N = false;
			addCycles(8);
			pc++;
			break;
	default:
		break;
	}
}

void CPU::ADD_SP_N() {
	uint8_t n = mmu.read8(pc + 1);
	flags.H = isHalfCarry16Bit(mmu.sp, (uint16_t)n, "ADD");
	mmu.sp += n;
	flags.Z = false;
	flags.N = false;
	flags.C = isCarry16bit(mmu.sp);
	pc += 2;
	addCycles(16);
}

void CPU::INC_NN(uint16_t opcode) {
	switch (opcode) {
		case 0x03:
			mmu.setRegisters16Bit(&reg, "BC", reg.BC + 1);
			addCycles(8);
			pc++;
			break;
		case 0x13:
			mmu.setRegisters16Bit(&reg, "DE", reg.DE + 1);
			addCycles(8);
			pc++;
			break;
		case 0x23:
			mmu.setRegisters16Bit(&reg, "HL", reg.HL + 1);
			addCycles(8);
			pc++;
			break;
		case 0x33:
			mmu.sp++;
			addCycles(8);
			pc++;
			break;
	}

}

void CPU::RES_B_R(uint16_t opcode) {
	uint8_t n;
	switch (opcode)
	{
		case 0x80:
			n = reg.B & ~(1 << 0);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0x81:
			n = reg.C & ~(1 << 0);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0x82:
			n = reg.D & ~(1 << 0);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0x83:
			n = reg.E & ~(1 << 0);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0x84:
			n = reg.H & ~(1 << 0);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0x85:
			n = reg.L & ~(1 << 0);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0x86:
			n = mmu.read8(reg.HL) & ~(1 << 0);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(16);
			break;
		case 0x87:
			n = (reg.A & ~(1 << 0));
			//std::cout << "Reseting bit 0 of A: " << hex << static_cast<unsigned>(reg.A) << " with reg.A & ~(1 << 0): " << hex << static_cast<unsigned>(n) << std::endl;
			mmu.setRegisters8Bit(&reg, "A", n);
			pc++;
			addCycles(8);
			break;
		case 0x88:
			n = reg.B & ~(1 << 1);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0x89:
			n = reg.C & ~(1 << 1);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0x8A:
			n = reg.D & ~(1 << 1);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0x8B:
			n = reg.E & ~(1 << 1);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0x8C:
			n = reg.H & ~(1 << 1);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0x8D:
			n = reg.L & ~(1 << 1);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0x8E:
			n = mmu.read8(reg.HL) & ~(1 << 0);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(16);
			break;
		case 0x8F:
			n = reg.A & ~(1 << 1);
			mmu.setRegisters8Bit(&reg, "A", n);
			pc++;
			addCycles(8);
			break;		
		case 0x90:
			n = reg.B & ~(1 << 2);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0x91:
			n = reg.C & ~(1 << 2);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0x92:
			n = reg.D & ~(1 << 2);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0x93:
			n = reg.E & ~(1 << 2);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0x94:
			n = reg.H & ~(1 << 2);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0x95:
			n = reg.L & ~(1 << 2);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0x96:
			n = mmu.read8(reg.HL) & ~(1 << 2);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(16);
			break;
		case 0x97:
			n = reg.A & ~(1 << 2);
			mmu.setRegisters8Bit(&reg, "A", n);
			pc++;
			addCycles(8);
			break;
		case 0x98:
			n = reg.B & ~(1 << 3);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0x99:
			n = reg.B & ~(1 << 3);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0x9A:
			n = reg.C & ~(1 << 3);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0x9B:
			n = reg.D & ~(1 << 3);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0x9C:
			n = reg.E & ~(1 << 3);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0x9D:
			n = reg.H & ~(1 << 3);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0x9E:
			n = reg.L & ~(1 << 3);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0x9F:
			n = mmu.read8(reg.HL) & ~(1 << 3);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(8);
			break;
		case 0xA0:
			n = reg.B & ~(1 << 4);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0xA1:
			n = reg.C & ~(1 << 4);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0xA2:
			n = reg.D & ~(1 << 4);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0xA3:
			n = reg.E & ~(1 << 4);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0xA4:
			n = reg.H & ~(1 << 4);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0xA5:
			n = reg.L & ~(1 << 4);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0xA6:
			n = mmu.read8(reg.HL) & ~(1 << 4);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(16);
			break;
		case 0xA7:
			n = reg.A & ~(1 << 4);
			mmu.setRegisters8Bit(&reg, "A", n);
			pc++;
			addCycles(8);
			break;
		case 0xA8:
			n = reg.B & ~(1 << 5);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0xA9:
			n = reg.C & ~(1 << 5);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0xAA:
			n = reg.D & ~(1 << 5);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0xAB:
			n = reg.E & ~(1 << 5);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0xAC:
			n = reg.H & ~(1 << 5);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0xAD:
			n = reg.L & ~(1 << 5);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0xAE:
			n = mmu.read8(reg.HL) & ~(1 << 5);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(16);
			break;
		case 0xAF:
			n = reg.A & ~(1 << 5);
			mmu.setRegisters8Bit(&reg, "A", n);
			pc++;
			addCycles(8);
			break;
		case 0xB0:
			n = reg.B & ~(1 << 6);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0xB1:
			n = reg.C & ~(1 << 6);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0xB2:
			n = reg.D & ~(1 << 6);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0xB3:
			n = reg.E & ~(1 << 6);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0xB4:
			n = reg.H & ~(1 << 6);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0xB5:
			n = reg.L & ~(1 << 6);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0xB6:
			n = mmu.read8(reg.HL) & ~(1 << 6);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(16);
			break;
		case 0xB7:
			n = reg.A & ~(1 << 6);
			mmu.setRegisters8Bit(&reg, "A", n);
			pc++;
			addCycles(8);
			break;
		case 0xB8:
			n = reg.B & ~(1 << 7);
			mmu.setRegisters8Bit(&reg, "B", n);
			pc++;
			addCycles(8);
			break;
		case 0xB9:
			n = reg.C & ~(1 << 7);
			mmu.setRegisters8Bit(&reg, "C", n);
			pc++;
			addCycles(8);
			break;
		case 0xBA:
			n = reg.D & ~(1 << 7);
			mmu.setRegisters8Bit(&reg, "D", n);
			pc++;
			addCycles(8);
			break;
		case 0xBB:
			n = reg.E & ~(1 << 7);
			mmu.setRegisters8Bit(&reg, "E", n);
			pc++;
			addCycles(8);
			break;
		case 0xBC:
			n = reg.H & ~(1 << 7);
			mmu.setRegisters8Bit(&reg, "H", n);
			pc++;
			addCycles(8);
			break;
		case 0xBD:
			n = reg.L & ~(1 << 7);
			mmu.setRegisters8Bit(&reg, "L", n);
			pc++;
			addCycles(8);
			break;
		case 0xBE:
			n = mmu.read8(reg.HL) & ~(1 << 7);
			mmu.write8(reg.HL, n);
			pc++;
			addCycles(16);
			break;
		case 0xBF:
			n = reg.A & ~(1 << 7);
			mmu.setRegisters8Bit(&reg, "A", n);
			pc++;
			addCycles(8);
			break;
	default:
		break;
	}
}

void CPU::RET_CC(uint16_t opcode) {
	switch (opcode)
	{
	case 0xC0:
		if (!flags.Z) {
			RET();
			addCycles(8);
		}
		else {
			pc++;
		}
		break;

	case 0xC8:
		if (flags.Z) {
			RET();
			addCycles(8);
		}
		else {
			pc++;
		}
		break;

	case 0xD0:
		if (!flags.C) {
			RET();
			addCycles(8);
		}
		else {
			pc++;
		}
		break;

	case 0xD8:
		if (flags.C) {
			RET();
			addCycles(8);
		}
		else {
			pc++;
		}
		break;

	default:
		break;
	}
}

GameboyFlags *CPU::getFlagState() {
	return &flags;
}

GameboyRegisters *CPU::getGameboyRegisters() {
	return &reg;
}

//Load the game into memory
void CPU::loadGame(const char* path) {
	//Holds the game
	ifstream game;
	game.open(path, ifstream::binary);
	
	//Set pos at the end
	game.seekg(0, ifstream::end);
	
	//Holds the size of the game
	int gamesize = game.tellg();
	cout << "Size is: " << gamesize << endl;
	
	//Set pos at the beginning
	game.seekg(0, ifstream::beg);
	char* tempGame = new char[gamesize];

	//Dump content into the buffer we created
	game.read(tempGame, gamesize);
	
	//Now dump the content in memory space desired
	for (int x = 0; x < gamesize; x++) {
		mmu.rom[x] = tempGame[x];
		//std::cout << "Address is " << x << ", Value of that is: " << hex << static_cast<unsigned>(mmu.rom[x]) << endl;
	}
	std::cout << "Game loaded into memory" << endl;
	game.close();
}


void CPU::loadBIOS() {
	const uint8_t bios[0x100] = {
	0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
	0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
	0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
	0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
	0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
	0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
	0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
	0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
	0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
	0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
	0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
	0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
	0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
	0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
	0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
	};

	//Dump into the 256 byte rom memory of gameboy
	for (int x = 0; x < 0x100; x++) {
		mmu.bios[x] = bios[x];
	}
}