#include "CPU.h"

void CPU::init() {
	pc = 0x100;
//	cout << hex << pc << endl;
}

void CPU::runLife() {
	uint16_t opcode = gameDump[pc];
	//cout << hex << pc << endl;
	cout << hex << static_cast<unsigned>(opcode) << endl;
	pc += 2;
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

	//Dump content into the buffer we created
	game.read(gameDump, gamesize);
	game.close();
}

void CPU::loadBIOS() {
	const char* path = "bios/bios.gb";

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
		memory[x] = tempBios[x];
	}
	bios.close();
}