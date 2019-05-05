#include "CPU.h"
#include "mmu.h"

MMU mmu;

void CPU::init() {
	pc = 0x100;
//	cout << hex << pc << endl;
}

void CPU::runLife() {
	uint16_t opcode = mmu.read8(pc);
	cout << "Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
	switch (opcode) {
		case 0x00:
			NOP(opcode);
			break;
		case 0xC3:
			JP_NN(opcode);
			break;
		default:
			cout << "Opcode: " << hex << static_cast<unsigned>(opcode) << " not implemented" << endl;
	}
}

void CPU::NOP(uint16_t opcode) {
	pc++;
	cout << "NOP" << endl;
}

void CPU::JP_NN(uint16_t opcode) {
	pc = mmu.read(pc + 1) + (mmu.read(pc + 2) << 8);
	cout << "JP " << hex << pc << "h" << endl;
}

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