/*
    Useful constants and type declarations are defined in this file.
*/
#pragma once
#include <stdint.h>
#include <functional>
#include <memory>

using opcode_t = uint8_t;
using byte_t = uint8_t;
using opcode_function = std::function<void()>;
constexpr const int NUMBER_OF_INSTRUCTIONS = 256;

constexpr const byte_t LSB_8BIT = 0x01;
constexpr const byte_t MSB_8BIT = 0x80;

// Register value constants

using register8_t = uint8_t;
using register16_t = uint16_t;

// ROM specific constants

// Starting position of program counter;
constexpr const register16_t PC_START = 0x100;

// Starting position of stack pointer
constexpr const register16_t SP_START = 0xFFFF;

// Memory
constexpr const register16_t PC_MAX_SIZE = 0xFFFF;
constexpr const register16_t RAM_MAX_SIZE = 0xFFFF;
constexpr const register16_t RAM_DATA_OFFSET = 0xFF00;

template <typename T>
using ptr = std::shared_ptr<T>;