#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// #define FPS 600
#define WIDTH 256
#define HEIGHT 256
#define pNum 5000



typedef struct particle_t {
  float x, y;
  float r;
  float m;
  SDL_Color c;
  float speedX;
  float speedY;
} particule;


inline SDL_bool collide(particule *p1, particule *p2) {
  return (p2->x - p1->x)*(p2->x - p1->x) + (p2->y - p1->y)*(p2->y - p1->y) <=  4;  // all particules have a radius of 1
  // return (p2->x - p1->x)*(p2->x - p1->x) + (p2->y - p1->y)*(p2->y - p1->y) <=  (p1->r + p2->r)*(p1->r + p2->r);
  // return(abs(p2->x - p1->x)<=2 && abs(p2->y - p1->y) <=2);
}


void SDL_RenderDrawParticule(SDL_Renderer *renderer, particule *p) {
  SDL_SetRenderDrawColor(renderer, p->c.r, p->c.g, p->c.b, p->c.a);
  SDL_RenderDrawPoint(renderer, p->x, p->y);
}


int main(int argc, char *argv[]) {

  // const int frameDelay = 1000 / FPS;
  // Uint32 frameStart, frameTime;

  float friction = .99;
  float gravity = .04;
  float elasticity = .8;

  particule p[pNum] = {{0}};

  float newVelX1, newVelY1;
  float newVelX2, newVelY2;
  float collisionPointX, collisionPointY;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("particules", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  for(int i=0; i<pNum; i++){
    p[i].m   = 12;
    p[i].r   = 60;
    p[i].x   = rand()%WIDTH;
    p[i].y   = rand()%HEIGHT;
    p[i].c.r = rand()%155+100;
    p[i].c.g = rand()%155+100;
    p[i].c.b = rand()%155+100;
    p[i].c.a = 255;
    p[i].speedX = (float)(rand()%1000) / 100.0 - 5.0;
    p[i].speedY = (float)(rand()%1000) / 100.0 - 5.0;
    // for (int j = 0; j < i; j++) {
    //   if (collide(p[i], p[j])){
    //     i--;
    //     break;
    //   }
    // }
  }

  while(isRunning) {
    // frameStart = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    while(SDL_PollEvent(&event))
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) isRunning = SDL_FALSE;

    for(int i = 0; i < pNum; i++){
       // add gravity
       p[i].speedY += gravity;

      // add friction
      p[i].speedX *= friction;
      p[i].speedY *= friction;

      if (p[i].speedX > -.01 && p[i].speedX < .01 )
        p[i].speedX = 0;

      if (p[i].speedY > -.01 && p[i].speedY < .01 )
        p[i].speedY = 0;

      p[i].x += p[i].speedX;  // update position
      p[i].y += p[i].speedY;
    }

    for(int i=0; i<pNum; i++) { // bounce each other
      for (int j=0; j<i; j++) {
        if(collide(&p[i], &p[j])) {

          collisionPointX = ((p[i].x * p[j].r) + (p[j].x * p[i].r)) / (p[i].r + p[j].r);
          collisionPointY = ((p[i].y * p[j].r) + (p[j].y * p[i].r)) / (p[i].r + p[j].r);
          SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
          SDL_RenderDrawPoint(renderer, collisionPointX, collisionPointY);

          // the collision point might be INSIDE the balls - they are overlapping
          float angle = atan2f(p[i].y - p[j].y, p[i].x - p[j].x);
          float distanceBetweenCircles = sqrt( (p[i].x - p[j].x) * (p[i].x - p[j].x) + (p[i].y - p[j].y) * (p[i].y - p[j].y) );
          float distanceToMove = p[j].r + p[i].r - distanceBetweenCircles;
          p[i].x += cos(angle) * distanceToMove;
          p[i].y += sin(angle) * distanceToMove;

          newVelX1 = (p[i].speedX * (p[i].r - p[j].r) + (2 * p[j].r * p[j].speedX)) / (p[i].r + p[j].r);
          newVelY1 = (p[i].speedY * (p[i].r - p[j].r) + (2 * p[j].r * p[j].speedY)) / (p[i].r + p[j].r);
          newVelX2 = (p[j].speedX * (p[j].r - p[i].r) + (2 * p[i].r * p[i].speedX)) / (p[i].r + p[j].r);
          newVelY2 = (p[j].speedY * (p[j].r - p[i].r) + (2 * p[i].r * p[i].speedY)) / (p[i].r + p[j].r);

          p[i].speedX = newVelX1*elasticity;
          p[i].speedY = newVelY1*elasticity;
          p[j].speedX = newVelX2*elasticity;
          p[j].speedY = newVelY2*elasticity;

          p[i].x += p[i].speedX;
          p[i].y += p[i].speedY;
          p[j].x += p[j].speedX;
          p[j].y += p[j].speedY;
        }
      }
    }

    for(int i = 0; i < pNum; i++){ // bounce on borders
      if (p[i].x < p[i].r) {
        p[i].speedX *= -elasticity;
        p[i].x = p[i].r;
      }
      if (p[i].x > WIDTH - p[i].r) {
        p[i].speedX *= -elasticity;
        p[i].x = WIDTH - p[i].r;
      }
      if (p[i].y < p[i].r) {
        p[i].speedY *= -elasticity;
        p[i].y = p[i].r;
      }
      if (p[i].y > HEIGHT - p[i].r) {
        p[i].speedY *= -elasticity;
        p[i].y = HEIGHT - p[i].r;
      }
    }

    // for(int i=0; i<pNum; i++)
    //   SDL_RenderDrawParticule(renderer, &p[i]);

    // frameTime = SDL_GetTicks() - frameStart;
    // if (frameDelay > frameTime ) SDL_Delay(frameDelay - frameTime);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
