#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 3000


void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
  if (x<0 || y <0 || x>=surface->w || y>=surface->h) return;

  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  if (bpp==4) { *(Uint32 *)p = pixel; return; }
  if (bpp==1) { *p = pixel; return; }
  if (bpp==2) { *(Uint16 *)p = pixel; return; }
  if (bpp==3) {
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    return;
  }
  return;
}


Uint32 getPixel(SDL_Surface *surface, int x, int y){

  if (x<0 || y <0 || x>=surface->w || y>=surface->h) return 0;

  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  if (bpp==1) return *p;
  if (bpp==2) return *(Uint16 *)p;
  if (bpp==3) {
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  }
  if (bpp==4) return *(Uint32 *)p;
  return 0;
}


double distance(double x1, double y1, double x2, double y2) {
  double dist = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
  return dist;
}


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  int x=SIZE/2, y=SIZE/2;
  Uint32 color;
  Uint8  R=0, G=0, B=0, A=255;

  const double Rx = 0;
  const double Ry = 0;
  const double Gx = 0;
  const double Gy = SIZE-1;
  const double Bx = SIZE-1;
  const double By = SIZE-1;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Colors", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  for (double i=0; i<SIZE; i++) {
    for (double j=0; j<SIZE; j++) {
      R = 255 - distance(i, j, Rx, Ry);
      G = 255 - distance(i, j, Gx, Gy);
      B = 255 - distance(i, j, Bx, By);
      color = SDL_MapRGBA(surface->format,  R, G, B, A);
      putPixel(surface, (int)i, (int)j, color);
    }
  }

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    color = getPixel(surface, x, y);
    SDL_GetRGBA(color, surface->format, &R, &G, &B, &A);
    R=(R+(rand()%3)-1);
    G=(G+(rand()%3)-1);
    B=(B+(rand()%3)-1);
    color = SDL_MapRGBA(surface->format, R, G, B, A);
    putPixel(surface, x, y, color);
    x=(SIZE+x+(rand()%3)-1)%SIZE;
    y=(SIZE+y+(rand()%3)-1)%SIZE;

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
