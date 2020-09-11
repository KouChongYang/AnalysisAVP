/*
 * @Author: gongluck
 * @Date: 2020-09-11 22:22:14
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-09-12 00:05:16
 */

#define SDL_MAIN_HANDLED

#define MY_QUIT SDL_USEREVENT+1

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
	while (i++ < 20)
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

	bool exit = false;
	SDL_Event event = { 0 };
	while (!exit)
	{
		ret = SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)
			{
				std::cout << char('a' + event.key.keysym.sym - SDLK_a) << " down" << std::endl;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_Event event_q;
				event_q.type = MY_QUIT;
				ret = SDL_PushEvent(&event_q);
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				std::cout << "mouse left down" << std::endl;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				std::cout << "mouse right down" << std::endl;
			}
			else
			{
				std::cout << "mouse down" << std::endl;
			}
			break;
		case SDL_MOUSEMOTION:
			std::cout << "mouse move " << event.button.x << ", " << event.button.y << std::endl;
			break;
		case MY_QUIT:
			std::cout << "my quit envent." << std::endl;
			exit = true;
			break;
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
