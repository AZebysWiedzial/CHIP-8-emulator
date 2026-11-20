#ifndef GRAPHICS_H
#define GRAPHICS_H

#include<SDL3/SDL.h>

#include "chip8State.h"

#define TILE_SIZE 10
#define WINDOW_TITLE "test"
#define DEBUGGER_WIDTH 100

typedef struct graphicsContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} graphicsContext;

void initGraphics(graphicsContext* gc, const char* title);
void updateDisplay(chip8State* state, graphicsContext* gc);
void deleteGraphics(graphicsContext* gc);
void removeExtension(char* str);
void showPanel(graphicsContext* gc);
#endif