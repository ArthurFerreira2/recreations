#include <stdlib.h>
#include <SDL2/SDL.h>

#define SIZE  256

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("trust", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
  SDL_ShowCursor(SDL_DISABLE);

  SDL_Rect rect = {0, 0, 10, 10};
  float speedX = 0, speedY = 0;
  float x = (SIZE - rect.w) / 2;
  float y = (SIZE - rect.h) / 2;

  const float accelleration = .8;
  const float maxSpeed      = 50;
  const float gravity       = .4;
  const float friction      = .97;

  SDL_bool GO_UP    = SDL_FALSE;
  SDL_bool GO_DOWN  = SDL_FALSE;
  SDL_bool GO_LEFT  = SDL_FALSE;
  SDL_bool GO_RIGHT = SDL_FALSE;

  SDL_bool running = SDL_TRUE;
  SDL_Event event;


  while(running) {

    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = SDL_FALSE;

      if (event.type ==  SDL_KEYDOWN)
        switch (event.key.keysym.sym) {
          case SDLK_LEFT:   GO_LEFT = SDL_TRUE; break;
          case SDLK_RIGHT: GO_RIGHT = SDL_TRUE; break;
          case SDLK_UP:       GO_UP = SDL_TRUE; break;
          case SDLK_DOWN:  GO_DOWN  = SDL_TRUE; break;
        }

      if (event.type ==  SDL_KEYUP)
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE: running = SDL_FALSE; break;
          case SDLK_LEFT:   GO_LEFT = SDL_FALSE; break;
          case SDLK_RIGHT: GO_RIGHT = SDL_FALSE; break;
          case SDLK_UP:       GO_UP = SDL_FALSE; break;
          case SDLK_DOWN:   GO_DOWN = SDL_FALSE; break;
        }
    }

    // update speed
    if (GO_LEFT  && speedX >= -maxSpeed) speedX -= accelleration;
    if (GO_RIGHT && speedX <=  maxSpeed) speedX += accelleration;
    if (GO_UP    && speedY >= -maxSpeed) speedY -= accelleration;
    if (GO_DOWN  && speedY <=  maxSpeed) speedY += accelleration;

    // gravity effect
    speedY += gravity;

    // friction effect
    speedX *= friction;
    speedY *= friction;
    if (speedX <= .3 && speedX >= -.3) speedX = 0;
    if (speedY <= .3 && speedY >= -.3) speedY = 0;

    // update object coodinates
    x += speedX;
    y += speedY;

    // edge colision
    if (x < 0)             { speedX = 0; x = 0;             }
    if (x > SIZE - rect.w) { speedX = 0; x = SIZE - rect.w; }
    if (y < 0)             { speedY = 0; y = 0;             }
    if (y > SIZE - rect.h) { speedY = 0; y = SIZE - rect.h; }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    rect.x = (int)x;
    rect.y = (int)y;

    if (GO_LEFT || GO_RIGHT || GO_UP || GO_DOWN)
      SDL_SetRenderDrawColor(renderer, 200, 100, 100, SDL_ALPHA_OPAQUE);
    else
      SDL_SetRenderDrawColor(renderer, 100, 100, 200, SDL_ALPHA_OPAQUE);

    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
