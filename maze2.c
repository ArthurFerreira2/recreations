// https://en.wikipedia.org/wiki/Maze_generation_algorithm

#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define CELL_SIZE 5
#define COLS 200
#define ROWS 150

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3


inline int index(short i, short j) {
  return (i<0 || i>=COLS || j<0 || j>=ROWS) ? -1 : i+j*COLS;
}


int main(int argc, char **argv) {

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("maze2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        CELL_SIZE*COLS+1, CELL_SIZE*ROWS+1, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_Event event;

  struct cell {
    short x, y;
    short i, j;
    SDL_bool visited;
    SDL_bool walls[4];
  } cells[COLS*ROWS];

  int neighbors[4], neighborsCount, idx, dist;

  for (short i=0, x=0; i<COLS; i++, x+=CELL_SIZE){
    for (short j=0, y=0; j<ROWS; j++, y+=CELL_SIZE) {
      idx = i+j*COLS;
      cells[idx].i = i;
      cells[idx].j = j;
      cells[idx].x = x;
      cells[idx].y = y;
      cells[idx].visited = SDL_FALSE;
      cells[idx].walls[UP   ] = SDL_TRUE;
      cells[idx].walls[LEFT ] = SDL_TRUE;
      cells[idx].walls[DOWN ] = SDL_TRUE;
      cells[idx].walls[RIGHT] = SDL_TRUE;
    }
  }

  int current = 0, next;
  cells[current].visited = SDL_TRUE;
  int stack[COLS*ROWS], sp = 0;
  stack[sp++] = current;

  while (sp) {
    current = stack[--sp];
    neighborsCount = 0;
    idx = index(cells[current].i, cells[current].j-1);
    if (idx>0 && !cells[idx].visited) neighbors[neighborsCount++] = idx;
    idx = index(cells[current].i-1, cells[current].j);
    if (idx>0 && !cells[idx].visited) neighbors[neighborsCount++] = idx;
    idx = index(cells[current].i, cells[current].j+1);
    if (idx>0 && !cells[idx].visited) neighbors[neighborsCount++] = idx;
    idx = index(cells[current].i+1, cells[current].j);
    if (idx>0 && !cells[idx].visited) neighbors[neighborsCount++] = idx;

    if (neighborsCount) {
      if (neighborsCount>1)
        stack[sp++] = current;
      next = neighbors[rand()%neighborsCount];
      cells[next].visited = SDL_TRUE;
      stack[sp++] = next;

      dist = cells[current].i - cells[next].i;
      if (dist==-1)
        cells[current].walls[LEFT] = cells[next].walls[RIGHT] = SDL_FALSE;
      else if (dist==1)
        cells[current].walls[RIGHT] = cells[next].walls[LEFT] = SDL_FALSE;
      else {
        dist = cells[current].j - cells[next].j;
        if (dist==1)
          cells[current].walls[UP] = cells[next].walls[DOWN] = SDL_FALSE;
        else if (dist==-1)
          cells[current].walls[DOWN] = cells[next].walls[UP] = SDL_FALSE;
        }
      current = next;
    }
  }

  for (idx=0; idx<COLS*ROWS; idx++) {
    if (cells[idx].walls[UP])
      SDL_RenderDrawLine(renderer, cells[idx].x, cells[idx].y, cells[idx].x + CELL_SIZE, cells[idx].y);
    if (cells[idx].walls[LEFT])
      SDL_RenderDrawLine(renderer, cells[idx].x + CELL_SIZE, cells[idx].y, cells[idx].x + CELL_SIZE, cells[idx].y + CELL_SIZE);
    if (cells[idx].walls[DOWN])
      SDL_RenderDrawLine(renderer, cells[idx].x, cells[idx].y + CELL_SIZE, cells[idx].x + CELL_SIZE, cells[idx].y + CELL_SIZE);
    if (cells[idx].walls[RIGHT])
      SDL_RenderDrawLine(renderer, cells[idx].x, cells[idx].y, cells[idx].x, cells[idx].y + CELL_SIZE);
  }

  SDL_RenderPresent(renderer);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(20);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
