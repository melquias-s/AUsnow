#include<SDL.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>

#define W 800
#define H 600
#undef main

SDL_Window* win = NULL;
SDL_Renderer* rdr = NULL;
//SDL_Surface* bmp_surf = NULL;
SDL_Rect rect = { 200,200,100,100 };
SDL_Point point;

int init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		SDL_Log("Init failed : %s", SDL_GetError());
		return -1;
	}
	//创建窗口
	win = SDL_CreateWindow("Hello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H,
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
	/*
	bmp_surf = SDL_LoadBMP("8.bmp");
	if (NULL == bmp_surf)
	{
		SDL_Log("SDL_LoadBMP failed : %s", SDL_GetError());
		return -1;
	}
	*/
	return 0;
}

void deinit()
{
	//销毁窗口
	//SDL_FreeSurface(bmp_surf);
	//SDL_FreeSurface(surf);
	SDL_DestroyRenderer(rdr);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

int drawrender()
{
	//1. 创建与窗口关联的渲染器
	//2. 设置渲染颜色
	SDL_SetRenderDrawColor(rdr, 0, 255, 255, 255);
	//3. 清除屏幕
	SDL_RenderClear(rdr);//用设置的渲染颜色“刷”屏幕
	SDL_SetRenderDrawBlendMode(rdr, SDL_BLENDMODE_BLEND);
	//渲染点、线
	/*
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_RenderDrawPoint(rdr, 200, 200);
	for (int i = 200; i < 300; i += 5)
	{
		SDL_RenderDrawPoint(rdr, i, 200);
	}
	SDL_RenderDrawLine(rdr, 0, 0, 200, 250);

	SDL_Point pt[5] = { {100,100}, {200,200}, {300,400}, {200,400}, {150,450} };
	SDL_RenderDrawLines(rdr, pt, 5);
	*/

	//练习：绘制一个19*19的围棋棋盘
	/*
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	for (int i = 100; i <= 280; i += 10)
	{
		SDL_RenderDrawLine(rdr, 100, i, 280, i);
	}
	for (int i = 100; i <= 280; i += 10)
	{
		SDL_RenderDrawLine(rdr, i, 100, i, 280);
	}
	*/

	//渲染矩形
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);


	//SDL_RenderDrawRect(rdr, &rect);
	SDL_RenderFillRect(rdr, &rect);
	//4. 渲染呈现
	SDL_RenderPresent(rdr);

	return 0;
}

int drawsurf()
{
	//修改像素点颜色
	//获取与窗口关联的surface上
	SDL_Surface* surf = SDL_GetWindowSurface(win);
	if (NULL == surf)
	{
		SDL_Log("SDL_GetWindowSurface failed : %s", SDL_GetError());
		return -1;
	}
	//修改像素点颜色值
	Uint32* px = (Uint32*)surf->pixels;
	px[20000] = 0xffffff;

	for (int i = 20000; i < 21000; i += 3)
	{
		px[i] = 0xffffff;
		//px[i] = SDL_MapRGB(surf->format, i % 225, (i + 100) % 255, (i + 200) % 255);
	}
	// 将绘制的内容更新到屏幕上
	SDL_UpdateWindowSurface(win);

	//绘制矩形
	//获取与窗口关联的surface上
	//定义一个区域
	SDL_Rect rect = { 100, 100, 20, 20 };
	//在surface上进行绘制
	SDL_FillRect(surf, &rect, 0xff0000);
	//SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format,255,0,0));
	//将绘制的内容更新到屏幕上
	SDL_UpdateWindowSurface(win);

	//显示BMP图片
	//获取与窗口关联的surface上 （与绘制矩形一样）
	//导入bmp图片
	SDL_Surface* bmp_surf = SDL_LoadBMP("8.bmp");
	if (NULL == bmp_surf)
	{
		SDL_Log("SDL_LoadBMP failed : %s", SDL_GetError());
		return -1;
	}
	// 将图片复制到窗口surface上
	SDL_BlitSurface(bmp_surf, NULL, surf, NULL);
	//将绘制的内容更新到屏幕上
	SDL_UpdateWindowSurface(win);
	//暂停一会---->事件监听与响应
	//SDL_Delay(3000); // unit:ms
	return 0;
}

void mouseevent()
{

}

void eventloop()
{
	SDL_Event event;
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			//SDL_Log("Event Type : %d", event.type);
			switch (event.type)
			{
			case SDL_QUIT:
				printf("quit\n");
				return;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED)
				{
					SDL_Log("Maxwindow!\n");
					break;
				}
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					drawrender();
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_Log("x = %d, y = %d, xrel = %d, yrel = %d", event.motion.x, event.motion.y, event.motion.xrel,
					event.motion.yrel);
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_Log("x = %d, y = %d, button = %d,clicks = %d", event.motion.x, event.motion.y, event.button.button, event.button.clicks);
				point.x = event.motion.x;
				point.y = event.motion.y;
				if (SDL_PointInRect(&point, &rect))
				{
					SDL_SetRenderDrawColor(rdr, 0, 100, 255, 255);
					SDL_RenderFillRect(rdr, &rect);
					SDL_RenderPresent(rdr);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				SDL_Log("x = %d, y = %d, button = %d,clicks = %d", event.motion.x, event.motion.y, event.button.button, event.button.clicks);
				SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
				SDL_RenderFillRect(rdr, &rect);
				SDL_RenderPresent(rdr);
				break;
			case SDL_MOUSEWHEEL:
				SDL_Log("slide...");
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_UP)
					rect.y -= 10;
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
					rect.y += 10;
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
					rect.x -= 10;
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
					rect.x += 10;
				drawrender();
				break;
			case SDL_KEYUP:
				SDL_Log("Scancode = %d, Sym = %d", event.key.keysym.scancode, event.key.keysym.sym);
			default:
				break;
			}
		}
	}

}


int main()
{
	//初始化
	if (init() < 0)
		return -1;

	drawrender();

	eventloop();

	deinit();

	return 0;
}
