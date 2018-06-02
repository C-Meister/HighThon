#pragma once


#include <stdio.h>				//Standard Input/Output
#include <stdlib.h>				//malloc ???
#include <math.h>				//???? ???? ???
#include <process.h>			//Thread ??? ???
#include <Windows.h>			//Window api ???? ???
#include <signal.h>				//Interrupt?? ???
#include <time.h>				//??? ???? ???
#include <stdbool.h>			//Bool ??? ???
#include <stdint.h>				//???? typedef ???? ??? ???
#include <direct.h>				//???? ???? ???
#include <WinInet.h>
#include <tchar.h>
#include <string>
#include <vector>
#include "lib//iconv.h"
#include "SDL/SDL.h"			//SDL - ?? ???????
#include "SDL/SDL_image.h"		//SDL - ????? ???????
#include "SDL/SDL_ttf.h"		//SDL - ???(????) ???????
#include "SDL/han2unicode.h"	//han2unicode - ?? 949 ?????? uniocde?? ?????
#include "SDL/SDL_mixer.h"		//SDL - ???? ???????4


#pragma comment (lib, "lib/libiconv.lib")
#pragma comment (lib, "lib/SDL2")			//????? ??????? 1
#pragma comment (lib, "lib/SDL2main")		//????? ??????? 2 
#pragma comment (lib, "lib/SDL2_image")		//????? ??????? 3
#pragma comment (lib, "lib/SDL2_ttf")		//????? ??????? 4
#pragma comment (lib, "lib/SDL2_mixer.lib")	//????? ???? ??????? 5
#pragma comment (lib, "ws2_32.lib")			//???? ???????
#pragma comment (lib, "lib/libmysql.lib")	//MySQL ???????
#pragma comment (lib, "wininet")


using namespace std;

#define POWER_DEFAULT 1
#define ENEMY 1
#define ALLY 0
#define TEAM_DEFAULT ALLY



SDL_Rect Rect(int x, int y, int w, int h) {
	SDL_Rect rect = { x,y,w,h };
	return rect;
}

SDL_Point Point(int x, int y) {
	SDL_Point point = { x,y };
	return point;
}

class Entity {

public:
	SDL_Rect reg;
	SDL_Rect src, dst;
	SDL_Texture *img;
	SDL_Renderer *renderer;
	int power;
	int id;
	bool team;

	
	

	Entity(SDL_Renderer * renderer,string filename, SDL_Rect dst, SDL_Rect reg, int id, int power=POWER_DEFAULT ,  bool team = TEAM_DEFAULT) {
		this->power = power;
		this->renderer = renderer;
		this->id = id;
		this->team = team;
		this->img = IMG_LoadTexture(renderer, filename.c_str());
		this->dst = dst;
		this->src.x = 0;
		this->src.y = 0;
		this->reg = reg;
		SDL_QueryTexture(this->img, NULL, NULL, &this->src.w, &this->src.h);
	}

	void RenderEntity() {
		SDL_RenderCopy(renderer, img, &src, &dst);
	}

	void Callback(SDL_Event event) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			SDL_Point point = Point(event.button.x, event.button.y);
			if (SDL_PointInRect(&point, &this->reg)) {

			}
		}
		
	}



};
typedef struct Entities {
	Entity *entities;
	int size;
}Entities;


