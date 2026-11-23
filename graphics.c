#include<stdio.h>
#include<stdlib.h>
#include "graphics.h"
#include "debugger.h"

void initGraphics(graphicsContext* gc, const char* title)
{
    SDL_Init(SDL_INIT_VIDEO);
    char titleCopy[strlen(title)];
    removeExtension(titleCopy);
    
    SDL_CreateWindowAndRenderer(title, DISPLAY_WIDTH * TILE_SIZE, DISPLAY_HEIGHT * TILE_SIZE, 0, &gc->window, &gc->renderer);
}

void updateDisplay(chip8State* state, graphicsContext* gc)
{
    


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
}

void updateSettings(chip8State* state, graphicsContext* gc, int selectedSetting)
{
    const int panelX = DISPLAY_WIDTH * TILE_SIZE;
    const char* labels[] = {
        "Shift in place",
        "Offset from VX",
        "Use temp I for memory"
    };
    const bool values[] = {
        state->settings.shiftInPlace,
        state->settings.takeOffsetValueFromVX,
        state->settings.useTempIValueForMemory
    };

    SDL_SetRenderDrawColor(gc->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(gc->renderer, panelX + 8, 8, "SETTINGS");

    for (int i = 0; i < 3; i++)
    {
        const int rowY = 32 + i * 24;
        if (i == selectedSetting)
        {
            SDL_FRect selection = {panelX, (float)rowY - 4, DEBUGGER_WIDTH, 18};
            SDL_SetRenderDrawColor(gc->renderer, 45, 80, 135, 255);
            SDL_RenderFillRect(gc->renderer, &selection);
        }

        SDL_SetRenderDrawColor(gc->renderer, 255, 255, 255, 255);
        SDL_RenderDebugTextFormat(
            gc->renderer, panelX + 8, rowY, "%s %-24s %s",
            i == selectedSetting ? ">" : " ", labels[i], values[i] ? "ON" : "OFF");
    }

    SDL_RenderDebugText(gc->renderer, panelX + 8, 120, "UP/DOWN: Select");
    SDL_RenderDebugText(gc->renderer, panelX + 8, 136, "SPACE/ENTER: Toggle");
    SDL_RenderDebugText(gc->renderer, panelX + 8, 152, "I: Close settings");
    // printf("Settings\n");
}

void updateDebugger(chip8State* state, graphicsContext* gc)
{
    // printf("Debugger\n");
    SDL_SetRenderDrawColor(gc->renderer, 255, 255, 255, 255);

    for (int i = 0; i <= 0xF; i++)
    {
        int xOffset = (i < 8 ? 0 : 60);
        SDL_RenderDebugTextFormat(gc->renderer, DISPLAY_WIDTH * TILE_SIZE + 20 + xOffset, 10 + 20 * (i % 8), "v%X: %X", i, state->registers[i]);
    }
    
    SDL_RenderDebugTextFormat(gc->renderer, DISPLAY_WIDTH * TILE_SIZE + 20, 180, "PC: %X", state->PC);
    SDL_RenderDebugTextFormat(gc->renderer, DISPLAY_WIDTH * TILE_SIZE + 20, 200, "I: %X", state->I);
    SDL_RenderDebugTextFormat(gc->renderer, DISPLAY_WIDTH * TILE_SIZE + 20, 220, "SP: %X", state->stackPointer);

    SDL_RenderDebugTextFormat(gc->renderer, DISPLAY_WIDTH * TILE_SIZE + 20, 260, "Delay timer: %d", state->delayTimer);
    SDL_RenderDebugTextFormat(gc->renderer, DISPLAY_WIDTH * TILE_SIZE + 20, 280, "Sound timer: %d", state->soundTimer);
}

void deleteGraphics(graphicsContext* gc)
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

void showPanel(graphicsContext* gc)
{
    int newWidth = (DISPLAY_WIDTH * TILE_SIZE) + DEBUGGER_WIDTH;
    int height = DISPLAY_HEIGHT * TILE_SIZE;
    
    SDL_SetWindowSize(gc->window, newWidth, height);
}

void hidePanel(graphicsContext* gc)
{
    int newWidth = DISPLAY_WIDTH * TILE_SIZE;
    int height = DISPLAY_HEIGHT * TILE_SIZE;

    SDL_SetWindowSize(gc->window, newWidth, height);
}