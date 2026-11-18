#ifndef GRAPHICS_H
#define GRAPHICS_H

#include<SDL3/SDL.h>

#include "Chip8State.h"

#define TILE_SIZE 10
#define WIDTH 64
#define HEIGHT 32
#define WINDOW_TITLE "test"

typedef struct graphicalContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} graphicalContext;

void initGraphics(graphicalContext* gc);
void updateDisplay(Chip8State* state, graphicalContext* gc);
void deleteGraphics(graphicalContext* gc);
#endif