#pragma once

// System headers
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

// User headers
#include "Constants.hh"
#include "Memory.hh"
#include "Register16bit.hh"
#include "Register8bit.hh"

/**
    This class is basically just to keep track of the state of the emulated CPU.
    It keeps the registers, memory and so on as members, but all modifications
    are done (using opcodes) using the instruction decoder.
*/
class Processor {
   public:
    Processor();

    // Weffc++
    Processor(const Processor &) = delete;
    void operator=(const Processor &) = delete;

    /**
        Reads instructions (and data) into the program memory vector given
        binary filename. Sets program counter to PC_START (0x100) after read.
    */
    void readInstructions(const std::string &filename, bool verbose = false);

    /**
        Prints the stack content in a radius around the stack pointer.
    */
    void printStack(int radius = 5);

    /**
        Prints the program memory content in a radius around the program
       counter.
    */
    void printProgramMemory(int radius = 5);
    void dump();

    /**
     *  Returns the instruction currently pointed at by the program counter.
     */
    opcode_t fetchInstruction();

    /**
     * Adds nr_cycles of cycles to machine_cycles. clock_cycles are also added
     * with machine_cycles * 4.
     */
    void add_machine_cycles(unsigned int nr_cycles) {
        machine_cycles += nr_cycles;
        clock_cycles += nr_cycles * 4;
    }

    // Handle flags according to operation results
    void checkFlagZ(register8_t result);
    void checkFlagC(int result);
    void checkFlagH(register8_t result);

    // Getters for flags
    bool getFlagC();
    bool getFlagH();
    bool getFlagN();
    bool getFlagZ();

    // Setters for flags
    void setFlagC();
    void setFlagH();
    void setFlagN();
    void setFlagZ();
    void resetFlagC();
    void resetFlagH();
    void resetFlagN();
    void resetFlagZ();

    ptr<Memory> program_memory { std::make_shared<Memory>(PC_MAX_SIZE) };
    ptr<Memory> ram { std::make_shared<Memory>(RAM_MAX_SIZE) };
    ptr<Memory> stack { std::make_shared<Memory>(RAM_MAX_SIZE) };

    // Handle stack pointer as 16 bit register
    ptr<Register16bit> SP { std::make_shared<Register16bit>("SP") };

    // Handle program counter as 16 bit register
    ptr<Register16bit> PC { std::make_shared<Register16bit>("PC") };

    // Registers
    ptr<Register8bit> A { std::make_shared<Register8bit>("A") };
    ptr<Register8bit> B { std::make_shared<Register8bit>("B") };
    ptr<Register8bit> C { std::make_shared<Register8bit>("C") };
    ptr<Register8bit> D { std::make_shared<Register8bit>("D") };
    ptr<Register8bit> E { std::make_shared<Register8bit>("E") };
    ptr<Register8bit> F { std::make_shared<Register8bit>("F") };
    ptr<Register8bit> H { std::make_shared<Register8bit>("H") };
    ptr<Register8bit> L { std::make_shared<Register8bit>("L") };

    ptr<Register16bit> AF { std::make_shared<Register16bit>("AF", A, F) };
    ptr<Register16bit> BC { std::make_shared<Register16bit>("BC", B, C) };
    ptr<Register16bit> DE { std::make_shared<Register16bit>("DE", D, E) };
    ptr<Register16bit> HL { std::make_shared<Register16bit>("HL", H, L) };

    // Clock and machine cycles
    long unsigned int clock_cycles { 0 };
    long unsigned int machine_cycles { 0 };

    // Store raw ROM data
    std::vector<opcode_t> rom_data {};

    // Interrupts
    bool interrupts_enabled { true };

    /**
     *   Return data from the interrupt register.
     */
    register8_t &get_interrupt_reg();

    /**
     *   Set data in the interrupt register.
     */
    void set_interrupt_reg(byte_t data);
};