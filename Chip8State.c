#include "Chip8State.h"
#include<string.h>

void initChip8State(Chip8State* state)
{
    memset(state, 0, sizeof(*state));
    memcpy(&state->memory[FONT_START], font, sizeof(font));

    state->display[2025] = 1;
}

