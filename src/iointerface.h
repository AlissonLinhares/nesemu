#ifndef IOINTERFACE_H
#define IOINTERFACE_H

#include <iostream>
#include <stdint.h>
#include <unistd.h>

namespace nesemu {
	class IOInterface {
	public:
		virtual void write(const uint16_t addr, const uint8_t value) = 0;
		virtual const uint8_t read(const uint16_t addr) = 0;
		virtual const bool isMyAddress(const uint16_t addr) const = 0;
		virtual void update() = 0;
	private:
	};
}

#endif
