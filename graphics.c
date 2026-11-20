#include<stdio.h>
#include<stdlib.h>
#include "graphics.h"

void initGraphics(graphicalContext* gc, const char* title)
{
    SDL_Init(SDL_INIT_VIDEO);
    char* titleCopy = (char*)malloc(strlen(title));
    removeExtension((char*)titleCopy);
    
    SDL_CreateWindowAndRenderer(title, DISPLAY_WIDTH * TILE_SIZE, DISPLAY_HEIGHT * TILE_SIZE, 0, &gc->window, &gc->renderer);
    free(titleCopy);
}

void updateDisplay(Chip8State* state, graphicalContext* gc)
{
    SDL_SetRenderDrawColor(gc->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gc->renderer);
            
    SDL_SetRenderDrawColor(gc->renderer, 255, 255, 255, 255);


    for (int i = 0; i < DISPLAY_HEIGHT; i++)
    {
        for (int j = 0; j < DISPLAY_WIDTH; j++)
        {
            int tile = i * DISPLAY_WIDTH + j;

            if(state->display[tile] == 0) continue;

            // printf("%d: %u\n", tile, color);

            SDL_FRect rect = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            SDL_RenderFillRect(gc->renderer, &rect);
        }
        
    }
    SDL_RenderPresent(gc->renderer);
}

void deleteGraphics(graphicalContext* gc)
{
    SDL_DestroyRenderer(gc->renderer);
    SDL_DestroyWindow(gc->window);
    
    SDL_Quit();
}

void removeExtension(char* str)
{
    char* dot = strrchr(str, '.');
    if(dot != NULL) *dot = '\0';
}