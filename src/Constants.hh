/*
    Useful contants are defined in this file.
*/
#pragma once

using opcode_t = unsigned char;
using byte_t = unsigned char;
using opcode_function = void (*)();

constexpr int NUMBER_OF_INSTRUCTIONS = 256;