#include <SDL3/SDL.h>
#include "debugger.h"

void initDebuggerState(debuggerState* debugger)
{
    debugger->panelStatus = CLOSED;
    debugger->isNextStepRequested = false;
    debugger->selectedSetting = 0;
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

void hideDebugger(debuggerState* debugger, graphicsContext* gc)
{
    debugger->panelStatus = CLOSED;
    hidePanel(gc);
}