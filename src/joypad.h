#include <iointerface.h>
#include <stdint.h>
#include "SDL2/SDL.h"

#ifndef JOYPAD_H
#define JOYPAD_H

namespace nesemu {
	class Joypad : public IOInterface {
	private:
		struct tJoyPad {
			bool a;
			bool b;
			bool select;
			bool start;
			bool up;
			bool down;
			bool left;
			bool right;
			bool strobe;
			unsigned tick;

			tJoyPad() : strobe(true), tick(0) {

			}

			inline void write(uint8_t value) {
				strobe = value & 1;
			}

			inline uint8_t read() {
				if (strobe) {
					tick = 0;
					return a;
				}

				switch (tick++) {
					case 0: return a;
					case 1: return b;
					case 2: return select;
					case 3: return start;
					case 4: return up;
					case 5: return down;
					case 6: return left;
					default:
						tick = 0;
				}

				return right;
			}
		};
	public:
		Joypad() {

		}

		inline virtual void write(const uint16_t addr, const uint8_t value) {
			if (addr == 0x4016)
				pad0.write(value);
			else
				pad1.write(value);
		}

		inline virtual const uint8_t read(const uint16_t addr) {
			if (addr == 0x4016)
				return pad0.read();
			return pad1.read();
		}

		inline virtual const bool isMyAddress(const uint16_t addr) const {
			return (addr == 0x4016) || (addr == 0x4017);
		}

		inline virtual void update() {
			SDL_Event event;

			while(SDL_PollEvent(&event)){
				// printKey(&event.key);
				bool state;
				switch(event.type){
					case SDL_KEYDOWN:
						state = true;
						break;
					case SDL_KEYUP:
						state = false;
						break;
					case SDL_QUIT:
						exit(0);
						break;
					default:
						continue;
				}

				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_A:
						pad0.left = state;
						break;
					case SDL_SCANCODE_S:
						pad0.down = state;
						break;
					case SDL_SCANCODE_D:
						pad0.right = state;
						break;
					case SDL_SCANCODE_W:
						pad0.up = state;
						break;
					case SDL_SCANCODE_SPACE:
						pad0.b = state;
						break;
					case SDL_SCANCODE_RETURN:
						pad0.a = state;
						break;
					case SDL_SCANCODE_P:
						pad0.start = state;
						break;
					case SDL_SCANCODE_BACKSPACE:
						pad0.select = state;
						break;
					case SDL_SCANCODE_Q:
						exit(0);
						break;
					default:
						break;
				}
			}
		}

		virtual ~Joypad() {

		}

	private:
		// https://wiki.libsdl.org/SDLScancodeLookup
		void printKey(SDL_KeyboardEvent *key){
			if( key->type == SDL_KEYUP )
				printf( "Release:- " );
			else
				printf( "Press:- " );

			printf("Scancode: 0x%02X", key->keysym.scancode );
			printf("Name: %s", SDL_GetKeyName( key->keysym.sym ) );
			printf("\n" );
		}

	private:
		tJoyPad pad0;
		tJoyPad pad1;
	};
}
#endif
