#pragma once

// System headers
#include <array>
#include <iostream>
#include <vector>

// User headers
#include "InstructionDecoder.hh"
#include "Processor.hh"
#include "Utility.hh"

// Terminal Input Handler
class TIH {
   public:
    TIH();

    bool getInput();

    void handle_input(InstructionDecoder &id, ptr<Processor> &p);

    void print_help();

    bool at_breakpoint(register16_t addr);

   private:
    std::string input {};
    std::array<bool, PC_MAX_SIZE> breakpoints {};
};