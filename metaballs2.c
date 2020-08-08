#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 30

#define NB_CIRCLES 5
#define RADIUS_MIN 50
#define RADIUS_MAX 80
#define CIRCLE_SPEED 5

typedef struct rgbColor_t {
  Uint8 r, g, b;
} rgbColor;


typedef struct hsvColor_t {
  Uint8 h, s, v;
} hsvColor;


void hsv2rgb(hsvColor hsv, rgbColor* rgb) {
  int region, remainder, p, q, t;

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

// Fast Approximate Distance Function
// https://www.flipcode.com/archives/Fast_Approximate_Distance_Functions.shtml
int approxDistance(int x1, int y1, int x2, int y2) {
  int dx=x1-x2, dy=y1-y2;

  if ( dx < 0 ) dx = -dx;
  if ( dy < 0 ) dy = -dy;

  // coefficients equivalent to ( 123/128 * max ) and ( 51/128 * min )
  if ( dx < dy )
    return ((( dy << 8 ) + ( dy << 3 ) - ( dy << 4 ) - ( dy << 1 ) +
             ( dx << 7 ) - ( dx << 5 ) + ( dx << 3 ) - ( dx << 1 )) >> 8 );
  else
    return ((( dx << 8 ) + ( dx << 3 ) - ( dx << 4 ) - ( dx << 1 ) +
             ( dy << 7 ) - ( dy << 5 ) + ( dy << 3 ) - ( dy << 1 )) >> 8 );
}


// Integer square root
// https://en.wikipedia.org/wiki/Integer_square_root
int integerSqrt(int n) {
  int shift = 2;
  // Find greatest shift.
  int nShifted = n >> shift;
  // We check for nShifted being n, since some implementations
  // perform shift operations modulo the word size.
  while (nShifted != 0 && nShifted != n) {
    shift = shift + 2;
    nShifted = n >> shift;
  }
  shift = shift - 2;
  // Find digits of result.
  int result = 0;
  while (shift >= 0) {
    result = result << 1;
    int candidateResult = result + 1;
    if (candidateResult*candidateResult <= (n >> shift))
      result = candidateResult;
    shift = shift - 2;
  }
  return result;
}

int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Metaballs2", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  struct circle {
    int x, y, r, dx, dy;
  } circles[NB_CIRCLES];

  for (int i=0; i<NB_CIRCLES; i++) {
    circles[i].x  = rand()%SIZE;
    circles[i].y  = rand()%SIZE;
    circles[i].r  = rand()%(RADIUS_MAX-RADIUS_MIN) + RADIUS_MIN;
    circles[i].dx = rand()%(CIRCLE_SPEED) + 1;
    circles[i].dy = rand()%(CIRCLE_SPEED) + 1;
  }

  hsvColor hsv = {0, 200, 255};
  rgbColor rgb;

  while (isRunning) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    for (int i=0; i<NB_CIRCLES; i++) {
      circles[i].x += circles[i].dx;
      if (circles[i].x < 0 || circles[i].x >= SIZE) circles[i].dx *= -1;
      circles[i].y += circles[i].dy;
      if (circles[i].y < 0 || circles[i].y >= SIZE) circles[i].dy *= -1;
    }

    for (int x=0; x<SIZE; x+=1) {
      for (int y=0; y<SIZE; y+=1) {
        int greyLevel = 0;
        for (int i=0; i<NB_CIRCLES; i++) {
          // int dist = sqrt((pow(x - circles[i].x, 2) + pow(y - circles[i].y, 2)));
          // int dist = approxDistance(x, y, circles[i].x, circles[i].y);
          int dist = integerSqrt((x - circles[i].x)*(x - circles[i].x) + (y - circles[i].y)*(y - circles[i].y));
          if (dist) greyLevel += (circles[i].r * RADIUS_MAX) / dist;
          else greyLevel = 255;
          if (greyLevel >= 255){
            greyLevel = 255;
            break;
          }
        }
        hsv.h = greyLevel/2 + 42;
        hsv2rgb(hsv, &rgb);
        SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer, x , y );
      }
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
