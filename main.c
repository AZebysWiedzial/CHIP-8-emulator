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



void handleEvents(chip8State* state, graphicsContext* gc, debuggerState* debugger, bool* quit)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) 
    {
        switch(event.type) {
            case SDL_EVENT_QUIT:
                *quit = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                if(event.key.key == KEY_QUIT) *quit = true;
                if(event.key.key == KEY_RESET) 
                {
                    char fileNameCopy[strlen(state->fileName)];
                    strcpy(fileNameCopy, state->fileName);
                    resetChip8State(state, fileNameCopy);
                }
                if(event.key.key == SDLK_O)
                {
                    if(debugger->panelStatus == CLOSED) showDebugger(debugger, gc);
                    else hidePanel(debugger, gc);
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
    while(!quit)
    {
        int frameStart = SDL_GetTicks();

        handleEvents(state, gc, debugger, &quit);

        // printInputKeys(state);

        for (int i = 0; i < 10; i++) 
        {

            parseNextOpcode(state);
        }

        if(state->delayTimer > 0) state->delayTimer--;
        if(state->soundTimer > 0) state->soundTimer--;

        updateDisplay(state, gc);

        Uint64 elapsed = SDL_GetTicks() - frameStart;
        if(elapsed < 16) SDL_Delay(16 - elapsed);
    }
}




int main(int argc, char* argv[])
{
    chip8State state;
    initChip8State(&state);    

    graphicsContext gc;
    initGraphics(&gc, argv[1]);

    debuggerState debugger;
    
    loadFileToMemory(argv[1], &state);
    


    loop(&state, &gc, &debugger);

    deleteGraphics(&gc);
    deleteChip8State(&state);
    return 0;
}

