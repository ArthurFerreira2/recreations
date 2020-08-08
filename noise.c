// https://lodev.org/cgtutor/randomnoise.html

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 30

double noise[SIZE][SIZE];


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


typedef struct hsvColor_t {
  int h;  // hue        [0,255]
  int s;  // saturation [0,255]
  int v;  // value      [0,255]
} hsvColor;



void hsv2rgb(hsvColor hsv, rgbColor* rgb) {
  int region, remainder, p, q, t;

  if (hsv.s == 0) {
    rgb->r = hsv.v;
    rgb->g = hsv.v;
    rgb->b = hsv.v;
    return;
  }

  region = hsv.h / 43;
  remainder = (hsv.h - (region * 43)) * 6;

  p = (hsv.v * (255 - hsv.s)) >> 8;
  q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
  t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
    case 0:
      rgb->r = hsv.v; rgb->g = t; rgb->b = p;
      break;
    case 1:
      rgb->r = q; rgb->g = hsv.v; rgb->b = p;
      break;
    case 2:
      rgb->r = p; rgb->g = hsv.v; rgb->b = t;
      break;
    case 3:
      rgb->r = p; rgb->g = q; rgb->b = hsv.v;
      break;
    case 4:
      rgb->r = t; rgb->g = p; rgb->b = hsv.v;
      break;
    default:
      rgb->r = hsv.v; rgb->g = p; rgb->b = q;
      break;
  }
  return;
}


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


inline double frand() {
  return (double)rand() / (double)RAND_MAX;
}


void generateNoise() {
  for (int y = 0; y < SIZE; y++)
    for (int x = 0; x < SIZE; x++)
      noise[y][x] = frand();
}

double smoothNoise(double x, double y)
{
   //get fractional part of x and y
   double fractX = x - (int)(x);
   double fractY = y - (int)(y);

   //wrap around
   int x1 = ((int)(x) + SIZE) % SIZE;
   int y1 = ((int)(y) + SIZE) % SIZE;

   //neighbor values
   int x2 = (x1 + SIZE - 1) % SIZE;
   int y2 = (y1 + SIZE - 1) % SIZE;

   //smooth the noise with bilinear interpolation
   double value = 0.0;
   value += fractX     * fractY     * noise[y1][x1];
   value += (1 - fractX) * fractY     * noise[y1][x2];
   value += fractX     * (1 - fractY) * noise[y2][x1];
   value += (1 - fractX) * (1 - fractY) * noise[y2][x2];

   return value;
}

double turbulence(double x, double y, double size) {
  double value = 0.0, initialSize = size;

  while (size >= 1) {
    value += smoothNoise(x / size, y / size) * size;
    size /= 2.0;
  }

  return (128.0 * value / initialSize);
}


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;


  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("noise", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE*5, SIZE*5, SDL_WINDOW_OPENGL);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  // color = getPixel(surface, x, y);
  // SDL_GetRGBA(color, surface->format, &R, &G, &B, &A);

  rgbColor rgb;
  hslColor hsl;
  hsvColor hsv;

  Uint32 color;
  Uint8  R=0, G=0, B=0, A=255;

  double xyValue, xValue, yValue, xyPeriod, xPeriod,  yPeriod, turbPower, turbSize, sineValue, distValue;

  generateNoise();

  for(int y = 0; y < SIZE; y++) {
    for(int x = 0; x < SIZE; x++) {

      R = G = B = (Uint8)(256 * noise[x][y]);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*0, y, color);

      R = G = B = (Uint8)(256 * noise[y / 8][x / 8]);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*1, y, color);

      R = G = B = (Uint8)(256 * smoothNoise(x, y));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*2, y, color);

      R = G = B = (Uint8)(256 * smoothNoise(x / 8.0, y / 8.0));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*3, y, color);


      R = G = B = (Uint8)(turbulence(x, y, 2));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*0, y+SIZE, color);

      R = G = B = (Uint8)(turbulence(x, y, 4));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*1, y+SIZE, color);

      R = G = B = (Uint8)(turbulence(x, y, 8));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*2, y+SIZE, color);

      R = G = B = (Uint8)(turbulence(x, y, 16));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*3, y+SIZE, color);


      R = G = B = (Uint8)(turbulence(x, y, 32));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*0, y+SIZE*2, color);

      R = G = B = (Uint8)(turbulence(x, y, 64));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*1, y+SIZE*2, color);

      R = G = B = (Uint8)(turbulence(x, y, 128));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*2, y+SIZE*2, color);

      R = G = B = (Uint8)(turbulence(x, y, 256));
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*3, y+SIZE*2, color);


      hsl.h=169;
      hsl.s=255;
      hsl.l = 192 + (Uint8)(turbulence(x, y, 64))/4;
      hsl2rgb(hsl, &rgb);
      color = SDL_MapRGBA(surface->format, rgb.r, rgb.g, rgb.b, A);
      putPixel(surface, x+SIZE*0, y+SIZE*3, color);


      //xPeriod and yPeriod together define the angle of the lines
      //xPeriod and yPeriod both 0 ==> it becomes a normal clouds or turbulence pattern
      xPeriod = 5.0; //defines repetition of marble lines in x direction
      yPeriod = 10.0; //defines repetition of marble lines in y direction
      //turbPower = 0 ==> it becomes a normal sine pattern
      turbPower = 5.0; //makes twists
      turbSize = 16.0; //initial size of the turbulence
      xyValue = x * xPeriod / SIZE + y * yPeriod / SIZE + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 256 * fabs(sin(xyValue * 3.14159));
      R = G = B = (Uint8)(sineValue);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*1, y+SIZE*3, color);


      turbSize = 32.0;
      xyValue = x * xPeriod / SIZE + y * yPeriod / SIZE + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 256 * fabs(sin(xyValue * 3.14159));
      R = G = B = (Uint8)(sineValue);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*2, y+SIZE*3, color);


      turbSize = 128.0;
      xyValue = x * xPeriod / SIZE + y * yPeriod / SIZE + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 256 * fabs(sin(xyValue * 3.14159));
      R = G = B = (Uint8)(sineValue);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*3, y+SIZE*3, color);

      turbPower = 1.0;
      turbSize = 32.0;
      xyValue = x * xPeriod / SIZE + y * yPeriod / SIZE + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 256 * fabs(sin(xyValue * 3.14159));
      R = G = B = (Uint8)(sineValue);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*0, y+SIZE*4, color);


      xPeriod = 0.0;
      yPeriod = 1.0;
      xyValue = x * xPeriod / SIZE + y * yPeriod / SIZE + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 256 * fabs(sin(xyValue * 3.14159));
      R = G = B = (Uint8)(sineValue);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*1, y+SIZE*4, color);

      turbPower = 5.0;
      xyValue = x * xPeriod / SIZE + y * yPeriod / SIZE + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 256 * fabs(sin(xyValue * 3.14159));
      R = G = B = (Uint8)(sineValue);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*2, y+SIZE*4, color);


      xyValue = x * xPeriod / SIZE + y * yPeriod / SIZE + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 226 * fabs(sin(xyValue * 3.14159));
      R = (Uint8)(30 + sineValue);
      G = (Uint8)(10 + sineValue);
      B = (Uint8)(sineValue);
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*3, y+SIZE*4, color);


// WOOD, last column
      xyPeriod = 12.0; //number of rings
      turbPower = 0.1; //makes twists
      turbSize = 32.0; //initial size of the turbulence
      xValue = (x - SIZE / 2) / (double)(SIZE);
      yValue = (y - SIZE / 2) / (double)(SIZE);
      distValue = sqrt(xValue * xValue + yValue * yValue) + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 128.0 * fabs(sin(2 * xyPeriod * distValue * 3.14159));
      R = (Uint8)(80 + sineValue);
      G = (Uint8)(30 + sineValue);
      B = 30;
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*4, y+SIZE*0, color);

      xyPeriod = 25.0;
      turbPower = 0.1;
      turbSize = 32.0;
      xValue = (x - SIZE / 2) / (double)(SIZE);
      yValue = (y - SIZE / 2) / (double)(SIZE);
      distValue = sqrt(xValue * xValue + yValue * yValue) + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 128.0 * fabs(sin(2 * xyPeriod * distValue * 3.14159));
      R = (Uint8)(80 + sineValue);
      G = (Uint8)(30 + sineValue);
      B = 30;
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*4, y+SIZE*1, color);

      xyPeriod = 12.0;
      turbPower = 0.2;
      turbSize = 32.0;
      xValue = (x - SIZE / 2) / (double)(SIZE);
      yValue = (y - SIZE / 2) / (double)(SIZE);
      distValue = sqrt(xValue * xValue + yValue * yValue) + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 128.0 * fabs(sin(2 * xyPeriod * distValue * 3.14159));
      R = (Uint8)(80 + sineValue);
      G = (Uint8)(30 + sineValue);
      B = 30;
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*4, y+SIZE*2, color);

      xyPeriod = 12.0;
      turbPower = 0.5;
      turbSize = 32.0;
      xValue = (x - SIZE / 2) / (double)(SIZE);
      yValue = (y - SIZE / 2) / (double)(SIZE);
      distValue = sqrt(xValue * xValue + yValue * yValue) + turbPower * turbulence(x, y, turbSize) / 256.0;
      sineValue = 128.0 * fabs(sin(2 * xyPeriod * distValue * 3.14159));
      R = (Uint8)(80 + sineValue);
      G = (Uint8)(30 + sineValue);
      B = 30;
      color = SDL_MapRGBA(surface->format, R, G, B, A);
      putPixel(surface, x+SIZE*4, y+SIZE*3, color);

      turbPower = 0.2;
      xValue = (x - SIZE / 2) / (double)(SIZE) + turbPower * turbulence(x, y, turbSize) / 256.0;
      yValue = (y - SIZE / 2) / (double)(SIZE) + turbPower * turbulence(SIZE - y, SIZE - x, turbSize) / 256.0;
      sineValue = 22.0 * fabs(sin(xyPeriod * xValue * 3.1415) + sin(xyPeriod * yValue * 3.1415));
      hsv.h=(Uint8)(sineValue);
      hsv.s=255;
      hsv.v = 255;
      hsv2rgb(hsv, &rgb);
      color = SDL_MapRGBA(surface->format, rgb.r, rgb.g, rgb.b, A);
      putPixel(surface, x+SIZE*4, y+SIZE*4, color);

    }
  }


  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
