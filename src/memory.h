#include <config.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <iointerface.h>

#ifndef MEMORY_H
#define MEMORY_H

using namespace std;

namespace nesemu {

	class Memory : public IOInterface {
	public:
		Memory(size_t size) : size(size) {
			memory = new uint8_t[size];
			clear();
		}

		Memory() : Memory(DEFAULT_MEMORY_SIZE) {

		}

		inline void dump() const {
			cout << "------ Memory  ------" << endl;
			cout << setfill('0'); // fill with 0s
			for (int i = 0; i < size; i++)
				cout << "0x" << setw(4) << hex << i << " = " << hex << setw(2)
				     << (unsigned) memory[i] << endl;
			cout << "---------------------" << endl;
		}

		inline virtual void update() {

		}

		inline virtual const uint8_t read(const uint16_t addr) {
			return memory[addr];
		}

		inline virtual void write(const uint16_t addr, const uint8_t value) {
			memory[addr] = value;
		}

		inline virtual const bool isMyAddress(const uint16_t addr) const {
			return true;
		}

		inline void setCHRAddr(const uint16_t chrAddr) {
			this->chrAddr = chrAddr;
		}

		inline uint16_t getCHRAddr() const {
			return this->chrAddr;
		}

		void clear() {
			for (int i = 0; i < size; i++)
				memory[i] = 0;
		}

		~Memory() {
			delete[] memory;
		}
	private:
		uint8_t *memory;
		uint16_t chrAddr;
		size_t size;
	};
}

#endif
