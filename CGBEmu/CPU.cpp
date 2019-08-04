#include "CPU.h"
#include "mmu.h"

MMU mmu;
GameboyRegisters reg;
GameboyFlags flags;

void CPU::init() {
	pc = 0x100;
}

void CPU::addCycles(int ciclesToAdd) {
	cicles += ciclesToAdd;
}

void CPU::clearCycles() {
	cicles = 0;
}

void CPU::runLife() {
	uint8_t opcode = mmu.read8(pc);
	switch (opcode) {
		case 0x00:
			NOP();
			break;
		case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E:
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
		case 0xFA:
			LD_A_regC(opcode);
			break;
		case 0xFE: case 0xBF: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE:
			CP_N(opcode);
			break;
		case 0x20: case 0x28: case 0x30: case 0x38:
			JR_CC_N(opcode);
			break;
		case 0x01: case 0x11: case 0x21: case 0x31:
			LD_N_NN(opcode);
			break;
		case 0xf0:
			//LDH_A_N(opcode);
			break;
		default:
			cout << "Opcode: " << hex << static_cast<unsigned>(opcode) << " not implemented" << endl;
	}
}

void CPU::LD_NN_N(uint16_t opcode) {
	uint8_t n = mmu.read8(pc+1);

	switch (opcode)
	{

	case 0x06:
		reg.B = n;
		addCycles(8);
		pc += 2;
		break;

	case 0x0E:
		reg.C = n;
		addCycles(8);
		pc += 2;
		break;

	case 0x16:
		reg.D = n;
		addCycles(8);
		pc += 2;
		break;

	case 0x1E:
		reg.E = n;
		addCycles(8);
		pc += 2;
		break;

	case 0x26:
		reg.H = n;
		addCycles(8);
		pc += 2;
		break;

	case 0x2E:
		reg.L = n;
		addCycles(8);
		pc+=2;
		break;

	default:
		pc += 2;
		break;
	}
}

void CPU::LD_r1_r2(uint16_t opcode) {
	switch (opcode)
	{
	case 0x7F:
		reg.A = reg.A;
		addCycles(4);
		pc += 2;
		break;
	case 0x78:
		reg.A = reg.B;
		addCycles(4);
		pc += 2;
		break;
	case 0x79:
		reg.A = reg.C;
		addCycles(4);
		pc += 2;
		break;
	case 0x7a:
		reg.A = reg.D;
		addCycles(4);
		pc += 2;
		break;
	case 0x7b:
		reg.A = reg.E;
		addCycles(4);
		pc += 2;
		break;
	case 0x7c:
		reg.A = reg.H;
		addCycles(4);
		pc += 2;
		break;
	case 0x7d:
		reg.A = reg.L;
		addCycles(4);
		pc += 2;
		break;
	case 0x7e:
		reg.A = mmu.read8(reg.HL);
		addCycles(8);
		pc += 2;
		break;
	case 0x40:
		reg.B = reg.B;
		addCycles(4);
		pc += 2;
		break;
	case 0x41:
		reg.B = reg.C;
		addCycles(4);
		pc += 2;
		break;
	case 0x42:
		reg.B = reg.D;
		addCycles(4);
		pc += 2;
		break;
	case 0x43:
		reg.B = reg.E;
		addCycles(4);
		pc += 2;
		break;
	case 0x44:
		reg.B = reg.H;
		addCycles(4);
		pc += 2;
		break;
	case 0x45:
		reg.B = reg.L;
		addCycles(4);
		pc += 2;
		break;
	case 0x46:
		reg.B = mmu.read8(reg.HL);
		addCycles(8);
		pc += 2;
		break;
	case 0x48:
		reg.C = reg.B;
		addCycles(4);
		pc += 2;
		break;
	case 0x49:
		reg.C = reg.C;
		addCycles(4);
		pc += 2;
		break;
	case 0x4A:
		reg.C = reg.D;
		addCycles(4);
		pc += 2;
		break;
	case 0x4B:
		reg.C = reg.E;
		addCycles(4);
		pc += 2;
		break;
	case 0x4C:
		reg.C = reg.H;
		addCycles(4);
		pc += 2;
		break;
	case 0x4D:
		reg.C = reg.L;
		addCycles(4);
		pc += 2;
		break;
	case 0x4E:
		reg.C = mmu.read8(reg.HL);
		addCycles(8);
		pc += 2;
		break;
	case 0x50:
		reg.D = reg.B;
		addCycles(4);
		pc += 2;
		break;
	case 0x51:
		reg.D = reg.C;
		addCycles(4);
		pc += 2;
		break;
	case 0x52:
		reg.D = reg.D;
		addCycles(4);
		pc += 2;
		break;
	case 0x53:
		reg.D = reg.E;
		addCycles(4);
		pc += 2;
		break;
	case 0x54:
		reg.D = reg.H;
		addCycles(4);
		pc += 2;
		break;
	case 0x55:
		reg.D = reg.L;
		addCycles(4);
		pc += 2;
		break;
	case 0x56:
		reg.D = mmu.read8(reg.HL);
		addCycles(8);
		pc += 2;
		break;
	case 0x58:
		reg.E = reg.B;
		addCycles(4);
		pc += 2;
		break;
	case 0x59:
		reg.E = reg.C;
		addCycles(4);
		pc += 2;
		break;
	case 0x5A:
		reg.E = reg.D;
		addCycles(4);
		pc += 2;
		break;
	case 0x5B:
		reg.E = reg.E;
		addCycles(4);
		pc += 2;
		break;
	case 0x5C:
		reg.E = reg.H;
		addCycles(4);
		pc += 2;
		break;
	case 0x5D:
		reg.E = reg.L;
		addCycles(4);
		pc += 2;
		break;
	case 0x5E:
		reg.E = mmu.read8(reg.HL);
		addCycles(8);
		pc += 2;
		break;
	case 0x60:
		reg.H = reg.B;
		addCycles(4);
		pc += 2;
		break;
	case 0x61:
		reg.H = reg.C;
		addCycles(4);
		pc += 2;
		break;
	case 0x62:
		reg.H = reg.D;
		addCycles(4);
		pc += 2;
		break;
	case 0x63:
		reg.H = reg.E;
		addCycles(4);
		pc += 2;
		break;
	case 0x64:
		reg.H = reg.H;
		addCycles(4);
		pc += 2;
		break;
	case 0x65:
		reg.H = reg.L;
		addCycles(4);
		pc += 2;
		break;
	case 0x66:
		reg.H = mmu.read8(reg.HL);
		addCycles(8);
		pc += 2;
		break;
	case 0x68:
		reg.L = reg.B;
		addCycles(4);
		pc += 2;
		break;
	case 0x69:
		reg.L = reg.C;
		addCycles(4);
		pc += 2;
		break;
	case 0x6A:
		reg.L = reg.D;
		addCycles(4);
		pc += 2;
		break;
	case 0x6B:
		reg.L = reg.E;
		addCycles(4);
		pc += 2;
		break;
	case 0x6C:
		reg.L = reg.H;
		addCycles(4);
		pc += 2;
		break;
	case 0x6D:
		reg.L = reg.L;
		addCycles(4);
		pc += 2;
		break;
	case 0x6E:
		reg.L = mmu.read8(reg.HL);
		addCycles(8);
		pc += 2;
		break;
	case 0x70:
		mmu.write8(mmu.read8(reg.HL), reg.B);
		addCycles(8);
		pc += 2;
		break;
	case 0x71:
		mmu.write8(mmu.read8(reg.HL), reg.C);
		addCycles(8);
		pc += 2;
		break;
	case 0x72:
		mmu.write8(mmu.read8(reg.HL), reg.D);
		addCycles(8);
		pc += 2;
		break;
	case 0x73:
		mmu.write8(mmu.read8(reg.HL), reg.E);
		addCycles(8);
		pc += 2;
		break;
	case 0x74:
		mmu.write8(mmu.read8(reg.HL), reg.H);
		addCycles(8);
		pc += 2;
		break;
	case 0x75:
		mmu.write8(mmu.read8(reg.HL), reg.L);
		addCycles(8);
		pc += 2;
		break;
	case 0x36:
		//mmu.write8(mmu.read8(reg.HL), reg.B);
		addCycles(12);
		pc += 2;
		break;
	default:
		pc += 2;
		break;
	}
}

void CPU::LD_A_N(uint16_t opcode) {
	switch (opcode)
	{
	case 0x7F:
		reg.A = reg.A;
		pc+=2;
		addCycles(4);
		break;
	case 0x78:
		reg.A = reg.B;
		pc+=2;
		break;
	case 0x79:
		reg.A = reg.C;
		pc+=2;
		break;
	case 0x7A:
		reg.A = reg.D;
		pc+=2;
		addCycles(4);
		break;
	case 0x7B:
		reg.A = reg.E;
		pc+=2;
		addCycles(4);
		break;
	case 0x7C:
		reg.A = reg.H;
		pc+=2;
		addCycles(4);
		break;
	case 0x7D:
		reg.A = reg.L;
		pc+=2;
		addCycles(4);
		break;
	case 0x0A:
		reg.A = mmu.read8(reg.BC);
		pc+=2;
		addCycles(8);
		break;
	case 0x1A:
		reg.A = mmu.read8(reg.DE);
		pc+=2;
		addCycles(8);
		break;
	case 0x7E:
		reg.A = mmu.read8(reg.HL);
		pc+=2;
		addCycles(8);
		break;
	case 0xFA:
		reg.A = mmu.read(reg.DE);
		pc+=3;
		addCycles(16);
		break;
	case 0x3E:
		reg.A = mmu.read8(reg.DE);
		pc+=2;
		addCycles(8);
		break;
	default:
		break;
	}
}

//Nop instruction
void CPU::NOP() {
	pc++;
	cout << "NOP" << endl;
	addCycles(4);
}

//Jump to a position
void CPU::JP_NN(uint16_t opcode) {
	pc = mmu.read(pc+1);
	cout << "JP, " << hex << pc << "h" << endl;
	addCycles(12);
}

void CPU::DI() {
	IME = false;
	pc++;
	cout << "DI" << endl;
	addCycles(4);
}

void CPU::EI() {
	IME = true;
	pc++;
	cout << "EI" << endl;
	addCycles(4);
}

void CPU::CALL_NN(uint16_t opcode) {
	uint16_t nn = mmu.read(pc+1);
	mmu.push(mmu.sp, nn);
	pc = nn;
	addCycles(12);
	cout << "CALL, " << nn << "h" << endl;
}

void CPU::LD_A_regC(uint16_t opcode) {
	uint16_t nn = mmu.read(pc+1);
	reg.A = mmu.read8(nn);
	pc += 3;
	
	cout << "LD A, (" << nn << ")h" << endl;
}

void CPU::CP_N(uint16_t opcode) {
	uint8_t n;
	uint16_t nn;
	
	switch (opcode) {
	case 0xFE:
		n = mmu.read8(pc+1);
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(8);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBF:
		n = reg.A;
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xB8:
		n = reg.B;
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xB9:
		n = reg.C;
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBA:
		n = reg.D;
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBB:
		n = reg.E;
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBC:
		n = reg.H;
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBD:
		n = reg.L;
		reg.A - n == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < n ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBE:
		nn = mmu.read(reg.HL);
		reg.A - nn == 0 ? flags.Z = true : flags.Z = false;
		flags.N = true;
		reg.A < nn ? flags.C = true : flags.C = false;
		flags.H = false;
		(((reg.A & 0xF) - (nn & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
		pc += 2;
		addCycles(8);
		cout << "CP, A " << hex << static_cast<unsigned>(nn) << "h" << endl;
		break;
	default:
		pc+=2;
	}

}

void CPU::JR_CC_N(uint16_t opcode) {
	uint8_t n = mmu.read8(pc+1);
	switch (opcode) {
		//JR NZ, N
	case 0x20: {
			if (!flags.Z) {
				pc = pc - (0xff - n) - 1;
			}
			pc += 2;
			cout << "JR NZ, " << hex << pc << endl;
			addCycles(8);
		}
		break;
		//JR Z, N
	case 0x28: {
			if (flags.Z) {
				pc = pc - (0xff - n) - 1;
			}
			pc += 2;
			addCycles(8);
			cout << "JR Z, " << hex << pc << endl;
		}
		break;
		//JR NC, N
	case 0x30: {
			if (!flags.C) {
				pc = pc - (0xff - n) - 1;
			}
			pc += 2;
			addCycles(8);
			cout << "JR NC, " << hex << pc << endl;
		}
		break;
		//JR C, N
	case 0x38: {
			if (flags.C) {
				pc = pc - (0xff - n) - 1;
			}
			pc += 2;
			addCycles(8);
			cout << "JR C, " << hex << pc << endl;
		}
		break;

	}
}

void CPU::LD_N_NN(uint16_t opcode) {
	uint16_t nn = mmu.read(pc+1);
	switch (opcode)
	{
	case 0x01:
		reg.BC = nn;
		cout << "LD BC, " << hex << nn << endl;
		pc += 3;
		addCycles(12);
		break;
	case 0x11:
		reg.DE = nn;
		cout << "LD DE, " << hex << nn << endl;
		pc += 3;
		addCycles(12);
		break;
	case 0x21:
		reg.HL = nn;
		cout << "LD HL, " << hex << nn << endl;
		pc += 3;
		addCycles(12);
		break;
	case 0x31:
		mmu.sp = nn;
		cout << "LD SP, " << hex << nn << endl;
		pc += 3;
		addCycles(12);
		break;
	}
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