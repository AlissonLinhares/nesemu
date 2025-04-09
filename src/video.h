#include <iointerface.h>
#include "SDL2/SDL.h"

#ifndef VIDEO_H
#define VIDEO_H

namespace nesemu {
	class Video : public IOInterface {
	public:
		Video(int width, int height) : width(width), height(height) {
			SDL_Init(SDL_INIT_VIDEO);

			// Full screen mode enabled
			window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
			renderer = SDL_CreateRenderer(window, -1, 0);
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
			SDL_RenderSetLogicalSize(renderer, width, height);

			// Full screen mode disabled
			// window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
			// renderer = SDL_CreateRenderer(window, -1, 0);
			texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, width, height);

			pixels = new Uint32[width * height];
			memset(pixels, 0, width * height * sizeof(Uint32));
		}

		inline virtual void write(const uint16_t addr, const uint8_t value)  {
			int index = addr - 0x200;
			int y = index / 32;
			int x = index % 32;
			draw(x, y, value);
			buffer[index] = value;
		}

		inline virtual const uint8_t read(const uint16_t addr) {
			return buffer[addr - 0x200];
		}

		inline virtual const bool isMyAddress(const uint16_t addr) const {
			return (addr >= 0x200) && (addr < 0x600);
		}

		inline virtual void update() {
			SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		inline void draw(int x, int y, uint8_t color) {
			pixels[y * width + x] = colorPalette[color & 0x3F];
		}

		virtual ~Video() {
			delete[] pixels;
			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);

			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	private:
		int width;
		int height;
		SDL_Texture *texture;
		SDL_Renderer *renderer;
		SDL_Window *window;
		uint8_t buffer[1024];
		Uint32 *pixels;
		Uint32 colorPalette[64] = {
			0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400,
			0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
			0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
			0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
			0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
			0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
			0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
			0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000
		};
	};
}

#endif
