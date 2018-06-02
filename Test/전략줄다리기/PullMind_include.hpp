#pragma once


#include <stdio.h>				//Standard Input/Output
#include <stdlib.h>				//malloc ???
#include <math.h>				//???? ???? ???
#include <process.h>			//Thread ??? ???
#include <signal.h>				//Interrupt?? ???
#include <time.h>				//??? ???? ???
#include <stdbool.h>			//Bool ??? ???
#include <stdint.h>				//???? typedef ???? ??? ???
#include <direct.h>				//???? ???? ???
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
#pragma comment (lib, "lib/libmysql.lib")	//MySQL ???????
#pragma comment (lib, "wininet")


using namespace std;

class Entity;
#define POWER_DEFAULT 1
#define ENEMY 1
#define ALLY 0
#define TEAM_DEFAULT ALLY

#define ENTITY_PLAYER 0
#define ENTITY_ROPE 1
#define ENTITY_BG 2

typedef vector<SDL_Point> VecP;
typedef vector<Entity *> VEC_ENTI;
typedef map<int, Entity *> MAP_ENTI;
typedef wchar_t Unicode;



extern VEC_ENTI vec_enti;
extern MAP_ENTI map_enti;
extern queue<int> idQ;
extern vector<int> R_status;

void PrintPoint(SDL_Point p, string str);
bool compPoint(SDL_Point p1, SDL_Point p2);
void PrintVecP(VecP v, string str);
void PrintRect(SDL_Rect rect, string str );
SDL_Rect Rect(int x, int y, int w, int h);
void moveRect(SDL_Rect& rect, SDL_Point point);
SDL_Point Point(int x, int y);
void getPoints(VecP& v, SDL_Point p1, SDL_Point p2);
double getAngle(SDL_Point p1, SDL_Point p2);
vector<int> getStatus();
void printStatus();


// --------------
class Entity {

public:
	SDL_Rect reg;
	SDL_Rect src = { 0,0,0,0 }, dst;
	SDL_Texture *img;
	SDL_Renderer *renderer;
	VecP v;
	MAP_ENTI m_e;
	SDL_Point center, point2;
	int id;
	int power;
	bool team;
	bool flag = false;
	bool focused = false;
	int type;
	double angle = 0;
	
		


	Entity(SDL_Renderer * renderer,string filename, SDL_Rect dst, SDL_Rect reg, int id, int power = POWER_DEFAULT, bool team = TEAM_DEFAULT, int type = ENTITY_PLAYER) {
		this->power = power;
		this->renderer = renderer;
		this->team = team;
		this->id = id;
		this->img = IMG_LoadTexture(renderer, filename.c_str());
		this->dst = dst;
		this->reg = reg;
		this->center = Point(dst.x + dst.w / 2, dst.y + dst.h / 2);
		this->type = type;
		SDL_QueryTexture(this->img, NULL, NULL, &this->src.w, &this->src.h);
		vec_enti.push_back(this);
		map_enti.insert(make_pair(id, this));
		RenderEntity();
	}
	

	void RenderEntity() {
		SDL_RenderCopyEx(renderer, img, &src, &dst, angle,NULL, SDL_FLIP_NONE);
	}
	void PrintInfo() {
		PrintPoint(this->center, "center");
		PrintPoint(this->point2, "point2");
		PrintRect(this->reg, "reg");
		PrintRect(this->src, "src");
		PrintRect(this->dst, "dst");
	}
	bool Callback(SDL_Event event) {
		if (type == ENTITY_BG)
			return false;

		if (type == ENTITY_ROPE) {
			center.x -= R_status[id - 1];
			moveRect(this->dst, center);
			moveRect(this->reg, center);
			for (auto it = m_e.begin(); it != m_e.end(); it++) {
				Entity *entity = (*it).second;
				entity->center.x -= R_status[id - 1];
				moveRect(entity->dst, entity->center);
				moveRect(entity->reg, entity->center);
				entity->RenderEntity();
			}
			RenderEntity();
			
		}

		if (flag)
			return false;

		switch (event.type) { 
		case SDL_MOUSEBUTTONDOWN:
		//	cout << (int)event.button.button<<endl;
			point2 = Point(event.button.x, event.button.y);
			switch (this->type) {
			case ENTITY_PLAYER:
				if (event.button.button == SDL_BUTTON_LEFT && SDL_PointInRect(&point2, &this->reg)) {
					this->focused = true;
					cancelfocus();
					return true;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT && focused) {
					getPoints(v, this->center, point2);
					angle = getAngle(this->center, point2);
					if (team == ENEMY)
						angle += 180;
					reverse(v.begin(), v.end());
					flag = true;
					this->center = point2;
					moveRect(this->reg, center);
					this->focused = false;
					idQ.push(id);
					removePlayer();
					return false;
				}
				else {
					this->focused = false;
				}
				break;
		
			}
		}
		return false;
	}
	void Animation(SDL_Point p) {
			center = p;
			moveRect(this->dst, center);
	}
	void removePlayer() {
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			if ((*it)->type == ENTITY_ROPE&& !SDL_PointInRect(&point2, &(*it)->reg)) {
				(*it)->m_e.erase(id);
			}
		}
		R_status = getStatus();
	}
	
	void addPlayer() {
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			if ((*it)->type == ENTITY_ROPE && SDL_PointInRect(&point2, &(*it)->reg)) {
				(*it)->m_e.insert(make_pair(id, this));

			}
		}
		R_status = getStatus();
	}
	void cancelfocus() {
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			if ((*it)->type == ENTITY_PLAYER && (*it)->focused && (*it)->id != this->id)
				(*it)->focused = false;
		}
	}

}; 
void printStatus() {

	vector<int>status = getStatus();
	for (auto it = status.begin(); it != status.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
vector<int> getStatus() {
	vector<int> v;
	for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
		if ((*it)->type == ENTITY_ROPE) {
			int sum = 0;
			MAP_ENTI m_e = (*it)->m_e;
			for (auto it2 = m_e.begin(); it2 != m_e.end(); it2++) {
				if(it2->second->team==ENEMY)
					sum -= (*it2).second->power;
				else
					sum += (*it2).second->power;
			}
			v.push_back(sum);
		}
	}
	return v;
}

double getAngle(SDL_Point p1, SDL_Point p2) {
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;

	int dx = x2 - x1;
	int dy = y2 - y1;

	double rad = atan2(dy, dx );
	double degree = (rad * 180) / M_PI;

	return degree;
}
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
void moveRect(SDL_Rect& rect, SDL_Point point) {//point�� ������ǥ
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


