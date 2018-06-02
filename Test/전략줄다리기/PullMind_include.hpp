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
#include <iostream>
#include <tchar.h>
#include <list>
#include <string>
#include <vector>
#include <queue>
#include <map>
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

class Entity;
#define POWER_DEFAULT 1
#define ENEMY 1
#define ALLY 0
#define TEAM_DEFAULT ALLY

typedef vector<SDL_Point> VecP;
typedef vector<Entity *> VEC_ENTI;
typedef map<int, Entity *> MAP_ENTI;


extern VEC_ENTI vec_enti;
extern MAP_ENTI map_enti;
extern queue<int> idQ;

void PrintPoint(SDL_Point p, string str);
bool compPoint(SDL_Point p1, SDL_Point p2);
void PrintVecP(VecP v, string str);
void PrintRect(SDL_Rect rect, string str );
SDL_Rect Rect(int x, int y, int w, int h);
void moveRect(SDL_Rect& rect, SDL_Point point);
SDL_Point Point(int x, int y);
void getPoints(VecP& v, SDL_Point p1, SDL_Point p2);


class Entity {

public:
	SDL_Rect reg;
	SDL_Rect src = { 0,0,0,0 }, dst;
	SDL_Texture *img;
	SDL_Renderer *renderer;
	VecP v;
	SDL_Point center, point= Point(-1, -1),point2;
	int id;
	int power;
	bool team;
	bool flag = false;
		


	Entity(SDL_Renderer * renderer,string filename, SDL_Rect dst, SDL_Rect reg, int id, int power=POWER_DEFAULT ,  bool team = TEAM_DEFAULT) {
		this->power = power;
		this->renderer = renderer;
		this->team = team;
		this->id = id;
		this->img = IMG_LoadTexture(renderer, filename.c_str());
		this->dst = dst;
		this->reg = reg;
		this->center = Point(dst.x + dst.w / 2, dst.y + dst.h / 2);
		SDL_QueryTexture(this->img, NULL, NULL, &this->src.w, &this->src.h);
		vec_enti.push_back(this);
		map_enti.insert(make_pair(id, this));
	}

	void RenderEntity() {
		SDL_RenderCopy(renderer, img, &src, &dst);
	}
	void PrintInfo() {
		PrintPoint(this->center, "center");
		PrintPoint(this->point, "point");
		PrintPoint(this->point2, "point2");
		PrintRect(this->reg, "reg");
		PrintRect(this->src, "src");
		PrintRect(this->dst, "dst");
	}
	void Callback(SDL_Event event) {
		if (flag)
			return;
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			point2 = Point(event.button.x, event.button.y);
			if (SDL_PointInRect(&point2, &this->reg)) { //영역선택 시
				cout << "영역" << endl;
				this->point = this->center;
			}
			else if (!compPoint(point, Point(-1, -1))) {
				cout << "영역바깥 그리고 -1,-1이 아님" << endl;
				getPoints(v, this->point, point2);
				reverse(v.begin(), v.end());
				flag = true;
				this->center = point2;
				moveRect(this->reg, center);
				this->point = Point(-1, -1);
				idQ.push(id);
			}
		}
	}
	void Animation(SDL_Point p) {
		
			moveRect(this->dst, p);

		
	}

};




void PrintPoint(SDL_Point p, string str = "") {
	cout << str << "	x: " << p.x << ", " << "y: " << p.y << endl;
}
bool compPoint(SDL_Point p1, SDL_Point p2) {

	return p1.x == p2.x&&p1.y == p2.y;

}
void PrintVecP(VecP v, string str = "") {
	cout << str << endl;
	for (auto it = v.begin(); it != v.end(); it++) {
		PrintPoint(*it);
	}
}
void PrintRect(SDL_Rect rect, string str = "") {
	cout << str << "x: " << rect.x << ", " << "y: " << rect.y << "w: " << rect.w << ", " << "h: " << rect.h << endl;

}
SDL_Rect Rect(int x, int y, int w, int h) {
	SDL_Rect rect = { x,y,w,h };
	return rect;
}
void moveRect(SDL_Rect& rect, SDL_Point point) {//point가 중점좌표
	rect = { point.x - rect.w / 2,point.y - rect.h / 2,rect.w,rect.h };
}
SDL_Point Point(int x, int y) {
	SDL_Point point = { x,y };
	return point;
}

void getPoints(VecP& v, SDL_Point p1, SDL_Point p2)
{
	int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
	int      dx, dy;
	int      p_value;
	int      inc_2dy;
	int      inc_2dydx;
	int      inc_value;
	int      ndx;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if (dy <= dx)
	{
		inc_2dy = 2 * dy;
		inc_2dydx = 2 * (dy - dx);
		if (x2 < x1)
		{
			ndx = x1;
			x1 = x2;
			x2 = ndx;

			ndx = y1;
			y1 = y2;
			y2 = ndx;
		}
		if (y1 < y2)  inc_value = 1;
		else inc_value = -1;
		v.push_back(Point(x1, y1));
		p_value = 2 * dy - dx;
		for (ndx = x1; ndx < x2; ndx++)
		{
			if (0 > p_value)    p_value += inc_2dy;
			else
			{
				p_value += inc_2dydx;
				y1 += inc_value;
			}
			v.push_back(Point(ndx, y1));
		}
	}
	else
	{
		inc_2dy = 2 * dx;
		inc_2dydx = 2 * (dx - dy);

		if (y2 < y1)
		{
			ndx = y1;
			y1 = y2;
			y2 = ndx;

			ndx = x1;
			x1 = x2;
			x2 = ndx;
		}
		if (x1 < x2)  inc_value = 1;
		else           inc_value = -1;
		v.push_back(Point(x1, y1));
		p_value = 2 * dx - dy;
		for (ndx = y1; ndx < y2; ndx++)
		{
			if (0 > p_value)    p_value += inc_2dy;
			else
			{
				p_value += inc_2dydx;
				x1 += inc_value;
			}
			v.push_back(Point(x1, ndx));
		}
	}
	if (!compPoint(v.front(), p1)) {
		reverse(v.begin(), v.end());
	}

}


