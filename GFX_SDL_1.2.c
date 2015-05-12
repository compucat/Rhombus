//SDL 1.2 graphics backend

#include "SDL/SDL.h"
SDL_Surface* hello=NULL;
SDL_Surface* screen=NULL;
void RhombusGFXInit(int scr_height, int scr_width)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("Rhombus: the Parallelogram by LFT FPGA Demo Emulator", "Rhombus");
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
	freopen( "CON", "w", stdout ); //Redirect stdout back to console
	freopen( "CON", "w", stderr );
	screen=SDL_SetVideoMode(scr_height, scr_width, 32, SDL_SWSURFACE);
	hello=SDL_LoadBMP("hello.bmp");
	SDL_BlitSurface(hello, NULL, screen, NULL);
	SDL_Flip(screen);
}

void RhombusGFXQuit(void)
{
	SDL_FreeSurface(hello);
	SDL_Quit();
}

//Defined by SDL: 
//void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
//Uint32 getpixel(SDL_Surface *surface, int x, int y)
//Before use, lock surface using SDL_LockSurface(surface)
//Unlock using SDL_UnlockSurface
/*
SDL's coordinates are like....

+----------------------->
|0.......................x
|.
|.
|.
|.
|.
|.
|.
|.
|.
|y
\/
*/
