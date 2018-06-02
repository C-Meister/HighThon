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


typedef struct Entities {
	Entity *entities;
	int size;
};


class Entity {

private:
	SDL_Rect reg;
	SDL_Rect src, dst;
	SDL_Texture *img;
	SDL_Renderer *renderer;
	int power;
	int id;
	bool team;

	
public:

	Entity(string filename, SDL_Rect dst, int power, int id, bool team) {
		this->power = power;
		this->id = id;
		this->team = team;
		
	}

	void RenderEntity() {
		SDL_RenderCopy(renderer, img, &src, &dst);
	}



};

class My_Rect {

private:
	SDL_Rect rect;




};