#include <stdint.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "chip8State.h"
#include "graphics.h"
#include "debugger.h"

#define KEY_QUIT SDLK_ESCAPE
#define KEY_RESET SDLK_P

static const SDL_Scancode keymap[] = {
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};

static void handleSettingsInput(chip8State* state, debuggerState* debugger, SDL_KeyboardEvent* keyEvent)
{
    switch (keyEvent->key)
    {
        case SDLK_UP:
            debugger->selectedSetting = (debugger->selectedSetting - 1 + 3) % 3;
            break;
        case SDLK_DOWN:
            debugger->selectedSetting = (debugger->selectedSetting + 1) % 3;
            break;
        case SDLK_SPACE:
        case SDLK_RETURN:
            if (keyEvent->repeat) break;
            switch (debugger->selectedSetting)
            {
                case 0:
                    state->settings.shiftInPlace = !state->settings.shiftInPlace;
                    break;
                case 1:
                    state->settings.takeOffsetValueFromVX = !state->settings.takeOffsetValueFromVX;
                    break;
                case 2:
                    state->settings.useTempIValueForMemory = !state->settings.useTempIValueForMemory;
                    break;
            }
            break;
    }
}

void render(chip8State* state, graphicsContext* gc, debuggerState* debugger)
{
    SDL_SetRenderDrawColor(gc->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gc->renderer);
        
    SDL_SetRenderDrawColor(gc->renderer, 255, 255, 255, 255);

    updateDisplay(state, gc);
    if(debugger->panelStatus == SETTINGS)
        updateSettings(state, gc, debugger->selectedSetting);
    else if(debugger->panelStatus == DEBUG)
        updateDebugger(state, gc);

    SDL_RenderPresent(gc->renderer);
}

void handleEvents(chip8State* state, graphicsContext* gc, debuggerState* debugger, bool* quit, bool* isInDebugMode)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) 
    {
        switch(event.type) {
            case SDL_EVENT_QUIT:
                *quit = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                if(debugger->panelStatus == SETTINGS)
                    handleSettingsInput(state, debugger, &event.key);
                if(event.key.key == KEY_QUIT) *quit = true;
                if(event.key.key == KEY_RESET) 
                {
                    char fileNameCopy[strlen(state->fileName)];
                    strcpy(fileNameCopy, state->fileName);
                    resetChip8State(state, fileNameCopy);
                }
                if(event.key.key == SDLK_O)
                {
                    if(debugger->panelStatus != DEBUG) showDebugger(debugger, gc);
                    else hideDebugger(debugger, gc);
                }
                if(event.key.key == SDLK_I)
                {
                    if(debugger->panelStatus != SETTINGS) showSettings(debugger, gc);
                    else hideDebugger(debugger, gc);
                }
                if(*isInDebugMode)
                {
                    if(event.key.key == SDLK_RIGHT)
                    {
                        debugger->isNextStepRequested = true;
                    }
                    if(event.key.key == SDLK_LEFT)
                    {
                        if(state->delayTimer > 0) state->delayTimer--;
                        if(state->soundTimer > 0) state->soundTimer--;
                    }
                }
                if(debugger->panelStatus == DEBUG)
                {
                    if(event.key.key == SDLK_UP) state->addressToShow -= 2;
                    if(event.key.key == SDLK_DOWN) state->addressToShow += 2;
                    if(event.key.key == SDLK_U && state->stackAddressToShow < STACK_DEPTH - 4) state->stackAddressToShow++;
                    if(event.key.key == SDLK_J && state->stackAddressToShow > 0) state->stackAddressToShow--;
                }
                if(event.key.key == SDLK_L)
                {
                    *isInDebugMode = !(*isInDebugMode);
                    if(*isInDebugMode) showDebugger(debugger, gc);
                }
                break;
            }
    }
    const bool *keys = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < 16; i++)
        state->inputKeys[i] = keys[keymap[i]];
}

void loop(chip8State* state, graphicsContext* gc, debuggerState* debugger)
{
    bool quit = false;
    bool isInDebugMode = false;
    while(!quit)
    {
        int frameStart = SDL_GetTicks();

        handleEvents(state, gc, debugger, &quit, &isInDebugMode);

        // printInputKeys(state);
        
        if(isInDebugMode)
        {
            if(debugger->isNextStepRequested)
            {
                printf("Next\n");
                parseNextOpcode(state);
                debugger->isNextStepRequested = false;
            }
            render(state, gc, debugger);
            continue;
        }
        for (int i = 0; i < 10; i++) 
        {
            parseNextOpcode(state);
        }

        if(state->delayTimer > 0) state->delayTimer--;
        if(state->soundTimer > 0) state->soundTimer--;

        render(state, gc, debugger);

        Uint64 elapsed = SDL_GetTicks() - frameStart;
        if(elapsed < 16) SDL_Delay(16 - elapsed);
    }
}

int main(int argc, char* argv[])
{
    chip8State state;
    initChip8State(&state, modernSettings);    

    graphicsContext gc;
    initGraphics(&gc, argv[1]);

    debuggerState debugger;
    initDebuggerState(&debugger);
    
    loadFileToMemory(argv[1], &state);

    loop(&state, &gc, &debugger);

    deleteGraphics(&gc);
    deleteChip8State(&state);
    return 0;
}

