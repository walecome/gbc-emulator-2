#pragma once
#include <iterator>
#include <fstream>
#include <vector>
#include "Constants.hh"

class Processor
{
  public:
    Processor() = default;

    /**
    Reads instructions (and data) into the program memory vector given
    binary filename
    */
    void readInstructions(const char *filename);

  private:
    unsigned int stack_pointer{0};
    unsigned int program_counter{0};
    std::vector<opcode_t> program_memory;
    opcode_function opcode_translations[NUMBER_OF_INSTRUCTIONS];
};