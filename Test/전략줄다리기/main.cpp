#include "SDL/SDL.h"
#include "PullMind_include.hpp"

#pragma comment (lib , "lib/SDL2.lib")
#undef main 

int main(void) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("hi", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Entity *entity = new Entity(renderer, "./resources/image/porg.jpg", Rect(100,100, 100,100),Rect(100,100,100,100), 1, 1, 1);

	entity->RenderEntity();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawPoint(renderer, 3, 3);
	SDL_RenderPresent(renderer);
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_WaitEventTimeout(&event, 100);
		entity->Callback(event);
	}

	

	
}