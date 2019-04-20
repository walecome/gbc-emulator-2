#pragma once

// System headers
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

// User headers
#include "Constants.hh"
#include "InstructionDecoder.hh"
#include "Memory.hh"
#include "Register16bit.hh"
#include "Register8bit.hh"

class Processor {
   public:
    Processor();
    ~Processor();

    // Weffc++
    Processor(const Processor &) = delete;
    void operator=(const Processor &) = delete;

    // Utils
    void readInstructions(const char *filename);
    void printStack(register16_t start, register16_t end);
    void print();

    // Opcode helpers
    opcode_t fetchInstruction();
    byte_t getCurrentData();
    void loadRegister(Register8bit *reg);
    void loadIntoMemory(Register16bit *address_reg, Register8bit *data_reg);
    void loadIntoMemory(Register16bit *address_reg, byte_t value);
    void loadFromMemory(Register8bit *data_reg, Register16bit *address_reg);
    byte_t loadFromMemory(Register16bit *address_reg);
    void incrementRegister(Register8bit *reg);
    void decrementRegister(Register8bit *reg);
    void copyRegister(Register8bit *destination, Register8bit *source);
    void addRegisters(Register8bit *destination, Register8bit *source);
    void addRegisters(Register16bit *destination, Register16bit *source);
    void subRegisters(Register8bit *source);
    void addWithCarry(Register8bit *destination, Register8bit *source);
    void subWithCarry(Register8bit *source);
    void andRegisters(Register8bit *source);
    void xorRegisters(Register8bit *source);
    void orRegisters(Register8bit *source);
    void cmpRegisters(Register8bit *source);
    void pushStack(Register16bit *source);
    void popStack(Register16bit *destination);
    void popStackAF();
    void performJump();
    void jumpIm16bit();
    void rlcRegister(Register8bit *source);
    void rrcRegister(Register8bit *source);
    void rlRegister(Register8bit *source);
    void rrRegister(Register8bit *source);
    void slaRegister(Register8bit *source);
    void sraRegister(Register8bit *source);
    void swapNibbles(Register8bit *reg);
    void srlRegister(Register8bit *reg);
    void testBit(int b, Register8bit *reg);
    void testBit(int b, Register16bit *reg);
    void resetBit(int b, Register8bit *reg);
    void resetBit(int b, Register16bit *address_reg);
    void setBit(int b, Register8bit *reg);
    void setBit(int b, Register16bit *reg);

    // Handle flags according to operation results
    template <class T>
    void checkFlagZ(T result);
    void checkFlagC(int result);
    void checkFlagH(register8_t result);

    // Getters for 8bit registers
    Register8bit *getA() { return A; }
    Register8bit *getB() { return B; }
    Register8bit *getC() { return C; }
    Register8bit *getD() { return D; }
    Register8bit *getE() { return E; }
    Register8bit *getF() { return F; }
    Register8bit *getH() { return H; }
    Register8bit *getL() { return L; }

    register8_t getValueA() { return A->getValue(); }
    register8_t getValueB() { return B->getValue(); }
    register8_t getValueC() { return C->getValue(); }
    register8_t getValueD() { return D->getValue(); }
    register8_t getValueE() { return E->getValue(); }
    register8_t getValueF() { return F->getValue(); }
    register8_t getValueH() { return H->getValue(); }
    register8_t getValueL() { return L->getValue(); }

    // Getters for 16bit registers
    Register16bit *getAF() { return AF; }
    Register16bit *getBC() { return BC; }
    Register16bit *getDE() { return DE; }
    Register16bit *getHL() { return HL; }

    register16_t getValueAF() { return AF->getValue(); }
    register16_t getValueBC() { return BC->getValue(); }
    register16_t getValueDE() { return DE->getValue(); }
    register16_t getValueHL() { return HL->getValue(); }

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
    register16_t getValuePC() { return program_counter->getValue(); }
    void setValuePC(register16_t value) { program_counter->setValue(value); }

    // SP
    register16_t getValueSP() { return stack_pointer->getValue(); }
    void setValueSP(register16_t value) { stack_pointer->setValue(value); }

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

    Memory *getRAM() { return ram; }
    Memory *getStack() { return stack; }
    Memory *getProgramMem() { return program_memory; }

   private:
    Memory *program_memory { new Memory(PC_MAX_SIZE) };
    opcode_function opcode_function_table[NUMBER_OF_INSTRUCTIONS];
    Memory *ram { new Memory(RAM_MAX_SIZE) };
    Memory *stack { new Memory(RAM_MAX_SIZE) };

    // Handle stack pointer as 16 bit register
    Register16bit *stack_pointer { new Register16bit("SP") };

    // Handle program counter as 16 bit register
    Register16bit *program_counter { new Register16bit("PC") };

    // Registers
    Register8bit *A { new Register8bit("A") };
    Register8bit *B { new Register8bit("B") };
    Register8bit *C { new Register8bit("C") };
    Register8bit *D { new Register8bit("D") };
    Register8bit *E { new Register8bit("E") };
    Register8bit *F { new Register8bit("F") };
    Register8bit *H { new Register8bit("H") };
    Register8bit *L { new Register8bit("L") };

    Register16bit *AF { new Register16bit("AF", A, F) };
    Register16bit *BC { new Register16bit("BC", B, C) };
    Register16bit *DE { new Register16bit("DE", D, E) };
    Register16bit *HL { new Register16bit("HL", H, L) };

    // Flags
    bool flagC { false };
    bool flagH { false };
    bool flagN { false };
    bool flagZ { false };

    // OPCode definitions
};