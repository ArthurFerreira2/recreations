#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>


#define SIZE  256
#define GROS  1
#define NORTH 1
#define SOUTH 2
#define EAST  3
#define WEST  4

#define reproAge 10
#define maxStarvation 8


int main(int argc, char **argv) {

  int x, y, i, j;
  int age, starvation;
  int nbFish = 0;
  int nbShark = 0;
  int directions[4] = {0};
  int nbDirections = 0;
  int direction, neig, xe, xw, yn, ys;

  typedef enum {
    none,
    fish,
    shark
  } species;

  struct animal {
    species sort;
    int age;
    int starvation;
  } tor[SIZE][SIZE] = {0};

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Wator", SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED, SIZE*GROS,
                                         SIZE*GROS, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  for (neig=0; neig<SIZE*SIZE/10; neig++) {
    x = rand() % SIZE;
    y = rand() % SIZE;
    tor[x][y].sort = rand() % 2 ? fish : shark;
    tor[x][y].starvation = rand() % maxStarvation;
    tor[x][y].age = rand() % reproAge;
  }

  while(isRunning) {
    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    for (x=0; x<SIZE; x++) {
      for (y=0; y<SIZE; y++) {

        xw = (x==0     ) ? SIZE-1 : x-1;
        yn = (y==0     ) ? SIZE-1 : y-1;
        xe = (x==SIZE-1) ? 0      : x+1;
        ys = (y==SIZE-1) ? 0      : y+1;

        if (tor[x][y].sort == fish) {
          nbDirections = 0;
          if(tor[xe][y].sort == none) directions[nbDirections++]=EAST;
          if(tor[xw][y].sort == none) directions[nbDirections++]=WEST;
          if(tor[x][yn].sort == none) directions[nbDirections++]=NORTH;
          if(tor[x][ys].sort == none) directions[nbDirections++]=SOUTH;
          tor[x][y].age++;
          age = tor[x][y].age;

          if (nbDirections>0){
            if (age > reproAge) {
              age = 0;
              tor[x][y].age=0;
            }
            else
              tor[x][y].sort = none;

            direction = rand()%nbDirections;
            i=x;
            j=y;
            if (directions[direction] == EAST)  i=xe;
            if (directions[direction] == WEST)  i=xw;
            if (directions[direction] == NORTH) j=yn;
            if (directions[direction] == SOUTH) j=ys;
            tor[i][j].age = age;
            tor[i][j].sort = fish;
          }
        }

        if (tor[x][y].sort == shark) {
          tor[x][y].starvation++;
          if (tor[x][y].starvation > maxStarvation)
            tor[x][y].sort = none;
          else {
            nbDirections = 0;
            if(tor[xe][y].sort == fish) directions[nbDirections++]=EAST;
            if(tor[xw][y].sort == fish) directions[nbDirections++]=WEST;
            if(tor[x][yn].sort == fish) directions[nbDirections++]=NORTH;
            if(tor[x][ys].sort == fish) directions[nbDirections++]=SOUTH;

            if (nbDirections > 0)
              starvation = 0;
            else {
              starvation = tor[x][y].starvation;
              if(tor[xe][y].sort == none) directions[nbDirections++]=EAST;
              if(tor[xw][y].sort == none) directions[nbDirections++]=WEST;
              if(tor[x][yn].sort == none) directions[nbDirections++]=NORTH;
              if(tor[x][ys].sort == none) directions[nbDirections++]=SOUTH;
            }

            tor[x][y].age++;
            age = tor[x][y].age;

            if (nbDirections > 0) {
              if (age > reproAge) {
                age = 0;
                tor[x][y].age = 0;
                tor[x][y].starvation = 0;
              }
              else
                tor[x][y].sort = none;

              direction = rand()%nbDirections;
              i=x;
              j=y;
              if (directions[direction] == EAST)  i=xe;
              if (directions[direction] == WEST)  i=xw;
              if (directions[direction] == NORTH) j=yn;
              if (directions[direction] == SOUTH) j=ys;
              tor[i][j].age = age;
              tor[i][j].starvation = starvation;
              tor[i][j].sort = shark;
            }
          }
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 55, 55, 55 , SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, 0, 0, SIZE*GROS, 0);
    nbFish = 0;
    nbShark = 0;
    for (x=0; x<SIZE; x++) {
      for (y=0; y<SIZE; y++) {
        if (tor[x][y].sort == fish) {
          SDL_SetRenderDrawColor(renderer, 0, 100, 255, SDL_ALPHA_OPAQUE);
          nbFish++;
        }
        else if (tor[x][y].sort == shark) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
          nbShark++;
        }
        else
          SDL_SetRenderDrawColor(renderer, 55, 55, 55 , SDL_ALPHA_OPAQUE);

        SDL_RenderDrawPoint(renderer, x*GROS, y*GROS);
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 100, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, 0, 0, nbFish*GROS/SIZE, 0);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, 0, 0, nbShark*GROS/SIZE, 0);

    SDL_Delay(100);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
