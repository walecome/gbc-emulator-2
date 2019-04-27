#pragma once

// System headers
#include <iostream>

// User headers
#include "InstructionDecoder.hh"
#include "Processor.hh"

// Terminal Input Hanndler
class TIH {
   public:
    TIH();

    bool getInput();

    void handle_input(InstructionDecoder &id, ptr<Processor> &p);

    void print_help();

   private:
    std::string input {};
};