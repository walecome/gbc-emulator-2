#pragma once

// System headers
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

    // Utils
    /**
        Reads instructions (and data) into the program memory vector given
        binary filename
    */
    void readInstructions(const char *filename);

    /**
        Prints the stack content in a radius from the stack pointer.
    */
    void printStack(int radius);
    void dump();

    // Handle flags according to operation results
    void checkFlagZ(register8_t result);
    void checkFlagC(int result);
    void checkFlagH(register8_t result);

    // Getters for 8bit registers
    ptr<Register8bit> getA() const { return A; }
    ptr<Register8bit> getB() const { return B; }
    ptr<Register8bit> getC() const { return C; }
    ptr<Register8bit> getD() const { return D; }
    ptr<Register8bit> getE() const { return E; }
    ptr<Register8bit> getF() const { return F; }
    ptr<Register8bit> getH() const { return H; }
    ptr<Register8bit> getL() const { return L; }

    register8_t getValueA() const { return A->getValue(); }
    register8_t getValueB() const { return B->getValue(); }
    register8_t getValueC() const { return C->getValue(); }
    register8_t getValueD() const { return D->getValue(); }
    register8_t getValueE() const { return E->getValue(); }
    register8_t getValueF() const { return F->getValue(); }
    register8_t getValueH() const { return H->getValue(); }
    register8_t getValueL() const { return L->getValue(); }

    // Getters for 16bit registers
    ptr<Register16bit> getAF() const { return AF; }
    ptr<Register16bit> getBC() const { return BC; }
    ptr<Register16bit> getDE() const { return DE; }
    ptr<Register16bit> getHL() const { return HL; }

    register16_t getValueAF() const { return AF->getValue(); }
    register16_t getValueBC() const { return BC->getValue(); }
    register16_t getValueDE() const { return DE->getValue(); }
    register16_t getValueHL() const { return HL->getValue(); }

    // Setters for 8bit registers
    void setValueA(register8_t value) { A->setValue(value); }
    void setValueB(register8_t value) { B->setValue(value); }
    void setValueC(register8_t value) { C->setValue(value); }
    void setValueD(register8_t value) { D->setValue(value); }
    void setValueE(register8_t value) { E->setValue(value); }
    void setValueF(register8_t value) { F->setValue(value); }
    void setValueH(register8_t value) { H->setValue(value); }
    void setValueL(register8_t value) { L->setValue(value); }

    // Setters for 16bit registers
    void setValueAF(register16_t value) { AF->setValue(value); }
    void setValueBC(register16_t value) { BC->setValue(value); }
    void setValueDE(register16_t value) { DE->setValue(value); }
    void setValueHL(register16_t value) { HL->setValue(value); }

    // PC
    register16_t getValuePC() { return PC->getValue(); }
    void setValuePC(register16_t value) { PC->setValue(value); }

    // SP
    register16_t getValueSP() { return SP->getValue(); }
    void setValueSP(register16_t value) { SP->setValue(value); }

    // Getters for flags
    bool getFlagC() { return flagC; }
    bool getFlagH() { return flagH; }
    bool getFlagN() { return flagN; }
    bool getFlagZ() { return flagZ; }

    // Setters for flags
    void setFlagC(bool value) { flagC = value; }
    void setFlagH(bool value) { flagH = value; }
    void setFlagN(bool value) { flagN = value; }
    void setFlagZ(bool value) { flagZ = value; }

    ptr<Memory> getRAM() { return ram; }
    ptr<Memory> getStack() { return stack; }
    ptr<Memory> getProgramMem() { return program_memory; }

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

    // Flags
    bool flagC { false };
    bool flagH { false };
    bool flagN { false };
    bool flagZ { false };
};