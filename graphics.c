#include<stdio.h>
#include "graphics.h"

void initGraphics(graphicalContext* gc)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_TITLE, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE, 0, &gc->window, &gc->renderer);
}

void updateDisplay(Chip8State* state, graphicalContext* gc)
{
    SDL_RenderClear(gc->renderer);

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            int tile = i * HEIGHT + j;

            Uint8 color = (state->display[tile] != 0) ? 255 : 0;

            // printf("%d: %u\n", tile, color);

            SDL_FRect rect = {i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            SDL_SetRenderDrawColor(gc->renderer, color, color, color, 255);
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