//SDL 1.2 graphics backend
//Also handles other useful functions (such as the 10ms music timer)
#include <stdint.h>
#include "SDL/SDL.h"
SDL_Surface* hello=NULL;
SDL_Surface* screen=NULL;
void RhombusGFXInit(int scr_height, int scr_width);
void RhombusGFXQuit(void);
void RhombusDrawFrame(void);
void putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
uint32_t getpixel(SDL_Surface *surface, int x, int y);
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

void RhombusDrawFrame(void)
{
	//test
	SDL_LockSurface(screen);
	int i;
	for(i=0; i<240; i++) putpixel(screen, i, i, i);
	SDL_UnlockSurface(screen);
	SDL_Flip(screen);
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

void putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

uint32_t getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
