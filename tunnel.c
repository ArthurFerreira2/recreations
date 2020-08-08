// https://lodev.org/cgtutor/tunnel.html

#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>

#define SIZE 256
#define FPS 60


void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
  if (x<0 || y <0 || x>=surface->w || y>=surface->h) return;

  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  if (bpp==4) { *(Uint32 *)p = pixel; return; }
  if (bpp==1) { *p = pixel; return; }
  if (bpp==2) { *(Uint16 *)p = pixel; return; }
  if (bpp==3) {
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
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
  return;
}


Uint32 getPixel(SDL_Surface *surface, int x, int y){

  if (x<0 || y <0 || x>=surface->w || y>=surface->h) return 0;

  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  if (bpp==1) return *p;
  if (bpp==2) return *(Uint16 *)p;
  if (bpp==3) {
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  }


  if (bpp==4) return *(Uint32 *)p;
  return 0;
}


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  SDL_Surface *texSurf = IMG_Load("tunTexture.png");
  int texWidth  = texSurf->w;
  int texHeight = texSurf->h;
  SDL_Window *window = SDL_CreateWindow("tunnel", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  int **distanceTable=malloc(2*SIZE*sizeof(int));
  for (int x=0; x<2*SIZE; x++)
    distanceTable[x] = malloc(2*SIZE*sizeof(int));

  int **angleTable=malloc(2*SIZE*sizeof(int));
  for (int x=0; x<2*SIZE; x++)
    angleTable[x] = malloc(2*SIZE*sizeof(int));

  Uint32 color;
  float animation;
  int shiftX, shiftY, shiftLookX, shiftLookY;

  for(int y = 0; y < SIZE * 2; y++) {
    for(int x = 0; x < SIZE * 2; x++) {
      int angle, distance;
      float ratio = 32.0;
      //these formulas are changed to work with the new center of the tables
      distance = (int)(ratio * texHeight / sqrt((float)((x - SIZE) * (x - SIZE) + (y - SIZE) * (y - SIZE)))) % texHeight;
      angle = (int)(0.5 * texWidth * atan2((float)(y - SIZE), (float)(x - SIZE)) / 3.14159265);
      distanceTable[y][x] = distance;
      angleTable[y][x] = angle;
    }
  }


  while(isRunning) {
    frameStart = SDL_GetTicks();
    animation = frameStart / 600.0;

    // calculate the shift values out of the animation value
    shiftX = (int)(texWidth * 1.0 * animation);
    shiftY = (int)(texHeight * 0.25 * animation);
    // calculate the look values out of the animation value
    // by using sine functions, it'll alternate between looking left/right and up/down
    // make sure that x + shiftLookX never goes outside the dimensions of the table, same for y
    shiftLookX = SIZE / 2 + (int)(SIZE / 4 * sin(animation));
    shiftLookY = SIZE / 2 + (int)(SIZE / 4 * sin(animation * 2.0));

    for(int y = 0; y < SIZE; y++)
      for(int x = 0; x < SIZE; x++) {
        // get the texel from the texture by using the tables, shifted with the animation variable
        // now, x and y are shifted as well with the "look" animation values
        color = getPixel(texSurf, (int)(distanceTable[x + shiftLookX][y + shiftLookY] + shiftX)%texWidth,
                                  (int)(   angleTable[x + shiftLookX][y + shiftLookY] + shiftY)%texHeight);
        putPixel(winSurf, x, y, color);
      }

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);

    SDL_UpdateWindowSurface(window);
  }

  SDL_FreeSurface(texSurf);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
