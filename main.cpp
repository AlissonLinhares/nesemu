#include <iostream>
#include <stdint.h>
#include <fstream>

#include <cpu.h>
#include <ppu.h>
#include <iobus.h>
#include <iointerface.h>
#include <config.h>
#include <joypad.h>
#include <memory.h>
#include <video.h>

using namespace nesemu;

bool loadBinary(Memory &memory, const char *path) {
	ifstream file(path, ios::in|ios::binary|ios::ate);

	if (file.is_open()) {
		streampos size = file.tellg();
		char *data = new char[size];

		file.seekg(0, ios::beg);
		file.read(data, size);
		file.close();

		// .db "NES", $1a ;identification of the iNES header
		// .db PRG_COUNT ;number of 16KB PRG-ROM pages
		// .db $01 ;number of 8KB CHR-ROM pages
		// .db $00|MIRRORING ;mapper 0 and mirroring
		// .dsb 9, $00 ;clear the remaining bytes

		if (data[0] == 'N' && data[1] == 'E' && data[2] == 'S')
			memory.setCHRAddr(1 << 14);

		for (int i = 0; i < size; i++)
			memory.write(i, (uint8_t) data[i]);

		return true;
	}

	return false;
}

int main(int argc, const char *argv[]) {
	if (argc != 2) {
		cout << "Usage: ./nesemu 5602binary" << endl;
		return 1;
	}

	Memory memory;
	if (!loadBinary(memory, argv[1])) {
		cout << "Unable to open file at: " << argv[1] << endl;
		return 1;
	}

	Joypad joypads;
	Video video(32,32);

	IOBus bus(memory);
	// PPU ppu(bus, memory.getCHRAddr());

	bus.connect(video);
	bus.connect(joypads);

	CPU cpu(bus);
	cpu.run();

	memory.dump();
	return 0;
}
