/*
	SDL 1.2 Graphics Backend
	Also handles other useful functions (such as the 10ms music timer
	
	This file is part of Rhombus: the emulator for LFT's FPGA demo "Parallelogram".
    Copyright (C) 2015 sirwinstoncat5 (GitHub username)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include "SDL/SDL.h"
extern uint16_t outPort[8];
SDL_Surface* qvga_buffer=NULL;
SDL_Surface* screen=NULL;
void RhombusGFXInit(int scr_height, int scr_width);
void RhombusGFXQuit(void);
void RhombusDrawFrame(void);
void putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
uint32_t getpixel(SDL_Surface *surface, int x, int y);
void scaleBlit(SDL_Surface* src, SDL_Surface* dst);
void RhombusGFXInit(int scr_width, int scr_height)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("Rhombus: the Parallelogram by LFT FPGA Demo Emulator", "Rhombus");
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
	freopen( "CON", "w", stdout ); //Redirect stdout back to console
	freopen( "CON", "w", stderr );
	screen=SDL_SetVideoMode(scr_width, scr_height, 32, SDL_SWSURFACE);
	SDL_Surface* splash=SDL_LoadBMP("hello.bmp");
	scaleBlit(splash, screen);
	SDL_Flip(screen);
	SDL_FreeSurface(splash);
	qvga_buffer=SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 32, 0, 0, 0, 0);
}

void RhombusGFXQuit(void)
{
	SDL_FreeSurface(qvga_buffer);
	SDL_Quit();
}

void RhombusDrawFrame(void)
{
	SDL_Event evt;
	SDL_PollEvent(&evt);
	if(evt.type==SDL_QUIT) exit(0);
	
	SDL_LockSurface(qvga_buffer);
	int addr;
	addr=131072*outPort[6];
	int i, j;
	//Begin test routine
	for(i=0; i<320; i++) for(j=0; j<240; j++) putpixel(qvga_buffer, i, j, rand()*rand()); //Draw a test screen
	//End test routine
	//Insert actual framebuffer drawing here
	SDL_UnlockSurface(qvga_buffer);
	scaleBlit(qvga_buffer, screen);
	SDL_Flip(screen);
}

void scaleBlit(SDL_Surface* src, SDL_Surface* dst) //Should have used SDL 2, probably a huge bottleneck, oh well.
{
	SDL_LockSurface(src);
	SDL_LockSurface(dst);
	float i, j;
	float scalex=(float) (src->w)/(float) (dst->w);
	float scaley=(float) (src->h)/(float) (dst->h);
	//printf("scale: %f, %f\n", scalex, scaley);
	//getchar();
	for(i=0; i<dst->w; i++)
	{
		for(j=0; j<dst->h; j++)
		{
			//printf("xy to xy: %d, %d, %d, %d\n", (int) (i*scalex), (int) (j*scaley), (int) i, (int) j);
			putpixel(dst, i, j, getpixel(src, (int) (i*scalex), (int) (j*scaley)));
		}
	}
	SDL_UnlockSurface(src);
	SDL_UnlockSurface(dst);
}
//Copied from SDL spec:
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
