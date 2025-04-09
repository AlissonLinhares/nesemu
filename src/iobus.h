#ifndef IOBUS_H
#define IOBUS_H

#include <list>
#include <iointerface.h>
#include <memory.h>
#include <iostream>
#include <stdint.h>
#include <unistd.h>

using namespace std;

namespace nesemu {
	class IOBus {
	public:
		IOBus(Memory& memory) : memory(memory) {
			nmiSignal = false;
		}

		void connect(IOInterface &device) {
			devices.push_back(&device);
		}

		void update() {
			for (auto it=devices.begin(); it != devices.end(); ++it)
				(*it)->update();
		}

		inline virtual const uint8_t load(const uint16_t addr) {
			return getDevice(addr)->read(addr);
		}

		inline virtual void store(const uint16_t addr, const uint8_t value) {
			return getDevice(addr)->write(addr, value);
		}

		inline const uint16_t load16(const uint16_t offset) {
			return (((uint16_t) load(offset + 1)) << 8) | load(offset);
		}

		inline void store16(const uint16_t offset, const uint16_t value) {
			store(offset + 1, value >> 8);
			store(offset, value & 0xFF);
		}

		inline IOInterface* getDevice(const uint16_t addr) {
			for (auto it=devices.begin(); it != devices.end(); ++it) {
				if ((*it)->isMyAddress(addr))
					return *it;
			}

			return &memory;
		}

		inline void setNMI(const bool nmi) {
			nmiSignal = nmi;
		}

		inline const bool getNMI() const {
			return nmiSignal;
		}

	private:
		list<IOInterface*> devices;
		Memory &memory;
		bool nmiSignal;
	};
}

#endif
