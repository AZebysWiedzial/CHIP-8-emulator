#include<Chip8State.h>
#include<string.h>

void initChip8State(Chip8State* state)
{
    memset(state, 0, sizeof(Chip8State));
    memcpy(state->memory[FONT_START], font, sizeof(font));
}

