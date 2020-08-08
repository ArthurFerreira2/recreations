#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


Uint32 getPixel(SDL_Surface *surface, int x, int y){

  // if (x<0 || y <0 || x>=surface->w || y>=surface->h) return 0;

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

  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  SDL_Surface *surface = IMG_Load("lena.png");
  int width  = surface->w;
  int height = surface->h;
  SDL_Window *window = SDL_CreateWindow("Floyd-Steinberg", SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


  Uint8 R, G, B, A;
  Uint32 color;
  float greyLevel, oldpixel, newpixel;
  float sum=.0, average=.0, error=.0;

  // each pixel will have a grey level value between 0 (black) and 1 (white)
  float **pixel=malloc(width * sizeof(float*));
  for (int x=0; x<width; x++)
    pixel[x]=malloc(height * sizeof(float));

  // first, convert the image to grey scale and store it into pixel
  for (int y=0; y<height; y++)
    for (int x=0; x<width; x++) {
      color = getPixel(surface, x, y);
      SDL_GetRGBA(color, surface->format, &R, &G, &B, &A);
      greyLevel = (float)(R+G+B)/3.0/255.0;
      sum += greyLevel;
      pixel[x][y] = greyLevel;
    }

  average = sum / (float)(width*height);

  // https://fr.wikipedia.org/wiki/Algorithme_de_Floyd-Steinberg
  for (int y=1; y<height-1; y++)
    for (int x=1; x<width-1; x++) {

      oldpixel = pixel[x][y];
      newpixel= (oldpixel < average) ? 0 : 1;
      pixel[x][y] = newpixel;

      error = oldpixel - newpixel;
      pixel[x + 1][y    ] += error * (7.0/16.0);
      pixel[x - 1][y + 1] += error * (3.0/16.0);
      pixel[x    ][y + 1] += error * (5.0/16.0);
      pixel[x + 1][y + 1] += error * (1.0/16.0);

      SDL_SetRenderDrawColor(renderer, newpixel*255, newpixel*255, newpixel*255, 255);
      SDL_RenderDrawPoint(renderer, x, y);
    }

  SDL_RenderPresent(renderer);


  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;
  while(isRunning) {
    SDL_Delay(200);
    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning = SDL_FALSE;
  }

  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
