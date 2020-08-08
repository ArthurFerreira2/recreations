// https://en.wikipedia.org/wiki/Maze_generation_algorithm

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define FPS 60

#define CELL_SIZE 16
#define COLS 16
#define ROWS 16

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3


inline int index(int i, int j) {
  return (i<0 || i>=COLS || j<0 || j>=ROWS) ? -1 : i+j*COLS;
}


int main(int argc, char **argv) {

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CELL_SIZE*COLS+1, CELL_SIZE*ROWS+1, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
  SDL_bool isRunning = SDL_TRUE, paused = SDL_TRUE;
  SDL_Event event;
  SDL_Rect rect = {0, 0, CELL_SIZE - 2, CELL_SIZE - 2};

  struct cell {
    int x, y;
    int i, j;
    SDL_bool visited;
    SDL_bool walls[4];
  } cells[COLS*ROWS];

  int stack[ROWS*COLS], sp;
  int current, next;
  int neighbors[4], neighborsCount, idx;

  while(isRunning) {

    SDL_SetRenderDrawColor(renderer,   0, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (int x=0; x<=COLS*CELL_SIZE; x+=CELL_SIZE)
    SDL_RenderDrawLine(renderer, x, 0, x, ROWS*CELL_SIZE);

    for (int y=0; y<=ROWS*CELL_SIZE; y+=CELL_SIZE)
    SDL_RenderDrawLine(renderer, 0, y, COLS*CELL_SIZE, y);

    for (int j=0; j<ROWS; j++) {
      for (int i=0; i<COLS; i++){
        idx = i+j*COLS;
        cells[idx].i = i;
        cells[idx].j = j;
        cells[idx].x = i * CELL_SIZE;
        cells[idx].y = j * CELL_SIZE;
        cells[idx].visited      = SDL_FALSE;
        cells[idx].walls[UP   ] = SDL_TRUE;
        cells[idx].walls[LEFT ] = SDL_TRUE;
        cells[idx].walls[DOWN ] = SDL_TRUE;
        cells[idx].walls[RIGHT] = SDL_TRUE;
      }
    }

    sp = current = 0;
    cells[current].visited = SDL_TRUE;
    stack[sp++] = current;

    while (sp) {

      current = stack[--sp];

      rect.x = cells[current].x + 1;
      rect.y = cells[current].y + 1;
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(renderer, &rect);
      SDL_RenderPresent(renderer);
      SDL_Delay(20);

      neighborsCount = 0;

      idx = index(cells[current].i, cells[current].j - 1);
      if (idx != -1 && !cells[idx].visited) neighbors[neighborsCount++] = idx;

      idx = index(cells[current].i - 1, cells[current].j);
      if (idx != -1 && !cells[idx].visited) neighbors[neighborsCount++] = idx;

      idx = index(cells[current].i, cells[current].j + 1);
      if (idx != -1 && !cells[idx].visited) neighbors[neighborsCount++] = idx;

      idx = index(cells[current].i + 1, cells[current].j);
      if (idx != -1 && !cells[idx].visited) neighbors[neighborsCount++] = idx;

      if (neighborsCount) {
        if (neighborsCount > 1) stack[sp++] = current;
        next = neighbors[rand()%neighborsCount];
        stack[sp++] = next;
        cells[next].visited = SDL_TRUE;

        SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);

        if (cells[current].i - cells[next].i == 1) {
          cells[current].walls[RIGHT] = cells[next].walls[LEFT] = SDL_FALSE;
          SDL_RenderDrawLine(renderer, cells[next].x+CELL_SIZE, cells[next].y+1, cells[next].x+CELL_SIZE, cells[next].y+CELL_SIZE-1);
        }
        else if (cells[current].i - cells[next].i == -1) {
          cells[current].walls[LEFT] = cells[next].walls[RIGHT] = SDL_FALSE;
          SDL_RenderDrawLine(renderer, cells[next].x, cells[next].y+1, cells[next].x, cells[next].y + CELL_SIZE-1);
        }
        else if (cells[current].j - cells[next].j == 1) {
          cells[current].walls[UP] = cells[next].walls[DOWN] = SDL_FALSE;
          SDL_RenderDrawLine(renderer, cells[next].x+1, cells[next].y+CELL_SIZE, cells[next].x+CELL_SIZE-1, cells[next].y+CELL_SIZE);
        }
        else if (cells[current].j - cells[next].j == -1) {
          cells[current].walls[DOWN] = cells[next].walls[UP] = SDL_FALSE;
          SDL_RenderDrawLine(renderer, cells[next].x+1, cells[next].y, cells[next].x+CELL_SIZE-1, cells[next].y);
        }
        current = next;
      }

      SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(renderer, &rect);

      while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT) {
          sp = 0;
          isRunning = SDL_FALSE;
        }
    }

    paused = SDL_TRUE;
    while (isRunning && paused) {
      while (SDL_PollEvent(&event)) {
        if (event.type==SDL_QUIT) isRunning = paused = SDL_FALSE;
        if (event.type==SDL_KEYDOWN) paused = SDL_FALSE;
      }
      SDL_Delay(100);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
