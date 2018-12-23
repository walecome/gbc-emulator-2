#include "Processor.hh"

void Processor::OPCode0x00()
{
    // NOP
}

void Processor::OPCode0x01()
{
    // LD BC, d16

    // TODO check stack data order

    byte_t data_b = program_memory[program_counter];
    ++program_counter;

    byte_t data_c = program_memory[program_counter];
    ++program_counter;

    setB(data_b);
    setC(data_c);
}

void Processor::OPCode0x02()
{
    // LD (BC), A
    // TODO handle memory
}

void Processor::OPCode0x03()
{
    // INC BC
    BC->increment();
}

void Processor::OPCode0x04()
{
    // INC B
    B->increment();

    // TODO double check flag logic
    register8_t value_b = B->getValue();

    if (value_b == 0x00)
        setFlagZ(true);

    setFlagN(false);

    if ((value_b & 0x0F) == 0x00)
        setFlagH(true);
}

void Processor::OPCode0x05()
{
    // DEC B
    B->decrement();

    register8_t value_b = B->getValue();

    if (value_b == 0x00)
        setFlagZ(true);

    setFlagN(true);

    if ((value_b & 0x0F) == 0x00)
        setFlagH(true);
}

void Processor::OPCode0x06()
{
    // LD B, d8
    byte_t data_b = program_memory[program_counter];
    ++program_counter;

    B->setValue(data_b);
}

void Processor::OPCode0x07()
{
    // RLCA
    // TODO bitwise rotation
}

void Processor::OPCode0x08()
{
    // LD (a16), SP
    // TODO memory management
}

void Processor::OPCode0x09()
{
    // ADD HL, BC

    register16_t value_hl = HL->getValue();

    register16_t value_bc = BC->getValue();

    value_hl += value_bc;

    HL->setValue(value_hl);
}

void Processor::OPCode0x0A()
{
    // LD A, (BC)
    // TODO memory management
}

void Processor::OPCode0x0B()
{
    // DEC BC
    BC->decrement();
}

void Processor::OPCode0x0C()
{
    // INC C
    C->increment();
}

void Processor::OPCode0x0D()
{
    // DEC C
    C->decrement();
}

void Processor::OPCode0x0E()
{
    // LD C, d8
    register8_t data_c = program_memory[program_counter];
    ++program_counter;

    C->setValue(data_c);
}

void Processor::OPCode0x0F()
{
    // RRCA
    // TODO fix
}

void Processor::OPCode0x10()
{
    // STOP 0
    // TODO fix
}

void Processor::OPCode0x11()
{
    // LD DE, d16
    // TODO check stack data order
    register8_t data_e = program_memory[program_counter];
    ++program_counter;

    register8_t data_d = program_memory[program_counter];
    ++program_counter;

    E->setValue(data_e);
    D->setValue(data_d);
}

void Processor::OPCode0x12()
{
    // LD (DE), A
    // TODO memory management
}

void Processor::OPCode0x13()
{
    // INC DE
    DE->increment();
}

void Processor::OPCode0x14()
{
    // INC D
    D->increment();
}

void Processor::OPCode0x15()
{
    // DEC D
    D->decrement();
}

void Processor::OPCode0x16()
{
    // LD D, d8
    register8_t data_d = program_memory[program_counter];
    ++program_counter;

    D->setValue(data_d);
}

void Processor::OPCode0x17()
{
    // RLA
    // TODO fix
}

void Processor::OPCode0x18()
{
    // JR d8
    // TODO double check logic
    program_counter += (int8_t)program_memory[program_counter];
}

void Processor::OPCode0x19()
{
    // ADD HL, DE
    register16_t value_hl = HL->getValue();
    register16_t value_de = DE->getValue();

    value_hl += value_de;

    HL->setValue(value_hl);
}

void Processor::OPCode0x1A()
{
    // LD A, (DE)
    // TODO memory management
}

void Processor::OPCode0x1B()
{
    // DEC DE
    DE->decrement();
}

void Processor::OPCode0x1C()
{
    // INC E
    E->increment();
}

void Processor::OPCode0x1D()
{
    // DEC E
    E->decrement();
}

void Processor::OPCode0x1E()
{
    // LD E, d8
    register8_t data_e = program_memory[program_counter];
    ++program_counter;

    E->setValue(data_e);
}

void Processor::OPCode0x1F()
{
}

void Processor::OPCode0x20()
{
}

void Processor::OPCode0x21()
{
}

void Processor::OPCode0x22()
{
}

void Processor::OPCode0x23()
{
}

void Processor::OPCode0x24()
{
}

void Processor::OPCode0x25()
{
}

void Processor::OPCode0x26()
{
}

void Processor::OPCode0x27()
{
}

void Processor::OPCode0x28()
{
}

void Processor::OPCode0x29()
{
}

void Processor::OPCode0x2A()
{
}

void Processor::OPCode0x2B()
{
}

void Processor::OPCode0x2C()
{
}

void Processor::OPCode0x2D()
{
}

void Processor::OPCode0x2E()
{
}

void Processor::OPCode0x2F()
{
}

void Processor::OPCode0x30()
{
}

void Processor::OPCode0x31()
{
}

void Processor::OPCode0x32()
{
}

void Processor::OPCode0x33()
{
}

void Processor::OPCode0x34()
{
}

void Processor::OPCode0x35()
{
}

void Processor::OPCode0x36()
{
}

void Processor::OPCode0x37()
{
}

void Processor::OPCode0x38()
{
}

void Processor::OPCode0x39()
{
}

void Processor::OPCode0x3A()
{
}

void Processor::OPCode0x3B()
{
}

void Processor::OPCode0x3C()
{
}

void Processor::OPCode0x3D()
{
}

void Processor::OPCode0x3E()
{
}

void Processor::OPCode0x3F()
{
}

void Processor::OPCode0x40()
{
}

void Processor::OPCode0x41()
{
}

void Processor::OPCode0x42()
{
}

void Processor::OPCode0x43()
{
}

void Processor::OPCode0x44()
{
}

void Processor::OPCode0x45()
{
}

void Processor::OPCode0x46()
{
}

void Processor::OPCode0x47()
{
}

void Processor::OPCode0x48()
{
}

void Processor::OPCode0x49()
{
}

void Processor::OPCode0x4A()
{
}

void Processor::OPCode0x4B()
{
}

void Processor::OPCode0x4C()
{
}

void Processor::OPCode0x4D()
{
}

void Processor::OPCode0x4E()
{
}

void Processor::OPCode0x4F()
{
}

void Processor::OPCode0x50()
{
}

void Processor::OPCode0x51()
{
}

void Processor::OPCode0x52()
{
}

void Processor::OPCode0x53()
{
}

void Processor::OPCode0x54()
{
}

void Processor::OPCode0x55()
{
}

void Processor::OPCode0x56()
{
}

void Processor::OPCode0x57()
{
}

void Processor::OPCode0x58()
{
}

void Processor::OPCode0x59()
{
}

void Processor::OPCode0x5A()
{
}

void Processor::OPCode0x5B()
{
}

void Processor::OPCode0x5C()
{
}

void Processor::OPCode0x5D()
{
}

void Processor::OPCode0x5E()
{
}

void Processor::OPCode0x5F()
{
}

void Processor::OPCode0x60()
{
}

void Processor::OPCode0x61()
{
}

void Processor::OPCode0x62()
{
}

void Processor::OPCode0x63()
{
}

void Processor::OPCode0x64()
{
}

void Processor::OPCode0x65()
{
}

void Processor::OPCode0x66()
{
}

void Processor::OPCode0x67()
{
}

void Processor::OPCode0x68()
{
}

void Processor::OPCode0x69()
{
}

void Processor::OPCode0x6A()
{
}

void Processor::OPCode0x6B()
{
}

void Processor::OPCode0x6C()
{
}

void Processor::OPCode0x6D()
{
}

void Processor::OPCode0x6E()
{
}

void Processor::OPCode0x6F()
{
}

void Processor::OPCode0x70()
{
}

void Processor::OPCode0x71()
{
}

void Processor::OPCode0x72()
{
}

void Processor::OPCode0x73()
{
}

void Processor::OPCode0x74()
{
}

void Processor::OPCode0x75()
{
}

void Processor::OPCode0x76()
{
}

void Processor::OPCode0x77()
{
}

void Processor::OPCode0x78()
{
}

void Processor::OPCode0x79()
{
}

void Processor::OPCode0x7A()
{
}

void Processor::OPCode0x7B()
{
}

void Processor::OPCode0x7C()
{
}

void Processor::OPCode0x7D()
{
}

void Processor::OPCode0x7E()
{
}

void Processor::OPCode0x7F()
{
}

void Processor::OPCode0x80()
{
}

void Processor::OPCode0x81()
{
}

void Processor::OPCode0x82()
{
}

void Processor::OPCode0x83()
{
}

void Processor::OPCode0x84()
{
}

void Processor::OPCode0x85()
{
}

void Processor::OPCode0x86()
{
}

void Processor::OPCode0x87()
{
}

void Processor::OPCode0x88()
{
}

void Processor::OPCode0x89()
{
}

void Processor::OPCode0x8A()
{
}

void Processor::OPCode0x8B()
{
}

void Processor::OPCode0x8C()
{
}

void Processor::OPCode0x8D()
{
}

void Processor::OPCode0x8E()
{
}

void Processor::OPCode0x8F()
{
}

void Processor::OPCode0x90()
{
}

void Processor::OPCode0x91()
{
}

void Processor::OPCode0x92()
{
}

void Processor::OPCode0x93()
{
}

void Processor::OPCode0x94()
{
}

void Processor::OPCode0x95()
{
}

void Processor::OPCode0x96()
{
}

void Processor::OPCode0x97()
{
}

void Processor::OPCode0x98()
{
}

void Processor::OPCode0x99()
{
}

void Processor::OPCode0x9A()
{
}

void Processor::OPCode0x9B()
{
}

void Processor::OPCode0x9C()
{
}

void Processor::OPCode0x9D()
{
}

void Processor::OPCode0x9E()
{
}

void Processor::OPCode0x9F()
{
}

void Processor::OPCode0xA0()
{
}

void Processor::OPCode0xA1()
{
}

void Processor::OPCode0xA2()
{
}

void Processor::OPCode0xA3()
{
}

void Processor::OPCode0xA4()
{
}

void Processor::OPCode0xA5()
{
}

void Processor::OPCode0xA6()
{
}

void Processor::OPCode0xA7()
{
}

void Processor::OPCode0xA8()
{
}

void Processor::OPCode0xA9()
{
}

void Processor::OPCode0xAA()
{
}

void Processor::OPCode0xAB()
{
}

void Processor::OPCode0xAC()
{
}

void Processor::OPCode0xAD()
{
}

void Processor::OPCode0xAE()
{
}

void Processor::OPCode0xAF()
{
}

void Processor::OPCode0xB0()
{
}

void Processor::OPCode0xB1()
{
}

void Processor::OPCode0xB2()
{
}

void Processor::OPCode0xB3()
{
}

void Processor::OPCode0xB4()
{
}

void Processor::OPCode0xB5()
{
}

void Processor::OPCode0xB6()
{
}

void Processor::OPCode0xB7()
{
}

void Processor::OPCode0xB8()
{
}

void Processor::OPCode0xB9()
{
}

void Processor::OPCode0xBA()
{
}

void Processor::OPCode0xBB()
{
}

void Processor::OPCode0xBC()
{
}

void Processor::OPCode0xBD()
{
}

void Processor::OPCode0xBE()
{
}

void Processor::OPCode0xBF()
{
}

void Processor::OPCode0xC0()
{
}

void Processor::OPCode0xC1()
{
}

void Processor::OPCode0xC2()
{
}

void Processor::OPCode0xC3()
{
}

void Processor::OPCode0xC4()
{
}

void Processor::OPCode0xC5()
{
}

void Processor::OPCode0xC6()
{
}

void Processor::OPCode0xC7()
{
}

void Processor::OPCode0xC8()
{
}

void Processor::OPCode0xC9()
{
}

void Processor::OPCode0xCA()
{
}

void Processor::OPCode0xCB()
{
}

void Processor::OPCode0xCC()
{
}

void Processor::OPCode0xCD()
{
}

void Processor::OPCode0xCE()
{
}

void Processor::OPCode0xCF()
{
}

void Processor::OPCode0xD0()
{
}

void Processor::OPCode0xD1()
{
}

void Processor::OPCode0xD2()
{
}

void Processor::OPCode0xD3()
{
}

void Processor::OPCode0xD4()
{
}

void Processor::OPCode0xD5()
{
}

void Processor::OPCode0xD6()
{
}

void Processor::OPCode0xD7()
{
}

void Processor::OPCode0xD8()
{
}

void Processor::OPCode0xD9()
{
}

void Processor::OPCode0xDA()
{
}

void Processor::OPCode0xDB()
{
}

void Processor::OPCode0xDC()
{
}

void Processor::OPCode0xDD()
{
}

void Processor::OPCode0xDE()
{
}

void Processor::OPCode0xDF()
{
}

void Processor::OPCode0xE0()
{
}

void Processor::OPCode0xE1()
{
}

void Processor::OPCode0xE2()
{
}

void Processor::OPCode0xE3()
{
}

void Processor::OPCode0xE4()
{
}

void Processor::OPCode0xE5()
{
}

void Processor::OPCode0xE6()
{
}

void Processor::OPCode0xE7()
{
}

void Processor::OPCode0xE8()
{
}

void Processor::OPCode0xE9()
{
}

void Processor::OPCode0xEA()
{
}

void Processor::OPCode0xEB()
{
}

void Processor::OPCode0xEC()
{
}

void Processor::OPCode0xED()
{
}

void Processor::OPCode0xEE()
{
}

void Processor::OPCode0xEF()
{
}

void Processor::OPCode0xF0()
{
}

void Processor::OPCode0xF1()
{
}

void Processor::OPCode0xF2()
{
}

void Processor::OPCode0xF3()
{
}

void Processor::OPCode0xF4()
{
}

void Processor::OPCode0xF5()
{
}

void Processor::OPCode0xF6()
{
}

void Processor::OPCode0xF7()
{
}

void Processor::OPCode0xF8()
{
}

void Processor::OPCode0xF9()
{
}

void Processor::OPCode0xFA()
{
}

void Processor::OPCode0xFB()
{
}

void Processor::OPCode0xFC()
{
}

void Processor::OPCode0xFD()
{
}

void Processor::OPCode0xFE()
{
}

void Processor::OPCode0xFF()
{
}
