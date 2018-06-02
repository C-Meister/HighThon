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

typedef vector<SDL_Point> VecP;
typedef vector<Entity *> VEC_ENTI;
typedef map<int, Entity *> MAP_ENTI;
typedef wchar_t Unicode;



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
// 입력창 함수
TTF_Font * Font_Size[100];
TTF_Font * Font_Size2[100];

void HitMind_TTF_Init();
void HitMind_TTF_Close();
void HitMind_TTF2_Init();
void HitMind_TTF2_Close();
void RenderTextureXYWH(SDL_Renderer* Renderer, SDL_Texture * Texture, double xx, double yy, double ww, double hh);
int PutText_Unicode(SDL_Renderer * renderer, Unicode * unicode, unsigned int x, unsigned int y, int size, SDL_Color color, int m);
SDL_Texture * LoadTexture(SDL_Renderer * Renderer, const char *file);
int TTF_DrawText(SDL_Renderer *Renderer, TTF_Font* Font, wchar_t* sentence, int x, int y, SDL_Color Color);
int TTF_DrawText(SDL_Renderer *Renderer, TTF_Font* Font, Uint16* sentence, int x, int y, SDL_Color Color) {
	SDL_Surface * Surface = TTF_RenderUNICODE_Blended(Font, sentence, Color);// 폰트의 종류,문자열, 색깔을 보내서 유니코드로 렌더한다음 서피스에 저장한다
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);// 서피스로부터 텍스쳐를 생성한다
	SDL_FreeSurface(Surface);//서피스 메모리를 해제 해준다.
	SDL_Rect Src;
	Src.x = 0;
	Src.y = 0;
	SDL_QueryTexture(Texture, NULL, NULL, &Src.w, &Src.h);
	SDL_Rect Dst;
	Dst.x = x;
	Dst.y = y;
	Dst.w = Src.w;
	Dst.h = Src.h;
	SDL_RenderCopy(Renderer, Texture, &Src, &Dst); //그대로 렌더러에 저장한다
	SDL_DestroyTexture(Texture);
	return Src.w;// 출력할 문자열의 너비를 반환
}
int PutText_Unicode(SDL_Renderer * renderer, Uint16 * unicode, unsigned int x, unsigned int y, int size, SDL_Color color, int m)
{

	if (m == 1)
		TTF_DrawText(renderer, Font_Size[size], unicode, x, y, color);			//Text를 적음
	else if (m == 2)
		TTF_DrawText(renderer, Font_Size2[size], unicode, x, y, color);

	return 0;	//평소에도 0을 리턴
}
SDL_Texture * LoadTexture(SDL_Renderer * Renderer, const char *file) { // 텍스쳐에 이미지파일 로드하는 함수 선언
	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;// JPG파일과 PNG파일 로드 가능
	if (IMG_Init(imgFlags) != imgFlags) {//IMG 초기화하고 초기화 안되면 if문 실행

		return nullptr;//널포인터 반환
	}
	SDL_Surface* Surface = IMG_Load(file);//서피스에 이미지로드
	if (Surface == nullptr) {//서피스에 이미지로드가 안되면 

		IMG_Quit();// IMG 종료
		return nullptr;// 널포인터 반환
	}
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);//서피스로부터 텍스쳐 생성
	SDL_FreeSurface(Surface);// 서피스 메모리해제
	if (Texture == nullptr) {// 텍스쳐 생성 실패시 if문실행

		IMG_Quit();// IMG 종료
		return nullptr;// 널포인터 반환
	}
	IMG_Quit();// IMG 종료
	return Texture;// Texture포인터 반환
}
void RenderTextureXYWH(SDL_Renderer* Renderer, SDL_Texture * Texture, double xx, double yy, double ww, double hh) {//텍스쳐를 출력하는 함수 선언
	SDL_Rect Src;// 직사각형 선언
	Src.x = 0;// 직사각형의 왼쪽위 꼭짓점의 x좌표초기화
	Src.y = 0;// 직사각형의 왼쪽위 꼭짓점의 y좌표초기화
	SDL_QueryTexture(Texture, NULL, NULL, &Src.w, &Src.h); // Texture의 너비와 높이 정보를 Src.w, Src.h에 저장
	SDL_Rect Dst;
	Dst.x = round(xx);//매개변수x를 왼쪽위 꼭짓점의 x좌표에 대입
	Dst.y = round(yy);//매개변수y를 왼쪽위 꼭짓점의 y좌표에 대입
	Dst.w = round(ww);//매개변수w를 직사각형의 너비에 대입
	Dst.h = round(hh);//매개변수h를 직사각형의 높이에 대입
	SDL_RenderCopy(Renderer, Texture, &Src, &Dst);//Src의 정보를 가지고 있는 Texture를 Dst의 정보를 가진 Texture 로 변환하여 렌더러에 저장
	return;
}
void HitMind_TTF_Init()
{
	for (int i = 0; i < 100; i++)
	{
		Font_Size[i] = TTF_OpenFont(".\\font\\NanumGothic.ttf", i);
	}
}
void HitMind_TTF_Close() {

	for (int i = 0; i < 100; i++)
	{
		TTF_CloseFont(Font_Size[i]);
	}

}
void HitMind_TTF2_Init()
{
	for (int i = 0; i < 100; i++)
	{
		Font_Size2[i] = TTF_OpenFont(".\\font\\NanumGothicBold.ttf", i);
	}
}
void HitMind_TTF2_Close() {

	for (int i = 0; i < 100; i++)
	{
		TTF_CloseFont(Font_Size2[i]);
	}

}
// --------------
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
	bool focus = false;
		


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
	bool Callback(SDL_Event event) {
		if (flag)
			return false;
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			cout << (int)event.button.button<<endl;
			point2 = Point(event.button.x, event.button.y);
				if (event.button.button==SDL_BUTTON_LEFT&&SDL_PointInRect(&point2, &this->reg)) { 
					cout << "영역" << endl;
					this->point = this->center;
					return true;
				}
				

			
				else if (event.button.button == SDL_BUTTON_RIGHT&&!compPoint(point, Point(-1, -1))) {
				cout << "영역바깥 그리고 -1,-1이 아님" << endl;
				getPoints(v, this->point, point2);
				reverse(v.begin(), v.end());
				flag = true;
				this->center = point2;
				moveRect(this->reg, center);
				this->point = Point(-1, -1);
				idQ.push(id);
				return true;
				}
		}
		return false;
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


