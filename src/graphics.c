#include "../includes/graphics.h"
#include "../includes/constants.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int init_graphics() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    return FALSE;

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);

  if (window == NULL)
    return FALSE;

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL)
    return FALSE;

  return TRUE;
}

void quit_graphics() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void draw_square(int x, int y, int color);