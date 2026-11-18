#include <stdint.h>
#include <SDL3/SDL.h>

#include "Chip8State.h"
#include "graphics.h"

#define KEY_QUIT SDLK_ESCAPE

void handleEvents(Chip8State* state, bool* quit)
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
                break;
            }
    }
}

void loop(Chip8State* state, graphicalContext* gc)
{
    bool quit = false;
    while(!quit)
    {
        int frameStart = SDL_GetTicks();

        handleEvents(state, &quit);

        if(state->delayTimer > 0) state->delayTimer--;
        if(state->soundTimer > 0) state->soundTimer--;

        updateDisplay(state, gc);

        Uint64 elapsed = SDL_GetTicks() - frameStart;
        if(elapsed < 16) SDL_Delay(16 - elapsed);
    }
}


int main()
{
    Chip8State state;
    initChip8State(&state);

    graphicalContext gc;
    initGraphics(&gc);
    updateDisplay(&state, &gc);

    loop(&state, &gc);

    deleteGraphics(&gc);
    return 0;
}

