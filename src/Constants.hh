/*
    Useful contants are defined in this file.
*/
#pragma once
#include <stdint.h>

using opcode_t = uint8_t;
using byte_t = uint8_t;
using opcode_function = void (*)();
constexpr int NUMBER_OF_INSTRUCTIONS = 256;

// Registers
using register8_t = uint8_t;
using register16_t = uint16_t;
