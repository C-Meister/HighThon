
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SDL/SDL.h"
#include "PullMind_include.hpp"


#undef main 

VEC_ENTI vec_enti;
MAP_ENTI map_enti;
vector<int> R_status(5);
SDL_Point drag_first = Point(-1, -1);

queue<int> idQ;


void TTF_DrawTextUnicode(SDL_Renderer* renderer, string text, SDL_Point point, TTF_Font *font, SDL_Color color = { 0,0,0,0 }) {
	Uint16 text2[50];
	han2unicode(text.c_str(), text2);
	SDL_Surface * surface = TTF_RenderUNICODE_Blended(font, text2, color);
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
	SDL_Point p1, p2;
	int e_num;
	int buffint = 0;
	event.type = SDL_USEREVENT;
	while (recv(server, msg, 30, 0) > 0) {
		printf("%s\n", msg);
		//printf("%s\n", msg);
		if (msg[3] == 'e' && msg[2] == 'v') {

			sscanf(msg, "move %1d %2d %4d,%4d %4d,%4d", &buffint, &e_num, &p1.x, &p1.y, &p2.x, &p2.y);
			
			if (buffint != my_idx) {
				printf("Ememy Turn\n");
				if (e_num - 6 <= 8) {
					e_num += 22;
				}
				else {
					e_num += 7;
				}

				p1.x = 1920 - p1.x;
				p2.x = 1920 - p2.x;


			}
			else
				printf("My Turn\n");
			printf("data : %d %d %d %d %d %d\n", buffint, e_num, p1.x, p1.y, p2.x, p2.y);
			moveEntity(e_num, p1, p2);
		}
		else if (strstr(msg, "match ") != NULL) {
			Sleep(1000);
			event.user.code = MATCHING;
			sscanf(msg, "match %[^n]s", buff);
			enemy_name = string(buff);
			event.user.data1 = buff;

		}
		else if (strstr(msg, "room ") != NULL) {
			msg[0] = 'j';
			msg[1] = 'o';
			msg[2] = 'i';
			msg[3] = 'n';
			send(server, msg, (int)strlen(msg), 0);
		}
		else if (strstr(msg, "exitroom") != NULL) {
			event.user.code = EXITING;

		}
		else if (strstr(msg, "join_success ") != NULL) {
			sscanf(msg, "join_success %d", &my_idx);
		}
		SDL_PushEvent(&event);
		memset(&event, 0, sizeof(event));
		event.type = SDL_USEREVENT;
		memset(msg, 0, sizeof(msg));
		memset(buff, 0, sizeof(buff));
	}
}

int main(void) {

	connectServer();
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ReceiveHandler, NULL, 0, NULL);
	bool loading = false;
	bool gamings = false;
	bool fquit = false;
	bool is30 = false;
	SDL_Window * Window = NULL;
	SDL_Color color = { 0,0,0 ,0 };
	SDL_Color white_color = { 255,0,0 ,0 };
	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);
	HitMind_TTF_Init();
	HitMind_TTF2_Init();

	int Display_X = 1920;
	int Display_Y = 1080;
	SDL_Window* window = SDL_CreateWindow("HitMind_2", 0, 0, 1920, 1080, SDL_WINDOW_SHOWN);
	SDL_Renderer*	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	string str;
	TTF_Init();
	TTF_Font* font = TTF_OpenFont(".\\font\\NanumGothic.ttf", 35);


	SDL_Texture *lobiimage = IMG_LoadTexture(renderer, ".\\resources\\image\\tema.jpg");
	SDL_Texture *inputimage = IMG_LoadTexture(renderer, ".\\resources\\image\\input.png");
	if (lobiimage == NULL)
		printf("");

	RenderTextureXYWH(renderer, lobiimage, 0, 0, Display_X, Display_Y);
	RenderTextureXYWH(renderer, inputimage, 710, 470, 500, 141);
	while (!fquit) {

		SDL_WaitEvent(&event);

		RenderTextureXYWH(renderer, lobiimage, 0, 0, Display_X, Display_Y);
		RenderTextureXYWH(renderer, inputimage, 710, 470, 500, 141);



		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_BACKSPACE && !str.empty())
				str.pop_back();
			else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER && !str.empty()) { //엔터입력
				matching_start(str.c_str());
				user_name = str;
				fquit = true;
				loading = true;
			} 
			break;
		case SDL_TEXTINPUT:
			str += event.text.text;
			//SDL_RenderClear(renderer);
			cout << str << endl;
			break;
		case SDL_QUIT:
			if (status == 1)
				matching_end();
			fquit = TRUE;
			break;
		default:
			break;
		}

		TTF_DrawText(renderer, str, Point(720, 550), font, color);
		SDL_RenderPresent(renderer);

	}


	
	int count = 0;
	while (loading) {
		SDL_WaitEventTimeout(&event, 500);
		
		switch (event.type) {
		case SDL_USEREVENT:
				if (event.user.code == MATCHING) {
					printf("match success\n");
					loading = false;
					gamings= true;
					//EXITING 이벤트 처리	
						
				}
				else if (event.user.code == EXITING) {
					gamings = false;
					break;
				}
			
			break;

		case SDL_QUIT:
			matching_end();
			loading = false;
			break;
		}
		
		RenderTextureXYWH(renderer, lobiimage, 0, 0, Display_X, Display_Y);
		
		string lodingS;
		if (count % 4 == 0)
			lodingS = "매칭중";
		else if (count % 4 == 1)
			lodingS = "매칭중 .";
		else if (count % 4 == 2)
			lodingS = "매칭중 . .";
		else if (count % 4 == 3)
			lodingS = "매칭중 . . .";

		TTF_DrawTextUnicode(renderer, lodingS, Point(860, 550), font, color);
		SDL_RenderPresent(renderer);
		count++;
	}

	Entity *Line[5], *entity[15], *enemies[15];

	Entity *BG = new Entity(renderer, "./resources/image/out.jpg", Rect(0, 0, 1920, 1080), Rect(0, 0, 1920, 1080), 6, 0, false, ENTITY_BG);
	
	Line[0] = new Entity(renderer, "./resources/entity/Line.png", Rect(510, 200, 900, 20), Rect(460, 200, 900, 100), 1, 0, false, ENTITY_ROPE);
	Line[1] = new Entity(renderer, "./resources/entity/Line.png", Rect(608, 386, 700, 20), Rect(560, 386, 700, 100), 2, 0, false, ENTITY_ROPE);
	Line[2]= new Entity(renderer, "./resources/entity/Line.png", Rect(510, 540, 900, 20), Rect(460, 540, 900, 100), 3, 0, false, ENTITY_ROPE);
	Line[3] = new Entity(renderer, "./resources/entity/Line.png", Rect(608, 694, 700, 20), Rect(560, 694, 700, 100), 4, 0, false, ENTITY_ROPE);
	Line[4] = new Entity(renderer, "./resources/entity/Line.png", Rect(510, 880, 900, 20), Rect(460, 880, 900, 100), 5, 0, false, ENTITY_ROPE);

	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8-i; j++) {
			entity[i * 8 + j] = new Entity(renderer, "./resources/entity/mob_11.png", Rect(25 + (100 * i), 140 + (i * 65) + (120 * j), 50, 50), Rect(25 + (100 * i), 140 + (i * 65) + (120 * j), 50, 50), i*8+j+7, 1, ALLY, ENTITY_PLAYER);
		}
	}
	for (int i = 1; i >= 0; i--) {
		for (int j = 6 + (1 - i); j >= 0; j--) {
			enemies[i * 8 + j] = new Entity(renderer, "./resources/entity/mor_11.png", Rect(1920 - (75 + (100 * i)), 1080 - (95 + (i * 65) + (120 * j)), 50, 50), Rect(1920 - (75 + (100 * i)), 1080 - (95 + (i * 65) + (120 * j)), 50, 50), i * 8 + j + 22, 1, ENEMY, ENTITY_PLAYER);
		}
	}


	int prev=NULL;
	
	SDL_RenderPresent(renderer);
	bool quit = false;
	bool dup = false;
	double limit = 30;//초단위
	SDL_Rect timer = Rect(30,100, 1820, 8);
	SDL_Color timer_color = { 255,255,0 ,0 };
	int start = clock();
	while (gamings) {
		SDL_WaitEventTimeout(&event, 0);

		SDL_RenderClear(renderer);

		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			if ((*it)->Callback(event)) {
				break;
			}
		}
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			Entity* entity = (*it);
			if (entity->focused)
				entity->drawFocus();
			if (!is30 || (entity->flag == false && entity->type != ENTITY_ENDROPE)) {
				//�ִϸ��̼� ���� �ƴ�
				entity->RenderEntity();
			}
		}
		if (is30) {
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
				entity->drawFocus();
			}
		}
		if (clock() - start >= limit * 1000) {
			is30 = true;
		}
		else {
			SDL_SetRenderDrawColor(renderer, timer_color.r, timer_color.g, timer_color.b, timer_color.a);
			SDL_RenderFillRect(renderer, &Rect(timer.x, timer.y, timer.w*(1 - (clock() - start) / (limit * 1000)), timer.h));
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		

		switch (event.type) {
		case SDL_QUIT:
			matching_end();
			gamings = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			drag_first = Point(event.button.x, event.button.y);
			break;
		case SDL_MOUSEMOTION:
			if (drag_first.x != -1) {
			//	printf("mouse motion %d %d\n", event.motion.x, event.motion.y);
				int x, y, xsz, ysz;
				if (drag_first.x > event.motion.x) {
					x = event.motion.x;
					xsz = drag_first.x - event.motion.x;
				}
				else {
					x = drag_first.x;
					xsz = event.motion.x - drag_first.x;
				}
				if (drag_first.y > event.motion.y) {
					y = event.motion.y;
					ysz = drag_first.y - event.motion.y;
				}
				else {
					y = drag_first.y;
					ysz = event.motion.y - drag_first.y;
				}
				SDL_SetRenderDrawColor(renderer, 0, 10, 10, 30);
				SDL_Rect area = Rect(x, y, xsz, ysz);
				SDL_RenderDrawRect(renderer, &area);
			}

			break;
		case SDL_MOUSEBUTTONUP:
			SDL_Delay(1);
			drag_first = Point(-1, -1);
			break;
		}
		
		Put_Text_Center(renderer, user_name, 75, 15, 210, 81, 255, 255, 255, 35, 1);           //내이름
		Put_Text_Center(renderer, enemy_name, 1635, 15, 210, 81, 255, 255, 255, 35, 1);			//적이름
		SDL_RenderPresent(renderer);
	}




}