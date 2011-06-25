#include <SDL.h>



int main(int argc, char **argv)
{
  SDL_Init( SDL_INIT_VIDEO );

  SDL_Surface* screen = SDL_SetVideoMode( 0, 0, 0, SDL_SWSURFACE );

  SDL_FillRect (screen, NULL, 0x0088ff);

  SDL_Flip (screen);

  SDL_Event event;
  bool gameRunning = true;

  while (gameRunning)
  {
    if (SDL_WaitEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        gameRunning = false;
      }
    } 
  }
  printf("End Loop");

  SDL_Quit();

  return 0;

}


