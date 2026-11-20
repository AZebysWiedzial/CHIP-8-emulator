#ifndef GRAPHICS_H
#define GRAPHICS_H

#include<SDL3/SDL.h>

#include "Chip8State.h"

#define TILE_SIZE 10
#define WINDOW_TITLE "test"

typedef struct graphicalContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} graphicalContext;

void initGraphics(graphicalContext* gc, const char* title);
void updateDisplay(Chip8State* state, graphicalContext* gc);
void deleteGraphics(graphicalContext* gc);
void removeExtension(char* str);
#endif