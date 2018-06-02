#include "SDL/SDL.h"
#include "socket.h"
#include "PullMind_include.hpp"

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib , "lib/SDL2.lib")
#undef main 
#define SOOHAN
VEC_ENTI vec_enti;
MAP_ENTI map_enti;
vector<int> R_status(5);

queue<int> idQ;
void TTF_DrawText(SDL_Renderer* renderer, string text, SDL_Point point, TTF_Font *font, SDL_Color color = { 0,0,0,0 }) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
	SDL_Rect dst;
	dst.x = point.x;
	dst.y = point.y;
	dst.w = src.w;
	dst.h = src.h;
	SDL_RenderCopy(renderer, texture, &src, &dst);
}
string user_name;
string enemy_name;
void ReceiveHandler(void) {
	char msg[255] = "";
	SDL_Event event = { 0 };
	char buff[100] = "";
	int buffint = 0;
	event.type = SDL_USEREVENT;
	event.user.code = SOCKET_EVENT;
	while (recv(server, msg, sizeof(msg), 0) > 0) {

		printf("%s\n", msg);
		if (strstr(msg, "match ") != NULL) {
			Sleep(1000);
			event.user.type = MATCHING;
			sscanf(msg, "match %[^n]s", buff);
			event.user.data1 = buff;

		}
		else if (strstr(msg, "room ") != NULL) {
			msg[0] = 'j';
			msg[1] = 'o';
			msg[2] = 'i';
			msg[3] = 'n';
			send(server, msg, strlen(msg), 0);
		}
		else if (strstr(msg, "exitroom") != NULL) {
			event.user.type = EXITING;

		}
		SDL_PushEvent(&event);
		memset(&event, 0, sizeof(event));
		memset(msg, 0, sizeof(msg));
		memset(buff, 0, sizeof(buff));
	}
}

int main(void) {



	SDL_Init(SDL_INIT_EVERYTHING);
//	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)connectServer, NULL, 0, NULL);
	
	SDL_Window *window = SDL_CreateWindow("Pull Mind", 0, 0, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Entity *Line[5], *entity[15], *enemies[15];
	
	Line[0] = new Entity(renderer, "./resources/entity/Line.png", Rect(460, 200, 900, 20), Rect(460, 200, 900, 100), 1, 0, false, ENTITY_ROPE);
	Line[1] = new Entity(renderer, "./resources/entity/Line.png", Rect(560, 386, 700, 20), Rect(560, 386, 700, 100), 2, 0, false, ENTITY_ROPE);
	Line[2]= new Entity(renderer, "./resources/entity/Line.png", Rect(460, 540, 900, 20), Rect(460, 540, 900, 100), 3, 0, false, ENTITY_ROPE);
	Line[3] = new Entity(renderer, "./resources/entity/Line.png", Rect(560, 694, 700, 20), Rect(560, 694, 700, 100), 4, 0, false, ENTITY_ROPE);
	Line[4] = new Entity(renderer, "./resources/entity/Line.png", Rect(460, 880, 900, 20), Rect(460, 880, 900, 100), 5, 0, false, ENTITY_ROPE);


	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8-i; j++) {
			entity[i * 8 + j] = new Entity(renderer, "./resources/entity/mob_11.png", Rect(25 + (100 * i), 75 + (i * 65) + (120 * j), 50, 50), Rect(25 + (100 * i), 75 + (i * 65) + (120 * j), 50, 50), i*8+j+6, 1, ALLY, ENTITY_PLAYER);
		}
	}
	for (int i = 1; i >= 0; i--) {
		for (int j = 6 + (1 - i); j >= 0; j--) {
			enemies[i * 8 + j] = new Entity(renderer, "./resources/entity/mor_11.png", Rect(1920 - (75 + (100 * i)), 1080 - (145 + (i * 65) + (120 * j)), 50, 50), Rect(1920 - (75 + (100 * i)), 1080 - (145 + (i * 65) + (120 * j)), 50, 50), i * 8 + j + 21, 1, ENEMY, ENTITY_PLAYER);
		}
	}


	int prev=NULL;
	
	SDL_RenderPresent(renderer);
	bool quit = false;
	bool dup = false;
	SDL_Event event;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	while (!quit) {
		SDL_WaitEventTimeout(&event, 10);
	

		SDL_RenderClear(renderer);
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			if ((*it)->Callback(event)) {
				break;
			}
		}
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			Entity* entity = (*it);
			if (entity->flag == false) {
				//�ִϸ��̼� ���� �ƴ�
				entity->RenderEntity();
			}
		}
		int size = idQ.size();
		for (int i = 0; i < size; i++) {
			int id = idQ.front(); idQ.pop();
			Entity * entity = map_enti.find(id)->second;
			if (entity->v.empty()) {
				entity->flag = false;
				entity->addPlayer();
			}
			else {
				SDL_Point p = entity->v.back(); entity->v.pop_back();
				entity->Animation(p);
				idQ.push(id);
			}
			entity->RenderEntity();
		}
		
		
		SDL_RenderPresent(renderer);
	}





}