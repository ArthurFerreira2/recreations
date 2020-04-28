// https://www.youtube.com/watch?v=E1B4UoSQMFw&list=PLRqwX-V7Uu6ZiZxtDDRCi6uhfTH4FilpH&index=19

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.1415926535
#define SIZE 256
#define MAX 1000000


float len = 70;
float stack[1000];
int sp = 0;
float x = 0;
float y = 0;
float angle = 0;


void push() {
  if (sp >= 998) {
    printf("stack overflow\n");
    exit(EXIT_FAILURE);
  }
  stack[sp++] = x;
  stack[sp++] = y;
  stack[sp++] = angle;
}


void pop() {
  if (sp < 2) {
    printf("stack underflow\n");
    exit(EXIT_FAILURE);
  }
  angle = stack[--sp];
  y = stack[--sp];
  x = stack[--sp];
}


void turtle(SDL_Renderer *renderer, char *instr, int nbInstr){
  float x2, y2;

  len*=0.5;
  x = SIZE / 2.5;
  y = SIZE - 10;
  angle = PI/2;

  for (int i=0; i<nbInstr; i++) {
    switch (instr[i]) {
      case 'F' : // draw forward
        x2 = x + cos(angle) * len;
        y2 = y - sin(angle) * len;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, len*2);
        SDL_RenderDrawLine(renderer, (int)x, (int)y, (int)x2, (int)y2);
        x = x2;
        y = y2;
        break;
      case '+' : // turn left
        angle-=PI/6.4;
        break;
      case '-' : // turn right
        angle+=PI/6.4;
        break;
      case '[' :
        push();
        break;
      case ']' :
        pop();
        break;
      default :
        break;
    }
  }
  SDL_RenderPresent(renderer);
}


int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("L-systemTree", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Event event;

  char word1[MAX]={'F'};
  int lenght1=1;
  char word2[MAX];
  int lenght2=0;
  char rule[20] = {'F','F','+','[','+','F','-','F','-','F',']','-','[','-','F','+','F','+','F',']'};
  int lenghtr=20;

  for (int depth=0; depth<6; depth++) {
    for (int i=0; i<lenght1; i++) {
      if (word1[i] == 'F')
        for (int j=0;j<lenghtr;j++)
          word2[lenght2++] = rule[j];
      else word2[lenght2++] = word1[i];
    }
    for (int i=0; i<lenght2; i++)
      word1[i]=word2[i];
    lenght1 = lenght2;
    lenght2 = 0;
    turtle(renderer, word1, lenght1);
  }

  while(1) {
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(256);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
