#include <SDL2/SDL.h>

#define SIZE 256
#define dA 1.0
#define dB .5
#define feed .055
#define k .062

typedef struct cell_t {
  float A;
  float B;
} cell;


int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window  *window  = SDL_CreateWindow("Reaction Diffusion", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32      *pixels  = winSurf->pixels;
  SDL_bool   isRunning = SDL_TRUE;
  SDL_Event  event;

  int x, y;
  float a, b, laplaceA, laplaceB;
  int greyLevel;

  cell table1[SIZE][SIZE] = {0};
  cell table2[SIZE][SIZE] = {0};
  cell (*cur)[SIZE] = table1;
  cell (*new)[SIZE] = table2;
  cell (*tmp)[SIZE] = NULL;


  for (x=0; x<SIZE; x++) {
    for (y=0; y<SIZE; y++) {
      cur[x][y].A = 1;
      if (x>SIZE/2-20 && x<SIZE/2+20 && y>SIZE/2-20 && y<SIZE/2+20)
        cur[x][y].B = 1;
      else
        cur[x][y].B = 0;
    }
  }

  while(isRunning) {
    for (x=1; x<SIZE-1; x++)
      for (y=1; y<SIZE-1; y++) {

        a = cur[x][y].A;
        b = cur[x][y].B;

        laplaceA =  cur[x-1][y-1].A * .05 + cur[x-1][y  ].A * 0.2 + cur[x-1][y+1].A * .05 +
                    cur[x  ][y-1].A * 0.2 + cur[x  ][y  ].A *  -1 + cur[x  ][y+1].A * 0.2 +
                    cur[x+1][y-1].A * .05 + cur[x+1][y  ].A * 0.2 + cur[x+1][y+1].A * .05;

        laplaceB =  cur[x-1][y-1].B * .05 + cur[x-1][y  ].B * 0.2 + cur[x-1][y+1].B * .05 +
                    cur[x  ][y-1].B * 0.2 + cur[x  ][y  ].B *  -1 + cur[x  ][y+1].B * 0.2 +
                    cur[x+1][y-1].B * .05 + cur[x+1][y  ].B * 0.2 + cur[x+1][y+1].B * .05;

        new[x][y].A = a +
                      (dA * laplaceA) -
                      (a * b * b) +
                      (feed * (1 - a));

        new[x][y].B = b +
                      (dB * laplaceB) +
                      (a * b * b) -
                      ((k + feed) * b);

        greyLevel=(int)(new[x][y].B * 1000);
        pixels[x + y * SIZE] = SDL_MapRGBA(winSurf->format, greyLevel, greyLevel, greyLevel, SDL_ALPHA_OPAQUE);
      }

    tmp = cur;
    cur = new;
    new = tmp;

    SDL_UpdateWindowSurface(window);
    while(SDL_PollEvent(&event)) if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
