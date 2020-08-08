#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define FPS 6000
#define WIDTH 256
#define HEIGHT 512
#define cNum 400


typedef struct circle_t {
  float x, y;
  float r;
  SDL_Color c;
  float speedX;
  float speedY;
} circle;


inline SDL_bool collide(circle c1, circle c2) {
  return (c2.x-c1.x)*(c2.x-c1.x) + (c2.y-c1.y)*(c2.y-c1.y) <=  (c1.r+c2.r)*(c1.r+c2.r);
}


void SDL_RenderDrawCircle(SDL_Renderer *renderer, circle *c) {

  SDL_SetRenderDrawColor(renderer, c->c.r, c->c.g, c->c.b, c->c.a);
  int x=0, y=c->r, p=1-c->r;

  for (x=0; x<=y; x++) {
    if (p<0) p+=2*x ;
    else {
      y-=1;
      p+=2*(x-y);
    }
    SDL_RenderDrawPoint(renderer, c->x+x, c->y-y);
    SDL_RenderDrawPoint(renderer, c->x-x, c->y-y);
    SDL_RenderDrawPoint(renderer, c->x+x, c->y+y);
    SDL_RenderDrawPoint(renderer, c->x-x, c->y+y);
    SDL_RenderDrawPoint(renderer, c->x+y, c->y-x);
    SDL_RenderDrawPoint(renderer, c->x-y, c->y-x);
    SDL_RenderDrawPoint(renderer, c->x+y, c->y+x);
    SDL_RenderDrawPoint(renderer, c->x-y, c->y+x);
  }
}


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  float friction = .99;
  float gravity = .04;
  float elasticity = .8;

  circle c[cNum] = {{0}};

  float newVelX1, newVelY1;
  float newVelX2, newVelY2;
  float collisionPointX, collisionPointY;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("balls", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;
  SDL_ShowCursor(SDL_DISABLE);

  for(int i=0; i<cNum; i++){
    c[i].r = 8;
    c[i].x = rand()%(int)(WIDTH-c[i].r*2) + c[i].r;
    c[i].y = rand()%(int)(HEIGHT-c[i].r*2) + c[i].r;
    c[i].c.r = rand()%55+200;
    c[i].c.g = rand()%55+200;
    c[i].c.b = rand()%55+200;
    c[i].c.a = 255;
    c[i].speedX = (float)(rand()%1000) / 100.0 - 5.0;
    c[i].speedY = (float)(rand()%1000) / 100.0 - 5.0;
    // for (int j = 0; j < i; j++) {
    //   if (collide(c[i], c[j])){
    //     i--;
    //     break;
    //   }
    // }
  }

  while(isRunning) {
    frameStart = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    while(SDL_PollEvent(&event))
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) isRunning = SDL_FALSE;

    for(int i = 0; i < cNum; i++){
       // add gravity
       c[i].speedY += gravity;

      // add friction
      c[i].speedX *= friction;
      c[i].speedY *= friction;

      if (c[i].speedX > -.01 && c[i].speedX < .01 )
        c[i].speedX = 0;

      if (c[i].speedY > -.01 && c[i].speedY < .01 )
        c[i].speedY = 0;

      c[i].x += c[i].speedX;  // update position
      c[i].y += c[i].speedY;
    }

    for(int i=0; i<cNum; i++) { // bounce each other
      for (int j=0; j<i; j++) {
        if(collide(c[i], c[j])) {

          collisionPointX = ((c[i].x * c[j].r) + (c[j].x * c[i].r)) / (c[i].r + c[j].r);
          collisionPointY = ((c[i].y * c[j].r) + (c[j].y * c[i].r)) / (c[i].r + c[j].r);
          SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
          SDL_RenderDrawPoint(renderer, collisionPointX, collisionPointY);

          // the collision point might be INSIDE the balls - they are overlapping
          float angle = atan2f(c[i].y - c[j].y, c[i].x - c[j].x);
          float distanceBetweenCircles = sqrt((c[i].x-c[j].x)*(c[i].x-c[j].x)+(c[i].y-c[j].y)*(c[i].y-c[j].y));
          float distanceToMove = c[j].r + c[i].r - distanceBetweenCircles;
          c[i].x += cos(angle) * distanceToMove;
          c[i].y += sin(angle) * distanceToMove;

          newVelX1 = (c[i].speedX * (c[i].r - c[j].r) + (2 * c[j].r * c[j].speedX)) / (c[i].r + c[j].r);
          newVelY1 = (c[i].speedY * (c[i].r - c[j].r) + (2 * c[j].r * c[j].speedY)) / (c[i].r + c[j].r);
          newVelX2 = (c[j].speedX * (c[j].r - c[i].r) + (2 * c[i].r * c[i].speedX)) / (c[i].r + c[j].r);
          newVelY2 = (c[j].speedY * (c[j].r - c[i].r) + (2 * c[i].r * c[i].speedY)) / (c[i].r + c[j].r);

          c[i].speedX = newVelX1*elasticity;
          c[i].speedY = newVelY1*elasticity;
          c[j].speedX = newVelX2*elasticity;
          c[j].speedY = newVelY2*elasticity;

          c[i].x += c[i].speedX;
          c[i].y += c[i].speedY;
          c[j].x += c[j].speedX;
          c[j].y += c[j].speedY;
        }
      }
    }

    for(int i = 0; i < cNum; i++){ // bounce on borders
      if (c[i].x < c[i].r) {
        c[i].speedX *= -elasticity;
        c[i].x = c[i].r;
      }
      if (c[i].x > WIDTH - c[i].r) {
        c[i].speedX *= -elasticity;
        c[i].x = WIDTH - c[i].r;
      }
      if (c[i].y < c[i].r) {
        c[i].speedY *= -elasticity;
        c[i].y = c[i].r;
      }
      if (c[i].y > HEIGHT - c[i].r) {
        c[i].speedY *= -elasticity;
        c[i].y = HEIGHT - c[i].r;
      }
    }

    for(int i=0; i<cNum; i++)
      SDL_RenderDrawCircle(renderer, &c[i]);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime )
      SDL_Delay(frameDelay - frameTime);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
