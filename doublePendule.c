// https://experiences.math.cnrs.fr/Iterations-de-l-application.html
// https://github.com/luisovidio/pendulum
// https://fr.qwe.wiki/wiki/Runge%E2%80%93Kutta_methods
// https://www.idpoisson.fr/perrollaz/PenduleDouble.html


#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 800
#define FPS 60
#define PI 3.1415926535

const double lenght1=SIZE/10, lenght2=SIZE/4, M1=8, M2=10, G=9.78, h=0.08;
double theta1 = PI + 0.0000001, theta2 = PI, Omega1 = 0, Omega2 = 0;


void derive(double In[], double dydx[]) {

  double D = In[1] - In[0];   // theta2 - theta1
  double DM = M2 + M1;
  double denomiateur = DM * lenght1 - M2 * lenght1 * cos(D) * cos(D);

  dydx[0] = In[2];

  dydx[2] = (M2*lenght1*In[2]*In[2]*sin(D)*cos(D) + M2*G*sin(In[1])*cos(D)
            + M2*lenght2*In[3]*In[3]*sin(D) - DM*G*sin(In[0])) / denomiateur;

  dydx[1] = In[3];

  denomiateur *= lenght2 / lenght1;

  dydx[3] = (-M2*lenght2*In[3]*In[3]*sin(D)*cos(D) + DM*G*sin(In[0])*cos(D)
            - DM*lenght1*In[2]*In[2]*sin(D) - DM*G*sin(In[1])) / denomiateur;
}


void compute() {

  double In[4] = {theta1, theta2, Omega1, Omega2};
  double Temp[4], dydx[4];
  double K1[4], K2[4], K3[4], K4[4];

  derive(In, dydx);
  for(int i = 0; i < 4; i++) {
    K1[i] = h * dydx[i];
    Temp[i] = In[i] + K1[i]/2.0;
  }
  derive(Temp, dydx);
  for(int i = 0; i < 4; i++) {
    K2[i] = h * dydx[i];
    Temp[i] = In[i] + K2[i]/2.0;
  }
  derive(Temp, dydx);
  for(int i = 0; i < 4; i++) {
    K3[i] = h * dydx[i];
    Temp[i] = In[i] + K3[i];
  }
  derive(Temp, dydx);
  for(int i = 0; i < 4; i++) {
    K4[i] = h * dydx[i];
    Temp[i] = In[i] + K1[i] / 6.0 + K2[i] / 3.0 + K3[i] / 3.0 + K4[i] / 6.0;
  }
  theta1 = Temp[0];
  theta2 = Temp[1];
  Omega1 = Temp[2];
  Omega2 = Temp[3];
}


int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window=SDL_CreateWindow("Double Pendule", SDL_WINDOWPOS_CENTERED,
                     SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect canvas = {0, 0, SIZE, SIZE};
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;
  SDL_Rect rect1 = {0, 0, M1, M1};
  SDL_Rect rect2 = {0, 0, M2, M2};
  SDL_Point dots[256] = {0};
  Uint8 dot = 0;
  double X1, X2, Y1, Y2;
  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  while(isRunning) {
    frameStart=SDL_GetTicks();

    // poll inputs
    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    // update model
    compute();

    // polar to cartesian
    X1 = lenght1 * sin(theta1) + SIZE/2;
    Y1 = lenght1 * cos(theta1) + SIZE/2;
    X2 = lenght2 * sin(theta2) + X1;
    Y2 = lenght2 * cos(theta2) + Y1;

    // cover the screen, using a semi transparent rectangle
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 32);
    SDL_RenderFillRect(renderer, &canvas);

    // draw 1st leg
    SDL_SetRenderDrawColor(renderer, 000, 000, 255, SDL_ALPHA_OPAQUE);  // Blue
    SDL_RenderDrawLine(renderer, SIZE/2, SIZE/2, X1, Y1);
    rect1.x = X1 - rect1.w/2;
    rect1.y = Y1 - rect1.h/2;
    SDL_RenderFillRect(renderer, &rect1);

    // draw 2nd leg
    SDL_SetRenderDrawColor(renderer, 255, 000, 000, SDL_ALPHA_OPAQUE);  // red
    SDL_RenderDrawLine(renderer, X1, Y1, X2, Y2);
    rect2.x = X2 - rect2.w/2;
    rect2.y = Y2 - rect2.h/2;
    SDL_RenderFillRect(renderer, &rect2);

    // draw the white dots - look the top left corner at the start
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white
    dots[dot].x = X2;
    dots[dot].y = Y2;
    dot++;
    for(int i=0; i<256; i++)
      SDL_RenderDrawPoint(renderer, dots[i].x, dots[i].y);

    // wait for next frame and render
    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
