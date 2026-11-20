#include <SDL3/SDL.h>
#include "debugger.h"

void initDebuggerState(debuggerState* debugger)
{
    debugger->panelStatus = CLOSED;
    debugger->isNextStepRequested = false;
}



void showSettings(debuggerState* debugger, graphicsContext* gc)
{
    debugger->panelStatus = SETTINGS;
    showPanel(gc);
}

void showDebugger(debuggerState* debugger, graphicsContext* gc)
{
    debugger->panelStatus = DEBUG;
    showPanel(gc);
}

void hidePanel(debuggerState* debugger, graphicsContext* gc)
{
    debugger->panelStatus = CLOSED;

    int newWidth = DISPLAY_WIDTH * TILE_SIZE;
    int height = DISPLAY_HEIGHT * TILE_SIZE;
    SDL_SetWindowSize(gc->window, newWidth, height);

}