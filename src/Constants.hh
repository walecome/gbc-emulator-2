/*
    Useful contants are defined in this file.
*/
#pragma once
#include <stdint.h>

using opcode_t = uint8_t;
using byte_t = uint8_t;
using opcode_function = void (*)();
constexpr int NUMBER_OF_INSTRUCTIONS = 256;

// Register value constants

using register8_t = uint8_t;
using register16_t = uint16_t;

// ROM specific constants

// Starting position of program counter;
constexpr register16_t PC_START = 0x100;

// Memory
constexpr register16_t RAM_MAX_SIZE = 0xFFFF;
constexpr register16_t RAM_DATA_OFFSET = 0xFF00;