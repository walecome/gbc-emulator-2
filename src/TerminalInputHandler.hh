#pragma once

// System headers
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

// Library headers
#include <curses.h>

// User headers
#include "InstructionDecoder.hh"
#include "Processor.hh"
#include "Utility.hh"

// Terminal Input Handler
class TIH {
   public:
    TIH(ptr<InstructionDecoder> instructionDecoder, ptr<Processor> cpu);
    ~TIH();
    TIH(const TIH &) = delete;
    auto operator=(const TIH &) = delete;

    bool initCurses();
    void cursesLoop();
    void clear();

    void render8bitRegister(const CPU_info &info);
    void render16bitRegister(const CPU_info &info);
    void renderInfo(const CPU_info &info);
    void renderCPUInfo();

    bool getInput();

    void handle_input(InstructionDecoder &id, ptr<Processor> &p);

    void print_help();

    bool at_breakpoint(register16_t addr);

   private:
    std::string input {};
    std::array<bool, PC_MAX_SIZE> breakpoints {};

    ptr<InstructionDecoder> instructionDecoder;
    ptr<Processor> cpu;

    WINDOW *mainwin;
};