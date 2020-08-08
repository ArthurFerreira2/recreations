// https://lodev.org/cgtutor/color.html#The_HSV_Color_Model

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 30

#define MAX_COLORS 16

// 16 colors palette from : http://alumni.media.mit.edu/~wad/color/palette.html
#define Black   (uint32_t)0        //   #000000 000
#define Brown   (uint32_t)8473113  //   #814A19 228
#define Green   (uint32_t)1927444  //   #1D6914 232
#define Red     (uint32_t)11346723 //   #AD2323 243
#define DkGray  (uint32_t)5723991  //   #575757 261
#define Blue    (uint32_t)2771927  //   #2A4BD7 316
#define Purple  (uint32_t)8464064  //   #8126C0 359
#define LtGreen (uint32_t)8504698  //   #81C57A 448
#define Orange  (uint32_t)16749107 //   #FF9233 452
#define Cyan    (uint32_t)2740432  //   #29D0D0 457
#define LtGray  (uint32_t)10526880 //   #A0A0A0 480
#define Yellow  (uint32_t)16772659 //   #FFEE33 544
#define LtBlue  (uint32_t)10334207 //   #9DAFFF 587
#define Tan     (uint32_t)15326907 //   #E9DEBB 642
#define Pink    (uint32_t)16764403 //   #FFCDF3 703
#define White   (uint32_t)16777215 //   #FFFFFF 765


uint32_t palette[16] = {Black, Brown, Green, Red, DkGray, Blue, Purple, LtGreen,
                        Orange, Cyan, LtGray, Yellow, LtBlue, Tan, Pink, White};


typedef struct rgbColor_t {
  int r;  // red   [0.255]
  int g;  // green [0,255]
  int b;  // blue  [0,255]
} rgbColor;


typedef struct hsvColor_t {
  int h;  // hue        [0,255]
  int s;  // saturation [0,255]
  int v;  // value      [0,255]
} hsvColor;


typedef struct hslColor_t {
  int h;  // hue        [0,255]
  int s;  // saturation [0,255]
  int l;  // lightness  [0,255]
} hslColor;


void rgb2hsv(rgbColor rgb, hsvColor* hsv) {
  int rgbMin, rgbMax;

  rgbMin = rgb.r<rgb.g ? (rgb.r<rgb.b?rgb.r:rgb.b) : (rgb.g<rgb.b?rgb.g:rgb.b);
  rgbMax = rgb.r>rgb.g ? (rgb.r>rgb.b?rgb.r:rgb.b) : (rgb.g>rgb.b?rgb.g:rgb.b);

  hsv->v = rgbMax;
  if (hsv->v == 0) {
    hsv->h = 0;
    hsv->s = 0;
    return;
  }

  hsv->s = 255 * (long)(rgbMax - rgbMin) / hsv->v;
  if (hsv->s == 0) {
    hsv->h = 0;
    return;
  }

  if (rgbMax == rgb.r)
    hsv->h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
  else if (rgbMax == rgb.g)
    hsv->h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
  else
    hsv->h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);
  return;
}


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



void rgb2hsl(rgbColor rgb, hslColor *hsl) {
  float r, g, b, h, s, l;  // this function works with floats between 0 and 1
  r = rgb.r / 256.0;
  g = rgb.g / 256.0;
  b = rgb.b / 256.0;
  float maxColor = fmax(r, fmax(g, b));
  float minColor = fmin(r, fmin(g, b));

  // R == G == B, so it's a shade of gray
  if(minColor == maxColor) {
    h = 0.0;  // it doesn't matter what value it has
    s = 0.0;
    l = r;    // doesn't matter if you pick r, g, or b
  }
  else {
    l = (minColor + maxColor) / 2;

    if (l < 0.5) s = (maxColor - minColor) / (maxColor + minColor);
    else s = (maxColor - minColor) / (2.0 - maxColor - minColor);

    if (r == maxColor) h = (g - b) / (maxColor - minColor);
    else if (g == maxColor) h = 2.0 + (b - r) / (maxColor - minColor);
    else h = 4.0 + (r - g) / (maxColor - minColor);

    h /= 6;  // to bring it to a number between 0 and 1
    if(h < 0) h ++;
  }

  hsl->h = (int)(h * 255.0);
  hsl->s = (int)(s * 255.0);
  hsl->l = (int)(l * 255.0);
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


double distance(double x1, double y1, double x2, double y2) {
  double dist = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
  return dist;
}



int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  int x=SIZE/2, y=SIZE/2;
  Uint32 color;
  Uint8  R=0, G=0, B=0, A=255;

  const double Rx = 0;
  const double Ry = 0;
  const double Gx = 0;
  const double Gy = SIZE-1;
  const double Bx = SIZE-1;
  const double By = SIZE-1;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Colors2", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  // for (int i=0; i<MAX_COLORS; i++)
  //   for (int j=0; j<SIZE/5; j++)
  //     for (int a=0; a<SIZE/MAX_COLORS; a++)
  //       putPixel(surface, a+i*SIZE/MAX_COLORS, j, palette[i]);

  for (double i=0; i<SIZE; i++) {
    for (double j=0; j<SIZE; j++) {
      R = 255 - distance(i, j, Rx, Ry)/(SIZE*sqrt(2))*255;
      G = 255 - distance(i, j, Gx, Gy)/(SIZE*sqrt(2))*255;
      B = 255 - distance(i, j, Bx, By)/(SIZE*sqrt(2))*255;
      color = SDL_MapRGBA(surface->format,  R, G, B, A);
      putPixel(surface, (int)i, (int)j, color);
    }
  }

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    color = getPixel(surface, x, y);
    SDL_GetRGBA(color, surface->format, &R, &G, &B, &A);
    R=(R+(rand()%3)-1);
    G=(G+(rand()%3)-1);
    B=(B+(rand()%3)-1);
    color = SDL_MapRGBA(surface->format, R, G, B, A);
    putPixel(surface, x, y, color);
    x=(SIZE+x+(rand()%3)-1)%SIZE;
    y=(SIZE+y+(rand()%3)-1)%SIZE;

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
