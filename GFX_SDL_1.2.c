//SDL 1.2 graphics backend

#include "SDL/SDL.h"
SDL_Surface* hello=NULL;
SDL_Surface* screen=NULL;
void RhombusGFXInit(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	freopen( "CON", "w", stdout ); //Redirect stdout back to console
	freopen( "CON", "w", stderr );
	screen=SDL_SetVideoMode(1024, 768, 32, SDL_SWSURFACE);
	hello=SDL_LoadBMP("hello.bmp");
	SDL_BlitSurface(hello, NULL, screen, NULL);
	SDL_Flip(screen);
}

void RhombusGFXQuit(void)
{
	SDL_FreeSurface(hello);
	SDL_Quit();
}
