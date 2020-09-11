/*
 * @Author: gongluck
 * @Date: 2020-09-11 22:22:14
 * @Last Modified by:   gongluck
 * @Last Modified time: 2020-09-11 22:22:14
 */

#define SDL_MAIN_HANDLED

const int WIDTH = 300;
const int HEIGHT = 300;
const int width = 10;
const int height = 10;

#include <iostream>
#include "SDL.h"

int main()
{
	auto ret = SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	auto renderer = SDL_CreateRenderer(window, -1, 0);
	auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

	int i = 0;
	while (i++ < 120)
	{
		ret = SDL_SetRenderTarget(renderer, texture);
		ret = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		ret = SDL_RenderClear(renderer);
		SDL_Rect rect = { rand() % (WIDTH - width), rand() % (HEIGHT - height), width, height };
		ret = SDL_RenderDrawRect(renderer, &rect);
		ret = SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		ret = SDL_RenderFillRect(renderer, &rect);

		ret = SDL_SetRenderTarget(renderer, nullptr);
		ret = SDL_RenderCopy(renderer, texture, nullptr, nullptr);

		SDL_RenderPresent(renderer);
		SDL_Delay(500);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
