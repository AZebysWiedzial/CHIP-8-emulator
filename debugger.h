#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "chip8State.h"
#include "graphics.h"


typedef enum 
{
    CLOSED, SETTINGS, DEBUG
} panelStatus;

typedef struct debuggerState
{
    panelStatus panelStatus;
    bool isNextStepRequested;
    int selectedSetting;
} debuggerState;

void initDebuggerState(debuggerState* debugger);


void showSettings(debuggerState* debugger, graphicsContext* gc);
void showDebugger(debuggerState* debugger, graphicsContext* gc);
void hideDebugger(debuggerState* debugger, graphicsContext* gc);

#endif