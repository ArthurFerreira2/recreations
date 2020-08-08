// https://lodev.org/cgtutor/randomnoise.html#3D_Random_Noise

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 120

#define noiseWidth 256
#define noiseHeight 256
#define noiseDepth 64

double noise[noiseDepth][noiseHeight][noiseWidth];



typedef struct rgbColor_t {
  int r;  // red   [0.255]
  int g;  // green [0,255]
  int b;  // blue  [0,255]
} rgbColor;


typedef struct hslColor_t {
  int h;  // hue        [0,255]
  int s;  // saturation [0,255]
  int l;  // lightness  [0,255]
} hslColor;


void hsl2rgb(hslColor hsl, rgbColor *rgb) {
  float r, g, b, h, s, l; //this function works with floats between 0 and 1
  float temp1, temp2, tempr, tempg, tempb;
  h = hsl.h / 256.0;
  s = hsl.s / 256.0;
  l = hsl.l / 256.0;

  //If saturation is 0, the color is a shade of gray
  if (s == 0) r = g = b = l;
  //If saturation > 0, more complex calculations are needed
  else {
    //Set the temporary values
    if (l < 0.5) temp2 = l * (1 + s);
    else temp2 = (l + s) - (l * s);
    temp1 = 2 * l - temp2;
    tempr = h + 1.0 / 3.0;
    if (tempr > 1) tempr--;
    tempg = h;
    tempb = h - 1.0 / 3.0;
    if (tempb < 0) tempb++;

    //Red
    if (tempr < 1.0 / 6.0) r = temp1 + (temp2 - temp1) * 6.0 * tempr;
    else if (tempr < 0.5) r = temp2;
    else if (tempr < 2.0 / 3.0) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
    else r = temp1;

    //Green
    if (tempg < 1.0 / 6.0) g = temp1 + (temp2 - temp1) * 6.0 * tempg;
    else if (tempg < 0.5) g = temp2;
    else if (tempg < 2.0 / 3.0) g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
    else g = temp1;

    //Blue
    if (tempb < 1.0 / 6.0) b = temp1 + (temp2 - temp1) * 6.0 * tempb;
    else if (tempb < 0.5) b = temp2;
    else if (tempb < 2.0 / 3.0) b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
    else b = temp1;
  }

  rgb->r = (int)(r * 255.0);
  rgb->g = (int)(g * 255.0);
  rgb->b = (int)(b * 255.0);
}



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


inline double frand() {
  return (double)rand() / (double)RAND_MAX;
}


void generateNoise() {
  for (int z = 0; z < noiseDepth; z++)
    for (int y = 0; y < noiseHeight; y++)
      for (int x = 0; x < noiseWidth; x++)
        noise[z][y][x] = frand();
}


double smoothNoise(double x, double y, double z) {
  //get fractional part of x and y
  double fractX = x - (int)(x);
  double fractY = y - (int)(y);
  double fractZ = z - (int)(z);

  //wrap around
  int x1 = ((int)(x) + noiseWidth)  % noiseWidth;
  int y1 = ((int)(y) + noiseHeight) % noiseHeight;
  int z1 = ((int)(z) + noiseDepth)  % noiseDepth;

  //neighbor values
  int x2 = (x1 + noiseWidth - 1) % noiseWidth;
  int y2 = (y1 + noiseHeight- 1) % noiseHeight;
  int z2 = (z1 + noiseDepth - 1) % noiseDepth;

  //smooth the noise with bilinear interpolation
  double value = 0.0;
  value += fractX       * fractY       * fractZ     * noise[z1][y1][x1];
  value += fractX       * (1 - fractY) * fractZ     * noise[z1][y2][x1];
  value += (1 - fractX) * fractY       * fractZ     * noise[z1][y1][x2];
  value += (1 - fractX) * (1 - fractY) * fractZ     * noise[z1][y2][x2];

  value += fractX       * fractY       * (1 - fractZ) * noise[z2][y1][x1];
  value += fractX       * (1 - fractY) * (1 - fractZ) * noise[z2][y2][x1];
  value += (1 - fractX) * fractY       * (1 - fractZ) * noise[z2][y1][x2];
  value += (1 - fractX) * (1 - fractY) * (1 - fractZ) * noise[z2][y2][x2];

  return value;
}


double turbulence(double x, double y, double z, double size) {
  double value = 0.0, initialSize = size;

  while (size >= 1) {
    value += smoothNoise(x / size, y / size, z / size) * size;
    size /= 2.0;
  }
  return (128.0 * value / initialSize);
}


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("noise3d", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  Uint32 color;
  hslColor hsl;
  rgbColor rgb;
  double t=.0;

  generateNoise();

  while(isRunning) {
    frameStart = SDL_GetTicks();

    t+=.2;
    for(int y = 0; y < SIZE; y++) {
      for(int x = 0; x < SIZE; x++) {
        hsl.h = 145;
        hsl.s = 255;
        hsl.l = 128 + (int)(turbulence(x, y, t, 32)) / 2;
        hsl2rgb(hsl, &rgb);
        color = SDL_MapRGBA(surface->format, rgb.r, rgb.g, rgb.b, SDL_ALPHA_OPAQUE);
        putPixel(surface, x, y, color);
      }
    }

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
      SDL_Delay(frameDelay - frameTime);

    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
