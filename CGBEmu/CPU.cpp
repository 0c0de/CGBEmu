#include "CPU.h"
#include "mmu.h"

MMU mmu;
GameboyRegisters reg;
GameboyFlags flags;

void CPU::init() {
	pc = 0x100;
	mmu.sp = 0x2000;
}

void CPU::runLife() {
	uint16_t opcode = mmu.read8(pc);
	switch (opcode) {
		case 0x00:
			NOP(opcode);
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
		case 0xFA:
			LD_A_NN(opcode);
			break;
		case 0xFE:
			CP_N(opcode);
			break;
		case 0x20: case 0x28: case 0x30: case 0x38:
			JR_CC_N(opcode);
			break;
		default:
			cout << "Opcode: " << hex << static_cast<unsigned>(opcode) << " not implemented" << endl;
	}
}

//Nop instruction
void CPU::NOP(uint16_t opcode) {
	pc++;
	cout << "NOP" << endl;
	addCycles(4);
}

//Jump to a position
void CPU::JP_NN(uint16_t opcode) {
	pc = mmu.read(pc + 1) + (mmu.read(pc + 2) << 8);
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
	uint16_t nn = mmu.read(pc + 1) + (mmu.read(pc + 2) << 8);
	uint16_t nextInstruction = pc + 1;
	mmu.PUSH(nextInstruction);
	pc = nn;
	addCycles(12);
	cout << "CALL, " << nn << "h" << endl;
}

void CPU::LD_A_NN(uint16_t opcode) {
	uint16_t nn = mmu.read(pc + 1) + (mmu.read(pc + 2) << 8);
	reg.A = nn;
	pc += 3;
	addCycles(16);
	cout << "LD A, " << nn << "h" << endl;
}

void CPU::CP_N(uint16_t opcode) {
	uint16_t n = mmu.read(pc + 1);
	reg.A - n == 1 ? flags.Z = true : flags.Z = false;
	flags.N = true;
	reg.A < n ? flags.C = true : flags.C = false;
	flags.H = false;
	(((reg.A & 0xF) - (n & 0xF)) & 0x10) == 0x10 ? flags.H = true : flags.H = false;
	pc += 2;
	addCycles(8);
	cout << "CP, " << hex << n << "h" << endl;
}

void CPU::JR_CC_N(uint16_t opcode) {
	switch (opcode) {
		//JR NZ, N
	case 0x20: {
			if (!flags.Z) {
				int8_t n = (int8_t)mmu.read(pc - 1) << 4;
				pc = n;
				addCycles(4);
			}
			pc += 2;
			addCycles(8);
		}
		break;
		case 0x28:
			break;
		case 0x30:
			break;
		case 0x38:
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
		mmu.memory[x] = tempGame[x];
	}
	game.close();
}

void CPU::loadBIOS() {
	const char* path = "games/bios/bios.gb";

	//Holds the game
	ifstream bios(path, ifstream::binary);

	//Set pos at the end
	bios.seekg(0, ifstream::end);

	//Holds the size of the game
	int biossize = bios.tellg();
	cout << "Size is: " << biossize << endl;

	//Set pos at the beginning
	bios.seekg(0, ifstream::beg);

	//Create a char for store temporally the bios
	char* tempBios = new char[biossize];

	//Dump content into the buffer we created
	bios.read(tempBios, biossize);
	
	//Dump into the 256 byte rom memory of gameboy
	for (int x = 0; x < biossize; x++) {
		mmu.memory[x] = tempBios[x];
		cout << mmu.memory[x] << endl;
	}
	bios.close();
}

void CPU::addCycles(int ciclesToAdd) {
	cicles += ciclesToAdd;
}

void CPU::clearCycles() {
	cicles = 0;
}