#include "SDL/SDL.h"
#include "socket.h"
#include "PullMind_include.hpp"
#include "main2.h"

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib , "lib/SDL2.lib")
#undef main 

VEC_ENTI vec_enti;
MAP_ENTI map_enti;

queue<int> idQ;

int main(void) {

#ifdef SOOHAN
	
	bool quit = false;
	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window * Window = NULL;
	SDL_Color color = { 0,0,0 ,0 };
	TTF_Init();
	HitMind_TTF_Init();
	HitMind_TTF2_Init();


	int backspacehappen = false;
	wchar_t change_password[5][256] = { L"", L"", L"", L"" };
	int enter = false; // ���Ͱ� �ԷµǾ����� �˷��ִ� �� ����
	int textinput = true; // ���ڰ� �ϳ� �� �ԷµǾ����� �˷��ִ� �� ����
	int hangeul = false; // ���� �Է��ϰ� �ִ� ���ڰ� �ѱ����� �ƴ��� �ĺ����ִ� �� ����
	int hanyeong = false; // �ѿ�Ű���¿� ���̴� �� ����
	wchar_t wchar[2] = L""; // �ѱ��� �ѱ��� �Է¿� ���̴� �迭
	int slice = 0;
	wchar_t name_put[255] = L"";	//�̸� �Է� �迭


	Display_X = 1920;
	Display_Y = 1080;
	Window = SDL_CreateWindow("HitMind_2", 100, 100, Display_X, Display_Y, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_SOFTWARE);


	SDL_Texture *lobiimage = LoadTexture(renderer, ".\\maintema.png");
	SDL_Texture *inputimage = LoadTexture(renderer, ".\\input.png");

	RenderTextureXYWH(renderer, lobiimage, 0, 0, Display_X, Display_Y);
	RenderTextureXYWH(renderer, inputimage, 710, 470, 500, 141);
	while (!quit) {

		SDL_WaitEvent(&event);

		RenderTextureXYWH(renderer, lobiimage, 0, 0, Display_X, Display_Y);
		RenderTextureXYWH(renderer, inputimage, 710, 470, 500, 141);
		switch (event.type)
		{
		case SDL_TEXTINPUT: // ä�� �Է� �̺�Ʈ
							//	printf("��");
			if (hanyeong == true && (event.text.text[0] == -29 || event.text.text[0] + 256 >= 234 && event.text.text[0] + 256 <= 237))// �ѱ��� ���
			{
				wcscpy(wchar, L"");
				int sum = (event.text.text[0] + 22) * 64 * 64 + (event.text.text[1] + 128) * 64 + event.text.text[2] + 41088;
				wchar[0] = sum;
				if (wcslen(name_put) < 255)
					wcscat(name_put, wchar);
				if (event.text.text[0] == -29)
					slice = 1;
				else
					slice = 1 + !((wchar[0] - 0xac00) % 28);
			}
			else if (!((event.text.text[0] == 'c' || event.text.text[0] == 'C') && (event.text.text[0] == 'v' || event.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL)) {// ���� �Է� ��
				wcscpy(wchar, L"");
				swprintf(wchar, sizeof(wchar) / sizeof(wchar_t), L"%hs", event.text.text);// event.text.text ���ڿ� �׳� ������ѹ���
				if (wcslen(name_put) < 255)
					wcscat(name_put, wchar);
				hangeul = false;
				slice = 0;
}
			textinput = true;
			break;
		case SDL_KEYDOWN:
			//	printf("��2");
			if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
				if (hangeul == true && enter == false)
					enter = true;
				else {

					printf("����");



					enter = false;

					//textinput = true;
				}
			}
			else if (event.key.keysym.sym == SDLK_TAB)
			{
				if (hangeul == true && enter == false)
					enter = true;
				else {

				}
			}
			else if (event.key.keysym.sym == SDLK_RALT)
				hanyeong = !(hanyeong);
			else if (event.key.keysym.sym == SDLK_BACKSPACE && wcslen(name_put) > 0)// Ű���� �齺���̽��� �迭�� ���̰� 1�̻��϶�
			{
				name_put[wcslen(name_put) - 1] = 0;
				backspacehappen = true;
			}
			else {
				hangeul = true;
				slice++;
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;

		}
		PutText_Unicode(renderer, name_put, 720, 549, 40, color, 1);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	HitMind_TTF_Close();
	HitMind_TTF2_Close();
	return 0;

#else

	SDL_Init(SDL_INIT_EVERYTHING);
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)connectServer, NULL, 0, NULL);

	SDL_Window *window = SDL_CreateWindow("hi", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Entity *entity = new Entity(renderer, "./resources/image/porg.jpg", Rect(100, 100, 100, 100), Rect(100, 100, 100, 100), 1, 1, 1);
	Entity *entity2 = new Entity(renderer, "./resources/image/porg.jpg", Rect(300, 100, 100, 100), Rect(300, 100, 100, 100), 2, 1, 1);
	Entity *entity3 = new Entity(renderer, "./resources/image/porg.jpg", Rect(300, 300, 100, 100), Rect(300, 300, 100, 100), 3, 1, 1);


	entity->RenderEntity();
	entity2->RenderEntity();
	entity3->RenderEntity();
	SDL_RenderPresent(renderer);
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_WaitEventTimeout(&event, 10);
		SDL_RenderClear(renderer);
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
			}
			else {
				SDL_Point p = entity->v.back(); entity->v.pop_back();
				entity->Animation(p);
				idQ.push(id);
			}
			entity->RenderEntity();
		}



		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			if ((*it)->Callback(event)) { break; }

		}
		SDL_RenderPresent(renderer);
	}

#endif


	
}