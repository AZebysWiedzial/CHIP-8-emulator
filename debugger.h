#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "graphics.h"
#include "chip8State.h"
// #include <SDL3/SDL.h>


typedef enum 
{
    CLOSED, SETTINGS, DEBUG
} panelStatus;

typedef struct debuggerState
{
    panelStatus panelStatus;
    bool isNextStepRequested;
} debuggerState;

void initDebuggerState(debuggerState* debugger);


void showSettings(debuggerState* debugger, graphicsContext* gc);
void showDebugger(debuggerState* debugger, graphicsContext* gc);
void hidePanel(debuggerState* debugger, graphicsContext* gc);

#endif