#ifndef CHIP8STATE_H
#define CHIP8STATE_H

#include<stdint.h>

#define REGISTER_COUNT 16
#define MEMORY_SIZE 4096
#define STACK_DEPTH 16
#define KEY_COUNT 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE DISPLAY_WIDTH*DISPLAY_HEIGHT
#define FONT_START 0x50
#define FONT_SIZE 5
#define MEMORY_START 0x200

#define USE_MODERN_INSTRUCTIONS 1

#define MAX_FILE_NAME_LENGTH 30

static const uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static const uint8_t keyOrder[KEY_COUNT] = {
    0x1, 0x2, 0x3, 0xC,
    0x4, 0x5, 0x6, 0xD,
    0x7, 0x8, 0x9, 0xE,
    0xA, 0x0, 0xB, 0xF
};

typedef struct Chip8State {
    uint8_t registers[REGISTER_COUNT]; 
    uint8_t memory[MEMORY_SIZE];

    uint16_t I;
    uint16_t PC;
    uint16_t stack[STACK_DEPTH];

    uint8_t stackPointer;

    uint8_t delayTimer;
    uint8_t soundTimer;

    uint8_t inputKeys[KEY_COUNT];
    int waitingKey;

    uint32_t display[DISPLAY_SIZE];

    uint16_t opcode;

    char fileName[MAX_FILE_NAME_LENGTH];
} Chip8State;

void initChip8State(Chip8State* state);
void parseNextOpcode(Chip8State* state);
void loadFileToMemory(const char* fileName, Chip8State* state);
void pushPC(Chip8State* state);
void popPC(Chip8State* state);
void resetInputKeys(Chip8State* state);

void resetChip8State(Chip8State* state, char* fileName);
void deleteChip8State(Chip8State* state);

void clearDisplay(Chip8State* state);
void drawSprite(Chip8State* state, uint8_t x, uint8_t y, uint8_t height);



void printInputKeys(Chip8State* state);
#endif