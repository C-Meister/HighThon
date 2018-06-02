#include "SDL/SDL.h"
#include <process.h>
#include "socket.h"
#include "PullMind_include.hpp"


#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib , "lib/SDL2.lib")
#undef main 

VEC_ENTI vec_enti;
MAP_ENTI map_enti;

queue<int> idQ;

int main(void) {


	SDL_Init(SDL_INIT_EVERYTHING);
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)connectServer, NULL, 0, NULL);

	SDL_Window *window = SDL_CreateWindow("hi", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Entity *entity = new Entity(renderer, "./resources/image/porg.jpg", Rect(100,100, 100,100),Rect(100,100,100,100), 1, 1, 1);
	Entity *entity2 = new Entity(renderer, "./resources/image/porg.jpg", Rect(300, 100, 100, 100), Rect(300, 100, 100, 100), 2, 1, 1);
	Entity *entity3 = new Entity(renderer, "./resources/image/porg.jpg", Rect(300, 300, 100, 100), Rect(300, 300, 100, 100), 3, 1, 1);


	entity->RenderEntity();
	entity2->RenderEntity();
	entity3->RenderEntity();
	SDL_RenderPresent(renderer);
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_WaitEventTimeout(&event, 0);
		SDL_RenderClear(renderer);
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			Entity* entity = (*it);
			if(entity->flag==false) {
				//애니메이션 중이 아님
				entity->RenderEntity();
			}
		}
		int size = idQ.size();
		for (int i = 0; i < size; i++) {
			int id = idQ.front(); idQ.pop();
			Entity * entity = map_enti.find(id)->second;
			if (entity->v.empty()) {
				entity->flag = false;
			}
			else {
				SDL_Point p = entity->v.back(); entity->v.pop_back();
				entity->Animation(p);
			}
			entity->RenderEntity();
			idQ.push(id);
		}

	
		
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			(*it)->Callback(event);
		}
		SDL_RenderPresent(renderer);
	}
	

	
}