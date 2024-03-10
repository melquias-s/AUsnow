//*************************AUsnow**********************//
//*****************teammates:**************//
#include<SDL.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<SDL_ttf.h>//字体处理相关库

#undef main
//*****************created by 周书逸 at 2024/3/10
//宽、高
#define Width 1000
#define Height 750
//基本图像处理变量
SDL_Surface* surf = NULL;
SDL_Window* win = NULL;
SDL_Renderer* rdr = NULL;
SDL_AudioDeviceID audioDevice;
//范围为整个屏幕的矩形
SDL_Rect wholerect = { 0,0,Width,Height };
//音频相关变量
Uint32 wavLength;
Uint8* wavBuffer;

int quit = 0;
//记录鼠标位置
SDL_Point mousept;

//图片音频地址
const char picadress[] = "../res/pic/";
const char musadress[] = "../res/music/";

//销毁图片渲染相关变量
void Destorypic(SDL_Surface* img_surf, SDL_Texture* texture)
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(img_surf);
}

//图片渲染函数
void Putpicture(const char file[], SDL_Rect rect)
{
	char filename[50];
	SDL_snprintf(filename, 50, "%s%s", picadress, file);

	SDL_Surface* img_surf = SDL_LoadBMP(filename);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(rdr, img_surf);

	SDL_RenderCopy(rdr, texture, NULL, &rect);

	SDL_RenderPresent(rdr);

	Destorypic(img_surf, texture);
}

//文字渲染函数
void Puttext(const char *word, SDL_Rect *rect)
{
	SDL_Color color = { 255, 255, 255, 255 };
	TTF_Font* font = TTF_OpenFont("../res/font/segoeprb.ttf", 72); // 选择一个字体和字号
	if (!font)
	{
		SDL_Log("TTF_OpenFont failed : %s", TTF_GetError());
		return;
	}
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, word, color); // 渲染文本到表面
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(rdr, text_surface); // 创建纹理
	
	SDL_RenderCopy(rdr, text_texture, NULL, rect);
	SDL_RenderPresent(rdr);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	TTF_CloseFont(font); 
}

//播放音乐函数
void Playmusic(const char file[])
{
	char filename[50];
	SDL_snprintf(filename, 50, "%s%s", musadress, file);

	SDL_AudioSpec wavSpec;

	SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength);

	audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	int success = SDL_QueueAudio(audioDevice, wavBuffer, wavLength);

	SDL_PauseAudioDevice(audioDevice, 0);

}

//背景动画
void background_gif()
{
	static int index=1;
	char filename[100] = { 0 };
	if (index > 100)
	{
		index = 1;
	}
	SDL_snprintf(filename, 100, "dir/Image%d.bmp", index++);

	Putpicture(filename, wholerect);
}


int  menuloop(SDL_Rect rect_name, SDL_Rect rect_begin, SDL_Rect rect_exit)
{
	SDL_Event event;
	while (!quit)
	{
		background_gif();
		//相关目录选项
		Puttext("Cyberpunk:Ark of the Past", &rect_name);
		Puttext("BEGIN THE GAME", &rect_begin);
		Puttext("EXIT THE GAME", &rect_exit); 

		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT://退出
				quit = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mousept.x = event.motion.x;
				mousept.y = event.motion.y;
				if (SDL_PointInRect(&mousept, &rect_begin))//选择进入游戏
				{
					return 1;
				}
				if (SDL_PointInRect(&mousept, &rect_exit))//选择退出游戏
				{
					return 2;
				}
				break;
			default:
				break;
			}
		}

	}
}

int init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		SDL_Log("Init failed : %s", SDL_GetError());
		return -1;
	}
	//创建窗口
	win = SDL_CreateWindow("Hello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height,
		SDL_WINDOW_RESIZABLE);
	if (NULL == win)
	{
		SDL_Log("SDL_CreateWindow failed : %s", SDL_GetError());
		return -1;
	}
	rdr = SDL_CreateRenderer(win, -1, 0);
	if (NULL == rdr)
	{
		SDL_Log("SDL_CreateRenderer failed : %s", SDL_GetError());
		return -1;
	}
	if (TTF_Init() < 0)
	{
		SDL_Log("TTF_Init failed : %s", TTF_GetError());
		return -1;
	}
	return 0;
}

//矩形大小设置
void SetRect(int x, int y, int w, int h,SDL_Rect *rect)
{
	rect->x = x;rect->y = y;
	rect->w = w;rect->h = h;
}

//初始界面
void menu()
{
	SDL_Rect game_name, game_begin, game_exit;
	SetRect((Width - 450) / 2, Height / 6, 450, 100, &game_name);
	SetRect((Width - 150) / 2, Height / 2 - 50, 150, 50, &game_begin);
	SetRect((Width - 150) / 2, Height / 2 + 50, 150, 50, &game_exit);

	Playmusic("background.wav");
	int choice=menuloop(game_name,game_begin, game_exit);
	if (choice == 1)
	{

	}
	else if (choice == 2)
	{

	}
	else
	{

	}
}

//销毁相关全局变量
void deinit()
{
	//销毁窗口
	//SDL_FreeSurface(bmp_surf);
	
	SDL_FreeWAV(wavBuffer);
	SDL_CloseAudioDevice(audioDevice);

	SDL_FreeSurface(surf);
	SDL_DestroyRenderer(rdr);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();
}

int main()
{
	if (init() < 0)
	{
		SDL_Log("Init failed");
	}

	menu();

	deinit();

	return 0;
}

