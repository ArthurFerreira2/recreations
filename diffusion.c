#include <SDL2/SDL.h>

#define SIZE 256
#define dA 1.0
#define dB .5
#define feed .055
#define k .062

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Reaction Diffusion", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32 *pixels = winSurf->pixels;
  SDL_Event event;

  int x, y, gL;
  float a, b, lA, lB;

  typedef struct cell_t {
    float A;
    float B;
  } cell;

  cell table1[SIZE][SIZE] = {0};
  cell table2[SIZE][SIZE] = {0};
  cell (*cur)[SIZE] = table1;
  cell (*new)[SIZE] = table2;
  cell (*tmp)[SIZE] = NULL;

  for (x=0; x<SIZE; x++)
    for (y=0; y<SIZE; y++) {
      cur[x][y].A = 1;
      if (x>SIZE/2-10 && x<SIZE/2+10 && y>SIZE/2-10 && y<SIZE/2+10)
        cur[x][y].B = 1;
    }

  while(SDL_TRUE) {
    for (x=1; x<SIZE-1; x++)
      for (y=1; y<SIZE-1; y++) {

        a = cur[x][y].A;
        b = cur[x][y].B;
        // Laplace transforms
        lA = cur[x-1][y-1].A * .05 + cur[x-1][y].A * 0.2 + cur[x-1][y+1].A * .05 +
             cur[x  ][y-1].A * 0.2 + cur[x  ][y].A *  -1 + cur[x  ][y+1].A * 0.2 +
             cur[x+1][y-1].A * .05 + cur[x+1][y].A * 0.2 + cur[x+1][y+1].A * .05;

        lB = cur[x-1][y-1].B * .05 + cur[x-1][y].B * 0.2 + cur[x-1][y+1].B * .05 +
             cur[x  ][y-1].B * 0.2 + cur[x  ][y].B *  -1 + cur[x  ][y+1].B * 0.2 +
             cur[x+1][y-1].B * .05 + cur[x+1][y].B * 0.2 + cur[x+1][y+1].B * .05;

        new[x][y].A = a + (dA * lA) - (a * b * b) + (feed * (1 - a));
        new[x][y].B = b + (dB * lB) + (a * b * b) - ((k + feed) * b);

        gL = (int)(new[x][y].B * 255);
        pixels[x + y * SIZE] = SDL_MapRGBA(winSurf->format, gL, gL, gL, SDL_ALPHA_OPAQUE);
      }

    tmp = cur;
    cur = new;
    new = tmp;

    SDL_UpdateWindowSurface(window);
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(20);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
