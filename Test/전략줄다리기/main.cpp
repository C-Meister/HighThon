#include "SDL/SDL.h"
#include "socket.h"
#include "PullMind_include.hpp"


#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib , "lib/SDL2.lib")
#undef main 

queue<int> idQ;

int main(void) {

	
	map<int, Entity *> entities;

	SDL_Init(SDL_INIT_EVERYTHING);
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)connectServer, NULL, 0, NULL);

	SDL_Window *window = SDL_CreateWindow("hi", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Entity *entity = new Entity(renderer, "./resources/image/porg.jpg", Rect(100,100, 100,100),Rect(100,100,100,100), 1, 1, 1);

	entities.insert(make_pair(entity->id, entity));
	entity->RenderEntity();
	SDL_RenderPresent(renderer);
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		int size = idQ.size();
		for (int i = 0; i < size; i++) {
			int id = idQ.front(); idQ.pop();
			Entity* entity = entities.find(id)->second;
			
			SDL_Point p;
			if (entity->v.empty()){
				entity->flag = false;
			}
			else {
				p = entity->v.back(); entity->v.pop_back();
				entity->Animation(p);
				idQ.push(id);
			}
		}

		SDL_WaitEventTimeout(&event, 1);
		entity->Callback(event);
	}
	

	
}