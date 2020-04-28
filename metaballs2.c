#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>


#define SIZE 256
#define FPS 20

#define NB_CIRCLES 5
#define RADIUS_MIN 30
#define RADIUS_MAX 60
#define CIRCLE_SPEED 4
#define ALPHA 255


typedef struct circle_t {
  int x, y, r, dx, dy;
} circle;

circle circles[NB_CIRCLES];


typedef struct rgbColor_t {
    Uint8 r, g, b;
} rgbColor;


typedef struct hsvColor_t {
    Uint8 h, s, v;
} hsvColor;


void rgb2hsv(rgbColor rgb, hsvColor* hsv) {
    unsigned char rgbMin, rgbMax;

    rgbMin=rgb.r<rgb.g?(rgb.r<rgb.b?rgb.r:rgb.b):(rgb.g<rgb.b?rgb.g:rgb.b);
    rgbMax=rgb.r>rgb.g?(rgb.r>rgb.b?rgb.r:rgb.b):(rgb.g>rgb.b?rgb.g:rgb.b);

    hsv->v = rgbMax;
    if (hsv->v == 0) {
        hsv->h = 0;
        hsv->s = 0;
        return;
    }

    hsv->s = 255 * (long)(rgbMax - rgbMin) / hsv->v;
    if (hsv->s == 0) {
        hsv->h = 0;
        return;
    }

    if (rgbMax == rgb.r)
        hsv->h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv->h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv->h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);
    return;
}


void hsv2rgb(hsvColor hsv, rgbColor* rgb) {
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb->r = hsv.v;
        rgb->g = hsv.v;
        rgb->b = hsv.v;
        return;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            rgb->r = hsv.v; rgb->g = t; rgb->b = p;
            break;
        case 1:
            rgb->r = q; rgb->g = hsv.v; rgb->b = p;
            break;
        case 2:
            rgb->r = p; rgb->g = hsv.v; rgb->b = t;
            break;
        case 3:
            rgb->r = p; rgb->g = q; rgb->b = hsv.v;
            break;
        case 4:
            rgb->r = t; rgb->g = p; rgb->b = hsv.v;
            break;
        default:
            rgb->r = hsv.v; rgb->g = p; rgb->b = q;
            break;
    }
    return;
}


void drawCircle(SDL_Renderer* renderer, circle c) {
  int PX, PY, DX = 1, DY = (int)c.r;
  int delta = (int)c.r - 1, ep;

  while(DY >= 0) {
    ep = delta + delta;

    PX = c.x + DX;
    PY = c.y + DY;
    SDL_RenderDrawPoint(renderer, PX , PY);
    PX = c.x - DX;
    SDL_RenderDrawPoint(renderer, PX , PY);
    PY = c.y - DY;
    SDL_RenderDrawPoint(renderer, PX , PY);
    PX = c.x + DX;
    SDL_RenderDrawPoint(renderer, PX , PY);

    if (ep < 0) {
      ep += DX;
      if (ep < 0) delta += --DY;
      else  delta += (--DY) - (++DX);
    }
    else {
      ep -= DY;
      if (ep >= 0) delta -= ++DX;
      else delta += (--DY) - (++DX);
    }
  }
}


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;

  int dist, greyLevel;
  hsvColor hsv = {0, 200, 255};
  rgbColor rgb;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Metaballs", SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
  SDL_RenderClear(renderer);

  for (int i=0; i<NB_CIRCLES; i++) {
    circles[i].x  = rand()%SIZE;
    circles[i].y  = rand()%SIZE;
    circles[i].r  = rand()%(RADIUS_MAX-RADIUS_MIN) + RADIUS_MIN;
    circles[i].dx = rand()%(CIRCLE_SPEED) + 1;
    circles[i].dy = rand()%(CIRCLE_SPEED) + 1;
  }

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    for (int i=0; i<NB_CIRCLES; i++) {
      circles[i].x += circles[i].dx;
      circles[i].y += circles[i].dy;
      if (circles[i].x < 0 || circles[i].x > SIZE) circles[i].dx *= -1;
      if (circles[i].y < 0 || circles[i].y > SIZE) circles[i].dy *= -1;
    }

    for(int x=0; x<SIZE; x++) {
      for(int y=0; y<SIZE; y++) {
        greyLevel = 0;
        for(int i=0; i<NB_CIRCLES; i++) {
          dist = sqrt((pow(x - circles[i].x, 2) + pow( y - circles[i].y, 2)));
          if (dist) greyLevel += (circles[i].r * RADIUS_MAX) / dist;
          else greyLevel = 255;
        }
        if (greyLevel > 255) greyLevel = 255;
        hsv.h = greyLevel;
        hsv2rgb(hsv, &rgb);
        SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, ALPHA);
        SDL_RenderDrawPoint(renderer, x , y );
      }
    }

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, ALPHA);
    // for (int i=0; i<NB_CIRCLES; i++)
    //   drawCircle(renderer, circles[i]);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
      SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
