#include "Chip8State.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void initChip8State(chip8State* state)
{
    memset(state, 0, sizeof(*state));
    memcpy(&state->memory[FONT_START], font, sizeof(font));
    state->PC = MEMORY_START;
    state->waitingKey = -1;


    srand(time(NULL));
}

void parseNextOpcode(chip8State* state)
{
    state->opcode = (state->memory[state->PC] << 8) | state->memory[state->PC + 1];
    // printf("%u\n", state->opcode);

    state->PC += 2;

    uint8_t firstNibble = (state->opcode >> 12) & 0xF;
    uint8_t secondNibble = (state->opcode >> 8) & 0xF;
    uint8_t thirdNibble = (state->opcode >> 4) & 0xF;
    uint8_t fourthNibble = state->opcode & 0xF;
    uint8_t secondByte = state->opcode & 0xFF;
    uint16_t secondThirdFourthNibbles = state->opcode & 0xFFF;
    
    switch(firstNibble)
    {
        case 0x0:
            switch(secondThirdFourthNibbles)
            {
                case 0x0E0:
                    clearDisplay(state);
                    break;
                case 0x0EE:
                    popPC(state);
                    break;
            }
            break;
        case 0x1:
            state->PC = secondThirdFourthNibbles;
            break;
        case 0x2:
            pushPC(state);
            state->PC = secondThirdFourthNibbles;
            break;
        //jump instructions
        case 0x3:
            if(state->registers[secondNibble] == secondByte) state->PC += 2;
            break;
        case 0x4:
            if(state->registers[secondNibble] != secondByte) state->PC += 2;
            break;
        case 0x5:
            if(state->registers[secondNibble] == state->registers[thirdNibble]) state->PC += 2;
            break;
        case 0x9:
            if(state->registers[secondNibble] != state->registers[thirdNibble]) state->PC += 2;
            break;
        case 0x6:
            state->registers[secondNibble] = secondByte;
            break;
        case 0x7:
            state->registers[secondNibble] += secondByte;
            break;
        case 0x8:
            switch (fourthNibble)
            {
                case 0x0:
                    state->registers[secondNibble] = state->registers[thirdNibble];
                    break;
                case 0x1:
                    state->registers[secondNibble] |= state->registers[thirdNibble];
                    break;
                case 0x2:
                    state->registers[secondNibble] &= state->registers[thirdNibble];
                    break;
                case 0x3:
                    state->registers[secondNibble] ^= state->registers[thirdNibble];
                    break;
                case 0x4:
                    uint16_t sum = state->registers[secondNibble] + state->registers[thirdNibble];
                    state->registers[secondNibble] = sum;
                    if(sum > 0xFF) state->registers[0xF] = 1;
                    else state->registers[0xF] = 0;
                    break;
                case 0x5:
                {
                    bool setFlag = (state->registers[secondNibble] >= state->registers[thirdNibble]);
                    state->registers[secondNibble] -= state->registers[thirdNibble];
                    state->registers[0xF] = setFlag;
                    break;
                }
                    
                case 0x7:
                {
                    bool setFlag = (state->registers[thirdNibble] >= state->registers[secondNibble]);
                    state->registers[secondNibble] = state->registers[thirdNibble] - state->registers[secondNibble];
                    state->registers[0xF] = setFlag;
                    break;
                }
                    
                case 0x6:
                    if(!USE_MODERN_INSTRUCTIONS) state->registers[secondNibble] = state->registers[thirdNibble];
                    uint8_t shiftedBit = state->registers[secondNibble] & 0x1;
                    state->registers[secondNibble] >>= 1;
                    state->registers[0xF] = shiftedBit;
                    break;
                case 0xE:
                    if(!USE_MODERN_INSTRUCTIONS) state->registers[secondNibble] = state->registers[thirdNibble];
                    shiftedBit = (state->registers[secondNibble] >> 7) & 0x1;
                    state->registers[secondNibble] <<= 1;
                    state->registers[0xF] = shiftedBit;
                    break;
                default:
                    break;
            }
            break;
        case 0xA:
            state->I = secondThirdFourthNibbles;
            break;
        case 0xB:
            if(!USE_MODERN_INSTRUCTIONS) state->PC = state->registers[0] + secondThirdFourthNibbles;
            else state->PC = state->registers[secondNibble] + secondThirdFourthNibbles;
            break;
        case 0xC:
            state->registers[secondNibble] = (rand() & 0xFF) & secondByte;
            break;
        case 0xD:
            //check if the drawing works
            // printf("Drawing sprite at (%u, %u)\n", state->registers[secondNibble], state->registers[thirdNibble]);
            drawSprite(state, state->registers[secondNibble], state->registers[thirdNibble], fourthNibble);
            break;
        case 0xE:
            switch (secondByte)
            {
                case 0x9E:
                    if(state->inputKeys[state->registers[secondNibble]]) state->PC += 2;
                    break;
                case 0xA1:
                    if(!state->inputKeys[state->registers[secondNibble]]) state->PC += 2;
                    break;
                default:
                    break;
            }
            break;
        case 0xF:
            switch (secondByte)
            {
                case 0x07:
                    state->registers[secondNibble] = state->delayTimer;
                    break;
                case 0x15:
                    state->delayTimer = state->registers[secondNibble];
                    break;
                case 0x18:
                    state->soundTimer = state->registers[secondNibble];
                    break;
                case 0x1E:
                    state->I += state->registers[secondNibble];
                    if(state->I > 0xFFF) state->registers[0xF] = 1;
                    break;
                case 0x0A:
                    if(state->waitingKey == -1)
                    {
                        for (int i = 0; i <= 0xF; i++)
                        {
                            if(state->inputKeys[i])
                            {
                                state->waitingKey = i;
                                break;
                            }
                        }
                        state->PC -= 2;
                    }
                    else if(state->inputKeys[state->waitingKey]) state->PC -= 2;
                    else 
                    {
                        state->registers[secondNibble] = state->waitingKey;
                        state->waitingKey = -1;
                    }
                    break;
                case 0x29:
                    state->I = FONT_START + ((state->registers[secondNibble] & 0xF) * FONT_SIZE);
                    break;
                case 0x33:
                {
                    uint16_t numberToConvert = state->registers[secondNibble];
                    for (int i = 2; i >= 0; i--)
                    {
                        state->memory[state->I + i] = numberToConvert % 10;
                        numberToConvert /= 10;
                    }
                    break;
                }
                case 0x55:
                    for (int i = 0; i <= secondNibble; i++)
                    {
                        state->memory[state->I + i] = state->registers[i];
                    }
                    if(!USE_MODERN_INSTRUCTIONS) state->I += secondNibble + 1;
                    break;
                case 0x65:
                    for (int i = 0; i <= state->registers[secondNibble]; i++)
                    {
                        state->registers[i] = state->memory[state->I + i];
                    }
                    if(!USE_MODERN_INSTRUCTIONS) state->I += state->registers[secondNibble] + 1;
                    break;
                    
                default:
                    break;
            }
            break;
        
    }
}

void pushPC(chip8State* state)
{
    state->stackPointer++;
    state->stack[state->stackPointer] = state->PC;
}

void popPC(chip8State* state)
{
    state->PC = state->stack[state->stackPointer];
    state->stackPointer--;
}

void resetInputKeys(chip8State* state)
{
    for (int i = 0; i < KEY_COUNT; i++)
    {
        state->inputKeys[i] = 0;
    }
}

void loadFileToMemory(const char* fileName, chip8State* state)
{
    strcpy((char*)(state->fileName), (char*)fileName);

    FILE* file = fopen(fileName, "rb");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // printf("File size: %ld\n", fileSize);

    fread(&state->memory[MEMORY_START], 1, (size_t)fileSize, file);

    fclose(file);

    printf("File name: %s\n", state->fileName);
}

void clearDisplay(chip8State* state)
{
    for (int i = 0; i < DISPLAY_SIZE; i++)
    {
        state->display[i] = 0;
    }
}

void drawSprite(chip8State* state, uint8_t x, uint8_t y, uint8_t height)
{
    uint8_t startX = x % DISPLAY_WIDTH;
    uint8_t startY = y % DISPLAY_HEIGHT;

    state->registers[0xF] = 0;

    for (int i = 0; i < height; i++)
    {
        uint8_t currByte = state->memory[state->I + i];
        uint8_t y = startY + i;
        if(y >= DISPLAY_HEIGHT) break;
        for (int j = 7; j >= 0; j--)
        {
            uint8_t currBit = (currByte >> j) & 0x1;
            uint8_t x = startX + (7 - j);
            if(x >= DISPLAY_WIDTH) continue;

            if(currBit)
            {
                uint16_t index = y * DISPLAY_WIDTH + x;
                if(state->display[index]) state->registers[0xF] = 1;
                state->display[index] ^= 0xFFFF;
            }
        }
        
    }
}

void printInputKeys(chip8State* state)
{
    printf("Input keys:\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int index = i * 4 + j;
            printf("%u ", state->inputKeys[keyOrder[index]]);
        }
        printf("\n");
    }
}

void resetChip8State(chip8State* state, char* fileName)
{
    initChip8State(state);
    loadFileToMemory(fileName, state);
}

void deleteChip8State(chip8State* state)
{
    
}