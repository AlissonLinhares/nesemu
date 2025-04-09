#include <iobus.h>
#include <iointerface.h>
#include "SDL2/SDL.h"

#ifndef PPU_H
#define PPU_H

#define PPU_CTRL    0
#define PPU_MASK    1
#define PPU_STATUS  2
#define PPU_OAMADDR 3
#define PPU_OAMDATA 4
#define PPU_SCROLL  5
#define PPU_ADDR    6
#define PPU_DATA    7
#define PPU_OAMDMA  8

namespace nesemu {

	class PPU : public IOInterface {
	private:
		struct tSprite {
			uint8_t y;
			uint8_t tileNumber;
			uint8_t attributes;
			uint8_t x;
		} __attribute__((packed));

	public:
		PPU(IOBus &io, uint16_t patterTableAddr) : io(io), patterTableAddr(patterTableAddr), width(256), height(240) {
			SDL_Init(SDL_INIT_VIDEO);

			// Full screen mode enabled
			// window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
			// renderer = SDL_CreateRenderer(window, -1, 0);
			// SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
			// SDL_RenderSetLogicalSize(renderer, width, height);

			// Full screen mode disabled
			window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
			renderer = SDL_CreateRenderer(window, -1, 0);
			texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, width, height);

			pixels = new Uint32[width * height];
			memset(pixels, 0, width * height * sizeof(Uint32));

			paletteAddr = 0;
			idxPallet = 0;
			countNMI = 0;
		}

		inline virtual void write(const uint16_t addr, const uint8_t value)  {
			if (addr == 0x4014) {
				spriteAddr = ((uint16_t) value) << 8 | spriteAddr;
				return;
			}

			switch ((addr - 0x2000) % 8) {
				case PPU_CTRL:
					ctrl = value;
					break;
				case PPU_ADDR:
					paletteAddr = (paletteAddr << 8) | value;
					break;
				case PPU_DATA:
					if (idxPallet < 16)
						bgPallet[idxPallet++] = systemPallet[value & 0x3F];
					else
						spPallet[idxPallet++] = systemPallet[value & 0x3F];
					break;
				case PPU_OAMADDR:
					spriteAddr = value;
					break;
				case PPU_MASK:
					mask = value;
					break;
				default:
					break;
			}
		}

		inline virtual const uint8_t read(const uint16_t addr) {
			switch ((addr - 0x2000) % 8) {
				case PPU_STATUS:
					uint8_t result = status;
					status = status & (~0x80);
					return result;
			}
		}

		inline virtual const bool isMyAddress(const uint16_t addr) const {
			return ((addr >= 0x2000) && (addr <= 0x3FFF)) || (addr == 0x4014);
		}

		inline virtual void update() {
			countNMI++;

			if (countNMI == 100) {
				io.setNMI(false);

				if (mask & 0x16) {
					for (int i = 0; i < 256; i += 4) {
						tSprite sprite;
						sprite.y = io.load(spriteAddr + i);
						sprite.tileNumber = io.load(spriteAddr + i + 1);
						sprite.attributes = io.load(spriteAddr + i + 2);
						sprite.x = io.load(spriteAddr + i + 3);
						draw(sprite);
					}
				}

				SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
				status = status & (~0x80);
				countNMI = 0;
			} else if (countNMI == 0) {
				status |= 0x80;
			} else {
				if ((ctrl & 0x80) && (status & 0x80))
					io.setNMI(true);
			}
		}

		inline void draw(int x, int y, Uint32 color) {
			pixels[y * width + x] = color;
		}

		inline void draw(const tSprite &sprite) {
			for (int i = 0; i < 8; i++) {
				uint8_t left = io.load(patterTableAddr + 8 * sprite.tileNumber + i);
				uint8_t right = io.load(patterTableAddr + 8 * sprite.tileNumber + i + 0x1000);

				for (int j = 0; j < 8; j++) {
					unsigned index = ((left >> (9 - j)) & 2) | ((right >> (8 - j)) & 1);
					if (index != 0)
						draw((sprite.x + j), (sprite.y + i), spPallet[index | ((sprite.attributes & 3) << 2)]);
				}
			}
		}

		virtual ~PPU() {
			delete[] pixels;
			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);

			SDL_DestroyWindow(window);
			SDL_Quit();
		}

	private:
		IOBus &io;
		uint16_t patterTableAddr;
		int width;
		int height;
		SDL_Texture *texture;
		SDL_Renderer *renderer;
		SDL_Window *window;
		Uint32 *pixels;
		const Uint32 systemPallet[64] = {
			0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400,
			0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
			0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
			0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
			0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
			0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
			0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
			0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000
		};

		Uint32 bgPallet[16];
		Uint32 spPallet[16];
		unsigned idxPallet;

		uint8_t registers[9];
		uint8_t ctrl;
		uint8_t status;
		uint16_t paletteAddr;
		uint8_t mask;
		uint16_t spriteAddr;
		int countNMI;
	};
}

#endif
