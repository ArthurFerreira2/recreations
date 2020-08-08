// https://en.wikipedia.org/wiki/Cyclic_cellular_automaton
// https://softologyblog.wordpress.com/2013/08/29/cyclic-cellular-automata/

#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define FPS 60
#define SIZE 256

#define NB_STATES 3 // can't exceed 16 as the palette only has 16 colors
#define THRESOLD 3
#define MOORE SDL_TRUE
#define NEUMANN SDL_FALSE
#define NEIGHBORHOOD MOORE


// 16 colors palette from : http://alumni.media.mit.edu/~wad/color/palette.html
#define Black   (Uint32)0          // #000000
#define DkGray  (Uint32)5723991    // #575757
#define LtGray  (Uint32)10526880   // #A0A0A0
#define White   (Uint32)16777215   // #FFFFFF
#define LtGreen (Uint32)8504698    // #81C57A
#define Green   (Uint32)1927444    // #1D6914
#define Cyan    (Uint32)2740432    // #29D0D0
#define LtBlue  (Uint32)10334207   // #9DAFFF
#define Blue    (Uint32)2771927    // #2A4BD7
#define Purple  (Uint32)8464064    // #8126C0
#define Tan     (Uint32)15326907   // #E9DEBB
#define Orange  (Uint32)16749107   // #FF9233
#define Brown   (Uint32)8473113    // #814A19
#define Red     (Uint32)11346723   // #AD2323
#define Pink    (Uint32)16764403   // #FFCDF3
#define Yellow  (Uint32)16772659   // #FFEE33

Uint32 palette[16] = {
  Black, DkGray, LtGray, White, LtGreen, Green, Cyan,
  LtBlue, Blue, Purple, Tan, Orange, Brown, Red, Pink, Yellow
};

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

int main(int argc, char **argv) {
  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Demons of Cyclic Space",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);

  int count;
  Uint32 successorState;

  Uint32 table1[SIZE][SIZE];
  Uint32 table2[SIZE][SIZE];
  Uint32 (*cur)[SIZE] = table1;
  Uint32 (*new)[SIZE] = table2;
  Uint32 (*tmp)[SIZE] = NULL;

  for (int x=0; x<SIZE; x++)
    for (int y=0; y<SIZE; y++)
      cur[x][y] = rand()%NB_STATES;

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    for (int y=0; y<SIZE; y++)
      for (int x=0; x<SIZE; x++) {
        new[x][y] = cur[x][y];
        count = 0;
        successorState = (cur[x][y] + 1) % NB_STATES;
        if (cur[(x-1)%SIZE][y] == successorState) count++;
        if (cur[(x+1)%SIZE][y] == successorState) count++;
        if (cur[x][(y-1)%SIZE] == successorState) count++;
        if (cur[x][(y+1)%SIZE] == successorState) count++;
        if (NEIGHBORHOOD==MOORE) {
          if (cur[(x-1)%SIZE][(y-1)%SIZE] == successorState) count++;
          if (cur[(x-1)%SIZE][(y+1)%SIZE] == successorState) count++;
          if (cur[(x+1)%SIZE][(y-1)%SIZE] == successorState) count++;
          if (cur[(x+1)%SIZE][(y+1)%SIZE] == successorState) count++;
        }
        if (count>=THRESOLD) new[x][y] = successorState;
        setPixel(winSurf, x, y, palette[new[x][y]]);
      }
    tmp = cur;
    cur = new;
    new = tmp;

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
