#include "Processor.hh"

void Processor::OPCode0x00()
{
    // NOP
}

void Processor::OPCode0x01()
{
    // LD BC, d16

    // TODO check stack data order

    loadRegister(B);
    loadRegister(C);
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
    incrementRegister(B);
}

void Processor::OPCode0x05()
{
    // DEC B
    decrementRegister(B);
}

void Processor::OPCode0x06()
{
    // LD B, d8
    loadRegister(B);
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
    loadFromMemory(A, BC);
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
    loadRegister(C);
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
    loadRegister(E);
    loadRegister(D);
}

void Processor::OPCode0x12()
{
    // LD (DE), A
    loadIntoMemory(DE, A);
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
    loadRegister(D);
}

void Processor::OPCode0x17()
{
    // RLA
    // TODO fix
}

void Processor::OPCode0x18()
{
    // JR d8
    performJump();
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
    loadFromMemory(A, DE);
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
    loadRegister(E);
}

void Processor::OPCode0x1F()
{
    // RRA
    // TODO fix
}

void Processor::OPCode0x20()
{
    // JR NZ, r8
    // TODO double check
    if (getFlagZ() == false)
    {
        performJump();
    }
}

void Processor::OPCode0x21()
{
    // LD HL, d16
    loadRegister(H);
    loadRegister(L);
}

void Processor::OPCode0x22()
{
    // LD (HL+), A
    // TODO memory management
}

void Processor::OPCode0x23()
{
    // INC HL
    HL->increment();
}

void Processor::OPCode0x24()
{
    // INC H
    H->increment();
}

void Processor::OPCode0x25()
{
    // DEC H
    H->decrement();
}

void Processor::OPCode0x26()
{
    // LD H, d8
    loadRegister(H);
}

void Processor::OPCode0x27()
{
    // DAA
    // TODO fix
}

void Processor::OPCode0x28()
{
    // JR Z, R8
    // TODO double check

    if (getFlagZ())
    {
        performJump();
    }
}

void Processor::OPCode0x29()
{
    // ADD HL, HL
    register16_t data_hl = HL->getValue();

    data_hl *= 2;

    HL->setValue(data_hl);
}

void Processor::OPCode0x2A()
{
    // LD A, (HL+)
    // TODO memory management
}

void Processor::OPCode0x2B()
{
    // DEC HL
    HL->decrement();
}

void Processor::OPCode0x2C()
{
    // INC L
    L->increment();
}

void Processor::OPCode0x2D()
{
    // DEC L
    L->decrement();
}

void Processor::OPCode0x2E()
{
    // LD L, d8
    loadRegister(L);
}

void Processor::OPCode0x2F()
{
    // CPL
    // TODO fix
}

void Processor::OPCode0x30()
{
    // JR NC, r8
    if (!getFlagC())
    {
        performJump();
    }
}

void Processor::OPCode0x31()
{
    // LD SP, d16
    loadRegister(stack_pointer->getLowRegister());
    loadRegister(stack_pointer->getHighRegister());
}

void Processor::OPCode0x32()
{
    // LD (HL-), A
    // TODO memory management
}

void Processor::OPCode0x33()
{
    // INC SP
    stack_pointer->increment();
}

void Processor::OPCode0x34()
{
    // INC (HL)
    register16_t address = RAM_DATA_OFFSET + HL->getValue();
    byte_t data = ram->getData(address);

    ++data;

    ram->setData(address, data);
}

void Processor::OPCode0x35()
{
    // DEC (HL)
    register16_t address = RAM_DATA_OFFSET + HL->getValue();
    byte_t data = ram->getData(address);

    --data;

    ram->setData(address, data);
}

void Processor::OPCode0x36()
{
    // LD (HL), d8
    byte_t data = getCurrentData();
    register16_t address = RAM_DATA_OFFSET + HL->getValue();

    ram->setData(address, data);
}

void Processor::OPCode0x37()
{
    // DAA
    // TODO fix
}

void Processor::OPCode0x38()
{
    // JR C, r8
    if (getFlagC())
    {
        performJump();
    }
}

void Processor::OPCode0x39()
{
    // ADD HL, SP
    register16_t data_hl = HL->getValue();
    register16_t data_sp = stack_pointer->getValue();

    data_hl += data_sp;

    HL->setValue(data_hl);
}

void Processor::OPCode0x3A()
{
    // LD A, (HL-)
    // TODO fix memory management
}

void Processor::OPCode0x3B()
{
    // DEC SP
    stack_pointer->decrement();
}

void Processor::OPCode0x3C()
{
    // INC A
    A->increment();
}

void Processor::OPCode0x3D()
{
    // DEC A
    A->decrement();
}

void Processor::OPCode0x3E()
{
    // LD A, d8
    loadRegister(A);
}

void Processor::OPCode0x3F()
{
    // CCF
    // TODO fix
}

void Processor::OPCode0x40()
{
    // LD B, B
    // Do nothing...
}

void Processor::OPCode0x41()
{
    // LD B, C
    copyRegister(B, C);
}

void Processor::OPCode0x42()
{
    // LD B, D
    copyRegister(B, D);
}

void Processor::OPCode0x43()
{
    // LD B, E
    copyRegister(B, E);
}

void Processor::OPCode0x44()
{
    // LD B, H
    copyRegister(B, H);
}

void Processor::OPCode0x45()
{
    // LD B, L
    copyRegister(B, L);
}

void Processor::OPCode0x46()
{
    // LD B, (HL)
    loadFromMemory(B, HL);
}

void Processor::OPCode0x47()
{
    // LD B, A
    copyRegister(B, A);
}

void Processor::OPCode0x48()
{
    // LD C, B
    copyRegister(C, B);
}

void Processor::OPCode0x49()
{
    // LD C, C
    // Do nothing...
}

void Processor::OPCode0x4A()
{
    // LD C, D
    copyRegister(C, D);
}

void Processor::OPCode0x4B()
{
    // LD C, E
    copyRegister(C, E);
}

void Processor::OPCode0x4C()
{
    // LD C, H
    copyRegister(C, H);
}

void Processor::OPCode0x4D()
{
    // LD C, L
    copyRegister(C, L);
}

void Processor::OPCode0x4E()
{
    // LD C, (HL)
    loadFromMemory(C, HL);
}

void Processor::OPCode0x4F()
{
    // LD C, A
    copyRegister(C, A);
}

void Processor::OPCode0x50()
{
    // LD D, B
    copyRegister(D, B);
}

void Processor::OPCode0x51()
{
    // LD D, C
    copyRegister(D, C);
}

void Processor::OPCode0x52()
{
    // LD D, D
    // Do nothing...
}

void Processor::OPCode0x53()
{
    // LD D, E
    copyRegister(D, E);
}

void Processor::OPCode0x54()
{
    // LD D, H
    copyRegister(D, H);
}

void Processor::OPCode0x55()
{
    // LD D, L
    copyRegister(D, L);
}

void Processor::OPCode0x56()
{
    // LD D, (HL)
    loadFromMemory(D, HL);
}

void Processor::OPCode0x57()
{
    // LD D, A
    copyRegister(D, A);
}

void Processor::OPCode0x58()
{
    // LD E, B
    copyRegister(E, B);
}

void Processor::OPCode0x59()
{
    // LD E, C
    copyRegister(E, C);
}

void Processor::OPCode0x5A()
{
    // LD E, D
    copyRegister(E, D);
}

void Processor::OPCode0x5B()
{
    // LD E, E
    // Do nothing...
}

void Processor::OPCode0x5C()
{
    // LD E, H
    copyRegister(E, H);
}

void Processor::OPCode0x5D()
{
    // LD E, L
    copyRegister(E, L);
}

void Processor::OPCode0x5E()
{
    // LD E, (HL)
    loadFromMemory(E, HL);
}

void Processor::OPCode0x5F()
{
    // LD E, A
    copyRegister(E, A);
}

void Processor::OPCode0x60()
{
    // LD H, B
    copyRegister(H, B);
}

void Processor::OPCode0x61()
{
    // LD H, C
    copyRegister(H, C);
}

void Processor::OPCode0x62()
{
    // LD H, D
    copyRegister(H, D);
}

void Processor::OPCode0x63()
{
    // LD H, E
    copyRegister(H, E);
}

void Processor::OPCode0x64()
{
    // LD H, H
    // Do nothing...
}

void Processor::OPCode0x65()
{
    // LD H, L
    copyRegister(H, L);
}

void Processor::OPCode0x66()
{
    // LD H, (HL)
    loadFromMemory(H, HL);
}

void Processor::OPCode0x67()
{
    // LD H, A
    copyRegister(H, A);
}

void Processor::OPCode0x68()
{
    // LD L, B
    copyRegister(L, B);
}

void Processor::OPCode0x69()
{
    // LD L, C
    copyRegister(L, C);
}

void Processor::OPCode0x6A()
{
    // LD L, D
    copyRegister(L, D);
}

void Processor::OPCode0x6B()
{
    // LD L, E
    copyRegister(L, E);
}

void Processor::OPCode0x6C()
{
    // LD L, H
    copyRegister(L, H);
}

void Processor::OPCode0x6D()
{
    // LD L, L
    // Do nothing...
}

void Processor::OPCode0x6E()
{
    // LD L, (HL)
    loadFromMemory(L, HL);
}

void Processor::OPCode0x6F()
{
    // LD L, A
    copyRegister(L, A);
}

void Processor::OPCode0x70()
{
    // LD (HL), B
    loadIntoMemory(HL, B);
}

void Processor::OPCode0x71()
{
    // LD (HL), C
    loadIntoMemory(HL, C);
}

void Processor::OPCode0x72()
{
    // LD (HL), D
    loadIntoMemory(HL, D);
}

void Processor::OPCode0x73()
{
    // LD (HL), E
    loadIntoMemory(HL, E);
}

void Processor::OPCode0x74()
{
    // LD (HL), H
    loadIntoMemory(HL, H);
}

void Processor::OPCode0x75()
{
    // LD (HL), L
    loadIntoMemory(HL, L);
}

void Processor::OPCode0x76()
{
    // HALT
    // TODO stop??
}

void Processor::OPCode0x77()
{
    // LD (HL), A
    loadIntoMemory(HL, A);
}

void Processor::OPCode0x78()
{
    // LD A, B
    copyRegister(A, B);
}

void Processor::OPCode0x79()
{
    // LD A, C
    copyRegister(A, C);
}

void Processor::OPCode0x7A()
{
    // LD A, D
    copyRegister(A, D);
}

void Processor::OPCode0x7B()
{
    // LD A, E
    copyRegister(A, E);
}

void Processor::OPCode0x7C()
{
    // LD A, H
    copyRegister(A, H);
}

void Processor::OPCode0x7D()
{
    // LD A, L
    copyRegister(A, L);
}

void Processor::OPCode0x7E()
{
    // LD A, (HL)
    loadFromMemory(A, HL);
}

void Processor::OPCode0x7F()
{
    // LD A, A
    // Do nothing...
}

void Processor::OPCode0x80()
{
    // ADD A, B
    addRegisters(A, B);
}

void Processor::OPCode0x81()
{
    // ADD A, C
    addRegisters(A, C);
}

void Processor::OPCode0x82()
{
    // ADD A, D
    addRegisters(A, D);
}

void Processor::OPCode0x83()
{
    // ADD A, E
    addRegisters(A, E);
}

void Processor::OPCode0x84()
{
    // ADD A, H
    addRegisters(A, H);
}

void Processor::OPCode0x85()
{
    // ADD A, L
    addRegisters(A, L);
}

void Processor::OPCode0x86()
{
    // ADD A, (HL)
    register8_t data_a = A->getValue();
    register16_t address = RAM_DATA_OFFSET + HL->getValue();
    register8_t data_mem = ram->getData(address);

    register8_t result = data_a + data_mem;

    A->setValue(result);
}

void Processor::OPCode0x87()
{
    // ADD A, A
    addRegisters(A, A);
}

void Processor::OPCode0x88()
{
    // ADC A, B
    addWithCarry(A, B);
}

void Processor::OPCode0x89()
{
    // ADC A, C
    addWithCarry(A, C);
}

void Processor::OPCode0x8A()
{
    // ADC A, D
    addWithCarry(A, D);
}

void Processor::OPCode0x8B()
{
    // ADC A, E
    addWithCarry(A, E);
}

void Processor::OPCode0x8C()
{
    // ADC A, H
    addWithCarry(A, H);
}

void Processor::OPCode0x8D()
{
    // ADC A, L
    addWithCarry(A, L);
}

void Processor::OPCode0x8E()
{
    // ADC A, (HL)
    // TODO fix
}

void Processor::OPCode0x8F()
{
    // ADC A, A
    addWithCarry(A, A);
}

void Processor::OPCode0x90()
{
    // SUB B
    subRegisters(B);
}

void Processor::OPCode0x91()
{
    // SUB C
    subRegisters(C);
}

void Processor::OPCode0x92()
{
    // SUB D
    subRegisters(D);
}

void Processor::OPCode0x93()
{
    // SUB E
    subRegisters(E);
}

void Processor::OPCode0x94()
{
    // SUB H
    subRegisters(H);
}

void Processor::OPCode0x95()
{
    // SUB L
    subRegisters(L);
}

void Processor::OPCode0x96()
{
    // SUB (HL)
    // TODO memory management
}

void Processor::OPCode0x97()
{
    // SUB A
    subRegisters(A);
}

void Processor::OPCode0x98()
{
    // SBC A, B
    subWithCarry(B);
}

void Processor::OPCode0x99()
{
    // SBC A, C
    subWithCarry(C);
}

void Processor::OPCode0x9A()
{
    // SBC A, D
    subWithCarry(D);
}

void Processor::OPCode0x9B()
{
    // SBC A, E
    subWithCarry(E);
}

void Processor::OPCode0x9C()
{
    // SBC A, H
    subWithCarry(H);
}

void Processor::OPCode0x9D()
{
    // SBC A, L
    subWithCarry(L);
}

void Processor::OPCode0x9E()
{
    // SBC A, (HL)
    // TODO memory management
}

void Processor::OPCode0x9F()
{
    // SBC A, A
    subWithCarry(A);
}

void Processor::OPCode0xA0()
{
    // AND B
    andRegisters(B);
}

void Processor::OPCode0xA1()
{
    // AND C
    andRegisters(C);
}

void Processor::OPCode0xA2()
{
    // AND D
    andRegisters(D);
}

void Processor::OPCode0xA3()
{
    // AND E
    andRegisters(E);
}

void Processor::OPCode0xA4()
{
    // AND H
    andRegisters(H);
}

void Processor::OPCode0xA5()
{
    // AND L
    andRegisters(L);
}

void Processor::OPCode0xA6()
{
    // AND (HL)
    register16_t address = RAM_DATA_OFFSET + HL->getValue();

    register8_t data_mem = ram->getData(address);

    register8_t data_a = A->getValue();

    data_a = data_a & data_mem;

    A->setValue(data_a);
}

void Processor::OPCode0xA7()
{
    // AND A
    // Do nothing...
}

void Processor::OPCode0xA8()
{
    // XOR B
    xorRegisters(B);
}

void Processor::OPCode0xA9()
{
    // XOR C
    xorRegisters(C);
}

void Processor::OPCode0xAA()
{
    // XOR D
    xorRegisters(D);
}

void Processor::OPCode0xAB()
{
    // XOR E
    xorRegisters(E);
}

void Processor::OPCode0xAC()
{
    // XOR H
    xorRegisters(H);
}

void Processor::OPCode0xAD()
{
    // XOR L
    xorRegisters(L);
}

void Processor::OPCode0xAE()
{
    // XOR (HL)
    // TODO memory management
}

void Processor::OPCode0xAF()
{
    // XOR A
    xorRegisters(A);
}

void Processor::OPCode0xB0()
{
    // OR B
    orRegisters(B);
}

void Processor::OPCode0xB1()
{
    // OR C
    orRegisters(C);
}

void Processor::OPCode0xB2()
{
    // OR D
    orRegisters(D);
}

void Processor::OPCode0xB3()
{
    // OR E
    orRegisters(E);
}

void Processor::OPCode0xB4()
{
    // OR H
    orRegisters(H);
}

void Processor::OPCode0xB5()
{
    // OR L
    orRegisters(L);
}

void Processor::OPCode0xB6()
{
    // OR (HL)
    // TODO memory management
}

void Processor::OPCode0xB7()
{
    // OR A
    // Do nothing...
}

void Processor::OPCode0xB8()
{
    // CP B
    cmpRegisters(B);
}

void Processor::OPCode0xB9()
{
    // CP C
    cmpRegisters(C);
}

void Processor::OPCode0xBA()
{
    // CP D
    cmpRegisters(D);
}

void Processor::OPCode0xBB()
{
    // CP E
    cmpRegisters(E);
}

void Processor::OPCode0xBC()
{
    // CP H
    cmpRegisters(H);
}

void Processor::OPCode0xBD()
{
    // CP L
    cmpRegisters(L);
}

void Processor::OPCode0xBE()
{
    // CP (HL)
    byte_t data = ram->getData(HL->getValue());

    setFlagZ(data == A->getValue());
    // TODO half carry flag
    setFlagN(true);
    setFlagC(A->getValue() < data);
}

void Processor::OPCode0xBF()
{
    // CP A
    cmpRegisters(A);
}

void Processor::OPCode0xC0()
{
    // RET NZ
    if (!getFlagZ())
    {
        popStack(program_counter);
    }
}

void Processor::OPCode0xC1()
{
    // POP BC
    popStack(BC);
}

void Processor::OPCode0xC2()
{
    // JP NZ, a16
    if (!getFlagZ())
    {
        jumpIm16bit();
    }
    else
    {
        // Skips these of we don't jump
        program_counter->increment();
        program_counter->increment();
    }
}

void Processor::OPCode0xC3()
{
    // JP a16
    jumpIm16bit();
}

void Processor::OPCode0xC4()
{
    // CALL NZ, a16
    if (!getFlagZ())
    {
        byte_t data_low = getCurrentData();
        byte_t data_high = getCurrentData();

        pushStack(program_counter);

        program_counter->getLowRegister()->setValue(data_low);
        program_counter->getHighRegister()->setValue(data_high);
    }
    else
    {
        program_counter->increment();
        program_counter->increment();
    }
}

void Processor::OPCode0xC5()
{
    // PUSH BC
    pushStack(BC);
}

void Processor::OPCode0xC6()
{
    // ADD A, d8
    // TODO fix flags
    register8_t data = getCurrentData();
    register8_t data_a = A->getValue();

    data_a += data;

    A->setValue(data_a);
}

void Processor::OPCode0xC7()
{
    // RST 00H
    pushStack(program_counter);
    program_counter->setValue(0x0000);
}

void Processor::OPCode0xC8()
{
    // RET Z
    if (getFlagZ())
    {
        popStack(program_counter);
    }
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
