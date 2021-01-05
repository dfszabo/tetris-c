#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdio.h>

int init_graphics();
void draw_square(int x, int y, int color);
void quit_graphics();

#endif