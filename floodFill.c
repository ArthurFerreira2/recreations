// https://fr.wikipedia.org/wiki/Algorithme_de_remplissage_par_diffusion

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define FPS 60
#define WIDTH 256
#define HEIGHT 256

#define Black  (Uint32)0
#define Blue   (Uint32)2771927
#define Green  (Uint32)1927444
#define Red    (Uint32)11346723
#define Yellow (Uint32)16772659
#define White  (Uint32)16777215


Uint32 getPixel(SDL_Surface *surface, int x, int y) {
  if (x<0 || y <0 || x>=surface->w || y>=surface->h) return 0;

  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  if (bpp==4) return *(Uint32 *)p;
  if (bpp==1) return *p;
  if (bpp==2) return *(Uint16 *)p;
  if (bpp==3) {
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  }
  return 0;
}


void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
  if (x<0 || y <0 || x>=surface->w || y>=surface->h) return;

  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  if (bpp==4) { *(Uint32 *)p = pixel; return; }
  if (bpp==1) { *p = pixel; return; }
  if (bpp==2) { *(Uint16 *)p = pixel; return; }
  if (bpp==3 && SDL_BYTEORDER == SDL_BIG_ENDIAN) {
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


void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2,  Uint32 color) {

  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int er = (dx > dy ? dx : -dy) / 2;
  int e2;

  for(;;) {
    setPixel(surface, x1, y1, color);
    if (x1 == x2 && y1 == y2)
      break;
    e2 = er;
    if (e2 > -dx) {
      er -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      er += dx;
      y1 += sy;
    }
  }
}


// this recursive implementation will fail with large zones -> stack overflow
void floodFill0(SDL_Surface *surface, int x, int y, Uint32 borderColor, Uint32 fillColor) {
  if (x<0 || y <0 || x>=surface->w || y>=surface->h) return;

  Uint32 actualColor = getPixel(surface, x, y);
  if (actualColor!=borderColor && actualColor!=fillColor) {
    setPixel(surface, x, y, fillColor);
    floodFill0(surface, x-1, y, borderColor, fillColor);
    floodFill0(surface, x+1, y, borderColor, fillColor);
    floodFill0(surface, x, y-1, borderColor, fillColor);
    floodFill0(surface, x, y+1, borderColor, fillColor);
  }
  return;
}


void floodFill(SDL_Surface *surface, int x, int y, Uint32 fillColor) {

  Uint32 actualColor = getPixel(surface, x, y);
  if (actualColor == fillColor) return;

  struct pile {
    int sp;
    int *x, *y;
  } p;

  p.x = (int*)malloc(WIDTH*HEIGHT*sizeof(int));
  p.y = (int*)malloc(WIDTH*HEIGHT*sizeof(int));
  p.x[0] = x;
  p.y[0] = y;
  p.sp = 1;

  int xt, yt, xw, xe;

  while(p.sp) {
    p.sp--;
    xt = p.x[p.sp];
    yt = p.y[p.sp];

    if (getPixel(surface, xt, yt) == actualColor) {
      xw=xe=xt;
      while(xe+1 < WIDTH && getPixel(surface, xe+1, yt) == actualColor) xe++;
      while(xw-1 >= 0    && getPixel(surface, xw-1, yt) == actualColor) xw--;

      for (int i=xw ; i<=xe; i++) {
        setPixel(surface, i, yt, fillColor);
        if (yt+1 <HEIGHT && getPixel(surface, i, yt+1) == actualColor) {
          p.x[p.sp] = i;
          p.y[p.sp] = yt + 1;
          p.sp++;
        }
        if (yt-1 >= 0    && getPixel(surface, i, yt-1) == actualColor) {
          p.x[p.sp] = i;
          p.y[p.sp] = yt - 1;
          p.sp++;
        }
      }
    }
  }
}



int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  SDL_bool CLICKED = SDL_FALSE;
  int mouseX=0, mouseY=0, lastPointX=0, lastPointY=0;

  uint32_t palette[5] = {Black, Red, Green, Blue, Yellow};

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Flood Fill", SDL_WINDOWPOS_CENTERED,
                      SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  SDL_FillRect(winSurf, NULL, White);

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event)) {
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) {
        isRunning = SDL_FALSE;
      }
      if (event.type==SDL_MOUSEMOTION && CLICKED) {
        SDL_GetMouseState(&mouseX, &mouseY);
        drawLine(winSurf,lastPointX, lastPointY, mouseX, mouseY, Black);
        lastPointX = mouseX;
        lastPointY = mouseY;
      }
      if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT) {
        CLICKED = SDL_TRUE;
        SDL_GetMouseState(&lastPointX, &lastPointY);
      }
      if (event.type==SDL_MOUSEBUTTONUP && event.button.button==SDL_BUTTON_LEFT) {
        CLICKED = SDL_FALSE;
      }
      if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_RIGHT) {
        CLICKED = SDL_FALSE;
        SDL_GetMouseState(&lastPointX, &lastPointY);
        floodFill(winSurf, lastPointX, lastPointY, palette[rand()%5]);
      }
      if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_MIDDLE) {
        CLICKED = SDL_FALSE;
        SDL_FillRect(winSurf, NULL, White);
      }
    }

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
