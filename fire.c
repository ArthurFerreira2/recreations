// https://lodev.org/cgtutor/fire.html

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 60


typedef struct rgbColor_t {
    int r;
    int g;
    int b;
} rgbColor;


typedef struct hsvColor_t {
    int h;
    int s;
    int v;
} hsvColor;


void rgb2hsv(rgbColor rgb, hsvColor* hsv) {
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r<rgb.g ? (rgb.r<rgb.b?rgb.r:rgb.b) : (rgb.g<rgb.b?rgb.g:rgb.b);
    rgbMax = rgb.r>rgb.g ? (rgb.r>rgb.b?rgb.r:rgb.b) : (rgb.g>rgb.b?rgb.g:rgb.b);

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


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("fire", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  Uint32 fire[SIZE][SIZE] = {0};
  hsvColor colorHSV = {0, 255, 0};
  rgbColor colorRGB;
  Uint8 R[256], G[256], B[256];

  for (int i = 0; i < 256; i++) {
    colorHSV.h = i/3;
    colorHSV.v = i<128 ? i*2 : 255;
    hsv2rgb(colorHSV, &colorRGB );
    R[i] = colorRGB.r;
    G[i] = colorRGB.g;
    B[i] = colorRGB.b;
  }


  while(isRunning) {

    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    for(int x = 0; x < SIZE; x++)
      fire[SIZE - 1][x] = rand()%256;

    for(int y = 0; y < SIZE-1; y++)
      for(int x = 0; x < SIZE; x++) {
        fire[y][x] = ((fire[(y + 1) % SIZE][(x - 1 + SIZE) % SIZE]
                     + fire[(y + 2) % SIZE][(x) % SIZE]
                     + fire[(y + 1) % SIZE][(x + 1) % SIZE]
                     + fire[(y + 3) % SIZE][(x) % SIZE]) * 64) / 257;

        SDL_SetRenderDrawColor(renderer, R[fire[y][x]], G[fire[y][x]], B[fire[y][x]], 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
