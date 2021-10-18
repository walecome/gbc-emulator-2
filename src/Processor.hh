#pragma once
#include <iterator>
#include <fstream>
#include <vector>
#include "Constants.hh"
#include "Register8bit.hh"
#include "Register16bit.hh"

class Processor
{
  public:
    Processor() = default;
    ~Processor();

    /**
    Reads instructions (and data) into the program memory vector given
    binary filename
    */
    void readInstructions(const char *filename);

    // Getters for 8bit registers
    register8_t getA();
    register8_t getB();
    register8_t getC();
    register8_t getD();
    register8_t getE();
    register8_t getF();
    register8_t getH();
    register8_t getL();

    // Getters for 16bit registers
    register16_t getAF();
    register16_t getBC();
    register16_t getDE();
    register16_t getHL();

    // Setters for 8bit registers
    void setA(register8_t value);
    void setB(register8_t value);
    void setC(register8_t value);
    void setD(register8_t value);
    void setE(register8_t value);
    void setF(register8_t value);
    void setH(register8_t value);
    void setL(register8_t value);

    // Setters for 16bit registers
    void setAF(register16_t value);
    void setBC(register16_t value);
    void setDE(register16_t value);
    void setHL(register16_t value);

  private:
    unsigned int stack_pointer{0};
    unsigned int program_counter{0};
    std::vector<opcode_t> program_memory;
    std::vector<byte_t> program_stack;
    opcode_function opcode_function_table[NUMBER_OF_INSTRUCTIONS];

    // Registers
    Register8bit *A = new Register8bit("A");
    Register8bit *B = new Register8bit("B");
    Register8bit *C = new Register8bit("C");
    Register8bit *D = new Register8bit("A");
    Register8bit *E = new Register8bit("E");
    Register8bit *F = new Register8bit("F");
    Register8bit *H = new Register8bit("H");
    Register8bit *L = new Register8bit("L");

    Register16bit *AF = new Register16bit("AF", A, F);
    Register16bit *BC = new Register16bit("BC", B, C);
    Register16bit *DE = new Register16bit("DE", D, E);
    Register16bit *HL = new Register16bit("HL", H, L);

    // Flags
    bool flagZ{false};
    bool flagN{false};
    bool flagH{false};
    bool flagC{false};

    // OPCode definitions

    void OPCode0x00();
    void OPCode0x01();
    void OPCode0x02();
    void OPCode0x03();
    void OPCode0x04();
    void OPCode0x05();
    void OPCode0x06();
    void OPCode0x07();
    void OPCode0x08();
    void OPCode0x09();
    void OPCode0x0A();
    void OPCode0x0B();
    void OPCode0x0C();
    void OPCode0x0D();
    void OPCode0x0E();
    void OPCode0x0F();
    void OPCode0x10();
    void OPCode0x11();
    void OPCode0x12();
    void OPCode0x13();
    void OPCode0x14();
    void OPCode0x15();
    void OPCode0x16();
    void OPCode0x17();
    void OPCode0x18();
    void OPCode0x19();
    void OPCode0x1A();
    void OPCode0x1B();
    void OPCode0x1C();
    void OPCode0x1D();
    void OPCode0x1E();
    void OPCode0x1F();
    void OPCode0x20();
    void OPCode0x21();
    void OPCode0x22();
    void OPCode0x23();
    void OPCode0x24();
    void OPCode0x25();
    void OPCode0x26();
    void OPCode0x27();
    void OPCode0x28();
    void OPCode0x29();
    void OPCode0x2A();
    void OPCode0x2B();
    void OPCode0x2C();
    void OPCode0x2D();
    void OPCode0x2E();
    void OPCode0x2F();
    void OPCode0x30();
    void OPCode0x31();
    void OPCode0x32();
    void OPCode0x33();
    void OPCode0x34();
    void OPCode0x35();
    void OPCode0x36();
    void OPCode0x37();
    void OPCode0x38();
    void OPCode0x39();
    void OPCode0x3A();
    void OPCode0x3B();
    void OPCode0x3C();
    void OPCode0x3D();
    void OPCode0x3E();
    void OPCode0x3F();
    void OPCode0x40();
    void OPCode0x41();
    void OPCode0x42();
    void OPCode0x43();
    void OPCode0x44();
    void OPCode0x45();
    void OPCode0x46();
    void OPCode0x47();
    void OPCode0x48();
    void OPCode0x49();
    void OPCode0x4A();
    void OPCode0x4B();
    void OPCode0x4C();
    void OPCode0x4D();
    void OPCode0x4E();
    void OPCode0x4F();
    void OPCode0x50();
    void OPCode0x51();
    void OPCode0x52();
    void OPCode0x53();
    void OPCode0x54();
    void OPCode0x55();
    void OPCode0x56();
    void OPCode0x57();
    void OPCode0x58();
    void OPCode0x59();
    void OPCode0x5A();
    void OPCode0x5B();
    void OPCode0x5C();
    void OPCode0x5D();
    void OPCode0x5E();
    void OPCode0x5F();
    void OPCode0x60();
    void OPCode0x61();
    void OPCode0x62();
    void OPCode0x63();
    void OPCode0x64();
    void OPCode0x65();
    void OPCode0x66();
    void OPCode0x67();
    void OPCode0x68();
    void OPCode0x69();
    void OPCode0x6A();
    void OPCode0x6B();
    void OPCode0x6C();
    void OPCode0x6D();
    void OPCode0x6E();
    void OPCode0x6F();
    void OPCode0x70();
    void OPCode0x71();
    void OPCode0x72();
    void OPCode0x73();
    void OPCode0x74();
    void OPCode0x75();
    void OPCode0x76();
    void OPCode0x77();
    void OPCode0x78();
    void OPCode0x79();
    void OPCode0x7A();
    void OPCode0x7B();
    void OPCode0x7C();
    void OPCode0x7D();
    void OPCode0x7E();
    void OPCode0x7F();
    void OPCode0x80();
    void OPCode0x81();
    void OPCode0x82();
    void OPCode0x83();
    void OPCode0x84();
    void OPCode0x85();
    void OPCode0x86();
    void OPCode0x87();
    void OPCode0x88();
    void OPCode0x89();
    void OPCode0x8A();
    void OPCode0x8B();
    void OPCode0x8C();
    void OPCode0x8D();
    void OPCode0x8E();
    void OPCode0x8F();
    void OPCode0x90();
    void OPCode0x91();
    void OPCode0x92();
    void OPCode0x93();
    void OPCode0x94();
    void OPCode0x95();
    void OPCode0x96();
    void OPCode0x97();
    void OPCode0x98();
    void OPCode0x99();
    void OPCode0x9A();
    void OPCode0x9B();
    void OPCode0x9C();
    void OPCode0x9D();
    void OPCode0x9E();
    void OPCode0x9F();
    void OPCode0xA0();
    void OPCode0xA1();
    void OPCode0xA2();
    void OPCode0xA3();
    void OPCode0xA4();
    void OPCode0xA5();
    void OPCode0xA6();
    void OPCode0xA7();
    void OPCode0xA8();
    void OPCode0xA9();
    void OPCode0xAA();
    void OPCode0xAB();
    void OPCode0xAC();
    void OPCode0xAD();
    void OPCode0xAE();
    void OPCode0xAF();
    void OPCode0xB0();
    void OPCode0xB1();
    void OPCode0xB2();
    void OPCode0xB3();
    void OPCode0xB4();
    void OPCode0xB5();
    void OPCode0xB6();
    void OPCode0xB7();
    void OPCode0xB8();
    void OPCode0xB9();
    void OPCode0xBA();
    void OPCode0xBB();
    void OPCode0xBC();
    void OPCode0xBD();
    void OPCode0xBE();
    void OPCode0xBF();
    void OPCode0xC0();
    void OPCode0xC1();
    void OPCode0xC2();
    void OPCode0xC3();
    void OPCode0xC4();
    void OPCode0xC5();
    void OPCode0xC6();
    void OPCode0xC7();
    void OPCode0xC8();
    void OPCode0xC9();
    void OPCode0xCA();
    void OPCode0xCB();
    void OPCode0xCC();
    void OPCode0xCD();
    void OPCode0xCE();
    void OPCode0xCF();
    void OPCode0xD0();
    void OPCode0xD1();
    void OPCode0xD2();
    void OPCode0xD3();
    void OPCode0xD4();
    void OPCode0xD5();
    void OPCode0xD6();
    void OPCode0xD7();
    void OPCode0xD8();
    void OPCode0xD9();
    void OPCode0xDA();
    void OPCode0xDB();
    void OPCode0xDC();
    void OPCode0xDD();
    void OPCode0xDE();
    void OPCode0xDF();
    void OPCode0xE0();
    void OPCode0xE1();
    void OPCode0xE2();
    void OPCode0xE3();
    void OPCode0xE4();
    void OPCode0xE5();
    void OPCode0xE6();
    void OPCode0xE7();
    void OPCode0xE8();
    void OPCode0xE9();
    void OPCode0xEA();
    void OPCode0xEB();
    void OPCode0xEC();
    void OPCode0xED();
    void OPCode0xEE();
    void OPCode0xEF();
    void OPCode0xF0();
    void OPCode0xF1();
    void OPCode0xF2();
    void OPCode0xF3();
    void OPCode0xF4();
    void OPCode0xF5();
    void OPCode0xF6();
    void OPCode0xF7();
    void OPCode0xF8();
    void OPCode0xF9();
    void OPCode0xFA();
    void OPCode0xFB();
    void OPCode0xFC();
    void OPCode0xFD();
    void OPCode0xFE();
    void OPCode0xFF();
};