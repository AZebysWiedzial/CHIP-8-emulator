#include "Chip8State.h"
#include<string.h>
#include<stdio.h>

void initChip8State(Chip8State* state)
{
    memset(state, 0, sizeof(*state));
    memcpy(&state->memory[FONT_START], font, sizeof(font));
}

void parseNextOpcode(Chip8State* state)
{

}

void loadFileToMemory(const char* fileName, Chip8State* state)
{
    FILE* file = fopen(fileName, "rb");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // printf("File size: %ld\n", fileSize);

    fread(&state->memory[MEMORY_START], 1, (size_t)fileSize, file);

    fclose(file);
}
