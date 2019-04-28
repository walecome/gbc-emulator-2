#include <iostream>

#include "InstructionDecoder.hh"

void InstructionDecoder::OPCode0x00() {
    // NOP
    std::cout << "NOP" << std::endl;
}

void InstructionDecoder::OPCode0x01() {
    // LD BC, d16
    // TODO double check
    loadRegister(C);
    loadRegister(B);
}

void InstructionDecoder::OPCode0x02() {
    // LD (BC), A
    // TODO double check
    loadIntoMemory(BC, A);
}

void InstructionDecoder::OPCode0x03() {
    // INC BC
    BC->increment();
}

void InstructionDecoder::OPCode0x04() {
    // INC B
    incrementRegister(B);
}

void InstructionDecoder::OPCode0x05() {
    // DEC B
    decrementRegister(B);
}

void InstructionDecoder::OPCode0x06() {
    // LD B, d8
    loadRegister(B);
}

void InstructionDecoder::OPCode0x07() {
    // RLCA
    // TODO double check
    rlcRegister(A);
}

void InstructionDecoder::OPCode0x08() {
    // LD (a16), SP
    // TODO double check
    register8_t data_low = getCurrentData();
    register8_t data_high = getCurrentData();

    SP->getLowRegister()->setValue(data_low);
    SP->getHighRegister()->setValue(data_high);
}

void InstructionDecoder::OPCode0x09() {
    // ADD HL, BC

    register16_t value_hl = HL->getValue();

    register16_t value_bc = BC->getValue();

    value_hl += value_bc;

    HL->setValue(value_hl);
}

void InstructionDecoder::OPCode0x0A() {
    // LD A, (BC)
    loadFromMemory(A, BC);
}

void InstructionDecoder::OPCode0x0B() {
    // DEC BC
    BC->decrement();
}

void InstructionDecoder::OPCode0x0C() {
    // INC C
    C->increment();
}

void InstructionDecoder::OPCode0x0D() {
    // DEC C
    C->decrement();
}

void InstructionDecoder::OPCode0x0E() {
    // LD C, d8
    loadRegister(C);
}

void InstructionDecoder::OPCode0x0F() {
    // RRCA
    // TODO double check
    rrcRegister(A);
}

void InstructionDecoder::OPCode0x10() {
    // STOP 0
    // TODO fix
    std::cout << "STOP" << std::endl;
}

void InstructionDecoder::OPCode0x11() {
    // LD DE, d16
    // TODO double check
    loadRegister(E);
    loadRegister(D);
}

void InstructionDecoder::OPCode0x12() {
    // LD (DE), A
    loadIntoMemory(DE, A);
}

void InstructionDecoder::OPCode0x13() {
    // INC DE
    DE->increment();
}

void InstructionDecoder::OPCode0x14() {
    // INC D
    D->increment();
}

void InstructionDecoder::OPCode0x15() {
    // DEC D
    D->decrement();
}

void InstructionDecoder::OPCode0x16() {
    // LD D, d8
    loadRegister(D);
}

void InstructionDecoder::OPCode0x17() {
    // RLA
    // TODO double check
    rlRegister(A);
}

void InstructionDecoder::OPCode0x18() {
    // JR d8
    performJump();
}

void InstructionDecoder::OPCode0x19() {
    // ADD HL, DE
    register16_t value_hl = HL->getValue();
    register16_t value_de = DE->getValue();

    value_hl += value_de;

    HL->setValue(value_hl);
}

void InstructionDecoder::OPCode0x1A() {
    // LD A, (DE)
    loadFromMemory(A, DE);
}

void InstructionDecoder::OPCode0x1B() {
    // DEC DE
    DE->decrement();
}

void InstructionDecoder::OPCode0x1C() {
    // INC E
    E->increment();
}

void InstructionDecoder::OPCode0x1D() {
    // DEC E
    E->decrement();
}

void InstructionDecoder::OPCode0x1E() {
    // LD E, d8
    loadRegister(E);
}

void InstructionDecoder::OPCode0x1F() {
    // RRA
    // TODO double check
    rrRegister(A);
}

void InstructionDecoder::OPCode0x20() {
    // JR NZ, r8
    // TODO double check
    if (!cpu->getFlagZ()) {
        performJump();
    }
}

void InstructionDecoder::OPCode0x21() {
    // LD HL, d16
    loadRegister(H);
    loadRegister(L);
}

void InstructionDecoder::OPCode0x22() {
    // LD (HL+), A
    // TODO double check
    loadIntoMemory(HL, A);
    HL->increment();
}

void InstructionDecoder::OPCode0x23() {
    // INC HL
    HL->increment();
}

void InstructionDecoder::OPCode0x24() {
    // INC H
    H->increment();
}

void InstructionDecoder::OPCode0x25() {
    // DEC H
    H->decrement();
}

void InstructionDecoder::OPCode0x26() {
    // LD H, d8
    loadRegister(H);
}

void InstructionDecoder::OPCode0x27() {
    // DAA
    // TODO fix
}

void InstructionDecoder::OPCode0x28() {
    // JR Z, r8
    // TODO double check

    if (cpu->getFlagZ()) {
        performJump();
    }
}

void InstructionDecoder::OPCode0x29() {
    // ADD HL, HL
    register16_t data_hl = HL->getValue();

    data_hl *= 2;

    HL->setValue(data_hl);
}

void InstructionDecoder::OPCode0x2A() {
    // LD A, (HL+)
    // TODO double check
    loadFromMemory(A, HL);
    HL->increment();
}

void InstructionDecoder::OPCode0x2B() {
    // DEC HL
    HL->decrement();
}

void InstructionDecoder::OPCode0x2C() {
    // INC L
    L->increment();
}

void InstructionDecoder::OPCode0x2D() {
    // DEC L
    L->decrement();
}

void InstructionDecoder::OPCode0x2E() {
    // LD L, d8
    loadRegister(L);
}

void InstructionDecoder::OPCode0x2F() {
    // CPL
    A->setValue(~A->getValue());
    cpu->setFlagN();
    cpu->setFlagH();
}

void InstructionDecoder::OPCode0x30() {
    // JR NC, r8
    if (!cpu->getFlagC()) {
        performJump();
    }
}

void InstructionDecoder::OPCode0x31() {
    // LD SP, d16
    loadRegister(SP->getLowRegister());
    loadRegister(SP->getHighRegister());
}

void InstructionDecoder::OPCode0x32() {
    // LD (HL-), A
    // TODO double check
    loadIntoMemory(HL, A);
    HL->decrement();
}

void InstructionDecoder::OPCode0x33() {
    // INC SP
    SP->increment();
}

void InstructionDecoder::OPCode0x34() {
    // INC (HL)
    register16_t address = RAM_DATA_OFFSET + HL->getValue();
    byte_t data = ram->getData(address);

    ++data;

    ram->setData(address, data);
}

void InstructionDecoder::OPCode0x35() {
    // DEC (HL)
    register16_t address = RAM_DATA_OFFSET + HL->getValue();
    byte_t data = ram->getData(address);

    --data;

    ram->setData(address, data);
}

void InstructionDecoder::OPCode0x36() {
    // LD (HL), d8
    byte_t data = getCurrentData();
    register16_t address = RAM_DATA_OFFSET + HL->getValue();

    ram->setData(address, data);
}

void InstructionDecoder::OPCode0x37() {
    // SCF
    cpu->resetFlagN();
    cpu->resetFlagH();
    cpu->setFlagC();
}

void InstructionDecoder::OPCode0x38() {
    // JR C, r8
    if (cpu->getFlagC()) {
        performJump();
    }
}

void InstructionDecoder::OPCode0x39() {
    // ADD HL, SP
    register16_t data_hl = HL->getValue();
    register16_t data_sp = SP->getValue();

    data_hl += data_sp;

    HL->setValue(data_hl);
}

void InstructionDecoder::OPCode0x3A() {
    // LD A, (HL-)
    // TODO double check
    loadFromMemory(A, HL);
    HL->decrement();
}

void InstructionDecoder::OPCode0x3B() {
    // DEC SP
    SP->decrement();
}

void InstructionDecoder::OPCode0x3C() {
    // INC A
    A->increment();
}

void InstructionDecoder::OPCode0x3D() {
    // DEC A
    A->decrement();
}

void InstructionDecoder::OPCode0x3E() {
    // LD A, d8
    loadRegister(A);
}

void InstructionDecoder::OPCode0x3F() {
    // CCF
    // Invert C flag
    if (cpu->getFlagC())
        cpu->resetFlagC();
    else
        cpu->setFlagC();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::OPCode0x40() {
    // LD B, B
    // Do nothing...
}

void InstructionDecoder::OPCode0x41() {
    // LD B, C
    copyRegister(B, C);
}

void InstructionDecoder::OPCode0x42() {
    // LD B, D
    copyRegister(B, D);
}

void InstructionDecoder::OPCode0x43() {
    // LD B, E
    copyRegister(B, E);
}

void InstructionDecoder::OPCode0x44() {
    // LD B, H
    copyRegister(B, H);
}

void InstructionDecoder::OPCode0x45() {
    // LD B, L
    copyRegister(B, L);
}

void InstructionDecoder::OPCode0x46() {
    // LD B, (HL)
    loadFromMemory(B, HL);
}

void InstructionDecoder::OPCode0x47() {
    // LD B, A
    copyRegister(B, A);
}

void InstructionDecoder::OPCode0x48() {
    // LD C, B
    copyRegister(C, B);
}

void InstructionDecoder::OPCode0x49() {
    // LD C, C
    // Do nothing...
}

void InstructionDecoder::OPCode0x4A() {
    // LD C, D
    copyRegister(C, D);
}

void InstructionDecoder::OPCode0x4B() {
    // LD C, E
    copyRegister(C, E);
}

void InstructionDecoder::OPCode0x4C() {
    // LD C, H
    copyRegister(C, H);
}

void InstructionDecoder::OPCode0x4D() {
    // LD C, L
    copyRegister(C, L);
}

void InstructionDecoder::OPCode0x4E() {
    // LD C, (HL)
    loadFromMemory(C, HL);
}

void InstructionDecoder::OPCode0x4F() {
    // LD C, A
    copyRegister(C, A);
}

void InstructionDecoder::OPCode0x50() {
    // LD D, B
    copyRegister(D, B);
}

void InstructionDecoder::OPCode0x51() {
    // LD D, C
    copyRegister(D, C);
}

void InstructionDecoder::OPCode0x52() {
    // LD D, D
    // Do nothing...
}

void InstructionDecoder::OPCode0x53() {
    // LD D, E
    copyRegister(D, E);
}

void InstructionDecoder::OPCode0x54() {
    // LD D, H
    copyRegister(D, H);
}

void InstructionDecoder::OPCode0x55() {
    // LD D, L
    copyRegister(D, L);
}

void InstructionDecoder::OPCode0x56() {
    // LD D, (HL)
    loadFromMemory(D, HL);
}

void InstructionDecoder::OPCode0x57() {
    // LD D, A
    copyRegister(D, A);
}

void InstructionDecoder::OPCode0x58() {
    // LD E, B
    copyRegister(E, B);
}

void InstructionDecoder::OPCode0x59() {
    // LD E, C
    copyRegister(E, C);
}

void InstructionDecoder::OPCode0x5A() {
    // LD E, D
    copyRegister(E, D);
}

void InstructionDecoder::OPCode0x5B() {
    // LD E, E
    // Do nothing...
}

void InstructionDecoder::OPCode0x5C() {
    // LD E, H
    copyRegister(E, H);
}

void InstructionDecoder::OPCode0x5D() {
    // LD E, L
    copyRegister(E, L);
}

void InstructionDecoder::OPCode0x5E() {
    // LD E, (HL)
    loadFromMemory(E, HL);
}

void InstructionDecoder::OPCode0x5F() {
    // LD E, A
    copyRegister(E, A);
}

void InstructionDecoder::OPCode0x60() {
    // LD H, B
    copyRegister(H, B);
}

void InstructionDecoder::OPCode0x61() {
    // LD H, C
    copyRegister(H, C);
}

void InstructionDecoder::OPCode0x62() {
    // LD H, D
    copyRegister(H, D);
}

void InstructionDecoder::OPCode0x63() {
    // LD H, E
    copyRegister(H, E);
}

void InstructionDecoder::OPCode0x64() {
    // LD H, H
    // Do nothing...
}

void InstructionDecoder::OPCode0x65() {
    // LD H, L
    copyRegister(H, L);
}

void InstructionDecoder::OPCode0x66() {
    // LD H, (HL)
    loadFromMemory(H, HL);
}

void InstructionDecoder::OPCode0x67() {
    // LD H, A
    copyRegister(H, A);
}

void InstructionDecoder::OPCode0x68() {
    // LD L, B
    copyRegister(L, B);
}

void InstructionDecoder::OPCode0x69() {
    // LD L, C
    copyRegister(L, C);
}

void InstructionDecoder::OPCode0x6A() {
    // LD L, D
    copyRegister(L, D);
}

void InstructionDecoder::OPCode0x6B() {
    // LD L, E
    copyRegister(L, E);
}

void InstructionDecoder::OPCode0x6C() {
    // LD L, H
    copyRegister(L, H);
}

void InstructionDecoder::OPCode0x6D() {
    // LD L, L
    // Do nothing...
}

void InstructionDecoder::OPCode0x6E() {
    // LD L, (HL)
    loadFromMemory(L, HL);
}

void InstructionDecoder::OPCode0x6F() {
    // LD L, A
    copyRegister(L, A);
}

void InstructionDecoder::OPCode0x70() {
    // LD (HL), B
    loadIntoMemory(HL, B);
}

void InstructionDecoder::OPCode0x71() {
    // LD (HL), C
    loadIntoMemory(HL, C);
}

void InstructionDecoder::OPCode0x72() {
    // LD (HL), D
    loadIntoMemory(HL, D);
}

void InstructionDecoder::OPCode0x73() {
    // LD (HL), E
    loadIntoMemory(HL, E);
}

void InstructionDecoder::OPCode0x74() {
    // LD (HL), H
    loadIntoMemory(HL, H);
}

void InstructionDecoder::OPCode0x75() {
    // LD (HL), L
    loadIntoMemory(HL, L);
}

void InstructionDecoder::OPCode0x76() {
    // HALT
    // TODO stop??
}

void InstructionDecoder::OPCode0x77() {
    // LD (HL), A
    loadIntoMemory(HL, A);
}

void InstructionDecoder::OPCode0x78() {
    // LD A, B
    copyRegister(A, B);
}

void InstructionDecoder::OPCode0x79() {
    // LD A, C
    copyRegister(A, C);
}

void InstructionDecoder::OPCode0x7A() {
    // LD A, D
    copyRegister(A, D);
}

void InstructionDecoder::OPCode0x7B() {
    // LD A, E
    copyRegister(A, E);
}

void InstructionDecoder::OPCode0x7C() {
    // LD A, H
    copyRegister(A, H);
}

void InstructionDecoder::OPCode0x7D() {
    // LD A, L
    copyRegister(A, L);
}

void InstructionDecoder::OPCode0x7E() {
    // LD A, (HL)
    loadFromMemory(A, HL);
}

void InstructionDecoder::OPCode0x7F() {
    // LD A, A
    // Do nothing...
}

void InstructionDecoder::OPCode0x80() {
    // ADD A, B
    addRegisters(A, B);
}

void InstructionDecoder::OPCode0x81() {
    // ADD A, C
    addRegisters(A, C);
}

void InstructionDecoder::OPCode0x82() {
    // ADD A, D
    addRegisters(A, D);
}

void InstructionDecoder::OPCode0x83() {
    // ADD A, E
    addRegisters(A, E);
}

void InstructionDecoder::OPCode0x84() {
    // ADD A, H
    addRegisters(A, H);
}

void InstructionDecoder::OPCode0x85() {
    // ADD A, L
    addRegisters(A, L);
}

void InstructionDecoder::OPCode0x86() {
    // ADD A, (HL)
    register8_t data_a = A->getValue();
    register16_t address = RAM_DATA_OFFSET + HL->getValue();
    register8_t data_mem = ram->getData(address);

    register8_t result = data_a + data_mem;

    A->setValue(result);
}

void InstructionDecoder::OPCode0x87() {
    // ADD A, A
    addRegisters(A, A);
}

void InstructionDecoder::OPCode0x88() {
    // ADC A, B
    addWithCarry(A, B);
}

void InstructionDecoder::OPCode0x89() {
    // ADC A, C
    addWithCarry(A, C);
}

void InstructionDecoder::OPCode0x8A() {
    // ADC A, D
    addWithCarry(A, D);
}

void InstructionDecoder::OPCode0x8B() {
    // ADC A, E
    addWithCarry(A, E);
}

void InstructionDecoder::OPCode0x8C() {
    // ADC A, H
    addWithCarry(A, H);
}

void InstructionDecoder::OPCode0x8D() {
    // ADC A, L
    addWithCarry(A, L);
}

void InstructionDecoder::OPCode0x8E() {
    // ADC A, (HL)
    // TODO fix
}

void InstructionDecoder::OPCode0x8F() {
    // ADC A, A
    addWithCarry(A, A);
}

void InstructionDecoder::OPCode0x90() {
    // SUB B
    subRegisters(B);
}

void InstructionDecoder::OPCode0x91() {
    // SUB C
    subRegisters(C);
}

void InstructionDecoder::OPCode0x92() {
    // SUB D
    subRegisters(D);
}

void InstructionDecoder::OPCode0x93() {
    // SUB E
    subRegisters(E);
}

void InstructionDecoder::OPCode0x94() {
    // SUB H
    subRegisters(H);
}

void InstructionDecoder::OPCode0x95() {
    // SUB L
    subRegisters(L);
}

void InstructionDecoder::OPCode0x96() {
    // SUB (HL)
    // TODO double check this!!
    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };
    loadFromMemory(tmp, HL);

    subRegisters(tmp);
}

void InstructionDecoder::OPCode0x97() {
    // SUB A
    subRegisters(A);
}

void InstructionDecoder::OPCode0x98() {
    // SBC A, B
    subWithCarry(B);
}

void InstructionDecoder::OPCode0x99() {
    // SBC A, C
    subWithCarry(C);
}

void InstructionDecoder::OPCode0x9A() {
    // SBC A, D
    subWithCarry(D);
}

void InstructionDecoder::OPCode0x9B() {
    // SBC A, E
    subWithCarry(E);
}

void InstructionDecoder::OPCode0x9C() {
    // SBC A, H
    subWithCarry(H);
}

void InstructionDecoder::OPCode0x9D() {
    // SBC A, L
    subWithCarry(L);
}

void InstructionDecoder::OPCode0x9E() {
    // SBC A, (HL)
    // TODO double check

    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };

    loadFromMemory(tmp, HL);

    subWithCarry(tmp);
}

void InstructionDecoder::OPCode0x9F() {
    // SBC A, A
    subWithCarry(A);
}

void InstructionDecoder::OPCode0xA0() {
    // AND B
    andRegisters(B);
}

void InstructionDecoder::OPCode0xA1() {
    // AND C
    andRegisters(C);
}

void InstructionDecoder::OPCode0xA2() {
    // AND D
    andRegisters(D);
}

void InstructionDecoder::OPCode0xA3() {
    // AND E
    andRegisters(E);
}

void InstructionDecoder::OPCode0xA4() {
    // AND H
    andRegisters(H);
}

void InstructionDecoder::OPCode0xA5() {
    // AND L
    andRegisters(L);
}

void InstructionDecoder::OPCode0xA6() {
    // AND (HL)
    register16_t address = RAM_DATA_OFFSET + HL->getValue();

    register8_t data_mem = ram->getData(address);

    register8_t data_a = A->getValue();

    data_a = data_a & data_mem;

    A->setValue(data_a);
}

void InstructionDecoder::OPCode0xA7() {
    // AND A
    // Do nothing...
}

void InstructionDecoder::OPCode0xA8() {
    // XOR B
    xorRegisters(B);
}

void InstructionDecoder::OPCode0xA9() {
    // XOR C
    xorRegisters(C);
}

void InstructionDecoder::OPCode0xAA() {
    // XOR D
    xorRegisters(D);
}

void InstructionDecoder::OPCode0xAB() {
    // XOR E
    xorRegisters(E);
}

void InstructionDecoder::OPCode0xAC() {
    // XOR H
    xorRegisters(H);
}

void InstructionDecoder::OPCode0xAD() {
    // XOR L
    xorRegisters(L);
}

void InstructionDecoder::OPCode0xAE() {
    // XOR (HL)
    // TODO double check
    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };

    loadFromMemory(tmp, HL);
    xorRegisters(tmp);
}

void InstructionDecoder::OPCode0xAF() {
    // XOR A
    xorRegisters(A);
}

void InstructionDecoder::OPCode0xB0() {
    // OR B
    orRegisters(B);
}

void InstructionDecoder::OPCode0xB1() {
    // OR C
    orRegisters(C);
}

void InstructionDecoder::OPCode0xB2() {
    // OR D
    orRegisters(D);
}

void InstructionDecoder::OPCode0xB3() {
    // OR E
    orRegisters(E);
}

void InstructionDecoder::OPCode0xB4() {
    // OR H
    orRegisters(H);
}

void InstructionDecoder::OPCode0xB5() {
    // OR L
    orRegisters(L);
}

void InstructionDecoder::OPCode0xB6() {
    // OR (HL)
    // TODO double check
    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };
    loadFromMemory(tmp, HL);
    orRegisters(tmp);
}

void InstructionDecoder::OPCode0xB7() {
    // OR A
    // Do nothing...
}

void InstructionDecoder::OPCode0xB8() {
    // CP B
    cmpRegisters(B);
}

void InstructionDecoder::OPCode0xB9() {
    // CP C
    cmpRegisters(C);
}

void InstructionDecoder::OPCode0xBA() {
    // CP D
    cmpRegisters(D);
}

void InstructionDecoder::OPCode0xBB() {
    // CP E
    cmpRegisters(E);
}

void InstructionDecoder::OPCode0xBC() {
    // CP H
    cmpRegisters(H);
}

void InstructionDecoder::OPCode0xBD() {
    // CP L
    cmpRegisters(L);
}

void InstructionDecoder::OPCode0xBE() {
    // CP (HL)
    byte_t data = ram->getData(HL->getValue());

    if (data == A->getValue())
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    // TODO half carry flag
    cpu->setFlagN();

    if (A->getValue() < data)
        cpu->setFlagC();
    else
        cpu->resetFlagC();
}

void InstructionDecoder::OPCode0xBF() {
    // CP A
    cmpRegisters(A);
}

void InstructionDecoder::OPCode0xC0() {
    // RET NZ
    if (!cpu->getFlagZ()) {
        popStack(PC);
    }
}

void InstructionDecoder::OPCode0xC1() {
    // POP BC
    popStack(BC);
}

void InstructionDecoder::OPCode0xC2() {
    // JP NZ, a16
    if (!cpu->getFlagZ()) {
        jumpIm16bit();
    } else {
        // Skips these of we don't jump
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xC3() {
    // JP a16
    jumpIm16bit();
}

void InstructionDecoder::OPCode0xC4() {
    // CALL NZ, a16
    if (!cpu->getFlagZ()) {
        byte_t data_low = getCurrentData();
        byte_t data_high = getCurrentData();

        pushStack(PC);

        PC->getLowRegister()->setValue(data_low);
        PC->getHighRegister()->setValue(data_high);
    } else {
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xC5() {
    // PUSH BC
    pushStack(BC);
}

void InstructionDecoder::OPCode0xC6() {
    // ADD A, d8
    // TODO fix flags
    register8_t data = getCurrentData();
    register8_t data_a = A->getValue();

    data_a += data;

    A->setValue(data_a);
}

void InstructionDecoder::OPCode0xC7() {
    // RST 00H
    pushStack(PC);
    PC->setValue(0x0000);
}

void InstructionDecoder::OPCode0xC8() {
    // RET Z
    if (cpu->getFlagZ()) {
        popStack(PC);
    }
}

void InstructionDecoder::OPCode0xC9() {
    // RET
    popStack(PC);
}

void InstructionDecoder::OPCode0xCA() {
    // JP Z, a16
    if (cpu->getFlagZ()) {
        jumpIm16bit();
    } else {
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xCB() {
    // Prefix CB
}

void InstructionDecoder::OPCode0xCC() {
    // CALL Z, a16
    if (cpu->getFlagZ()) {
        byte_t data_low = getCurrentData();
        byte_t data_high = getCurrentData();

        pushStack(PC);

        PC->getLowRegister()->setValue(data_low);
        PC->getHighRegister()->setValue(data_high);
    } else {
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xCD() {
    // CALL a16
    byte_t data_low = getCurrentData();
    byte_t data_high = getCurrentData();

    pushStack(PC);

    PC->getLowRegister()->setValue(data_low);
    PC->getHighRegister()->setValue(data_high);
}

void InstructionDecoder::OPCode0xCE() {
    // ADC A, d8
    // TODO double check...
    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };
    tmp->setValue(getCurrentData());
    addWithCarry(A, tmp);
}

void InstructionDecoder::OPCode0xCF() {
    // RST 08H
    pushStack(PC);
    PC->setValue(0x0008);
}

void InstructionDecoder::OPCode0xD0() {
    // RET NC
    if (!cpu->getFlagC()) {
        popStack(PC);
    }
}

void InstructionDecoder::OPCode0xD1() {
    // POP DE
    popStack(DE);
}

void InstructionDecoder::OPCode0xD2() {
    // JP NC, a16
    if (!cpu->getFlagC()) {
        jumpIm16bit();
    } else {
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xD3() {
    // No instruction
    throw 1;
}

void InstructionDecoder::OPCode0xD4() {
    // CALL NC, a16
    if (!cpu->getFlagC()) {
        byte_t data_low = getCurrentData();
        byte_t data_high = getCurrentData();

        pushStack(PC);

        PC->getLowRegister()->setValue(data_low);
        PC->getHighRegister()->setValue(data_high);
    } else {
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xD5() {
    // PUSH DE
    pushStack(DE);
}

void InstructionDecoder::OPCode0xD6() {
    // SUB d8
    // TODO double check..
    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };
    tmp->setValue(getCurrentData());
    subRegisters(tmp);
}

void InstructionDecoder::OPCode0xD7() {
    // RST 10H
    pushStack(PC);
    PC->setValue(0x0010);
}

void InstructionDecoder::OPCode0xD8() {
    // RET C
    if (cpu->getFlagC()) {
        popStack(PC);
    }
}

void InstructionDecoder::OPCode0xD9() {
    // RETI
    // TODO fix interupts
}

void InstructionDecoder::OPCode0xDA() {
    // JP C, a16
    if (cpu->getFlagC()) {
        jumpIm16bit();
    } else {
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xDB() {
    // No instruction
    throw 1;
}

void InstructionDecoder::OPCode0xDC() {
    // CALL C, a16
    if (cpu->getFlagC()) {
        byte_t data_low = getCurrentData();
        byte_t data_high = getCurrentData();

        pushStack(PC);

        PC->getLowRegister()->setValue(data_low);
        PC->getHighRegister()->setValue(data_high);
    } else {
        PC->increment();
        PC->increment();
    }
}

void InstructionDecoder::OPCode0xDD() {
    // No instruction
    throw 1;
}

void InstructionDecoder::OPCode0xDE() {
    // SBC A, d8
    // TODO double check
    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };
    tmp->setValue(getCurrentData());
    subWithCarry(tmp);
}

void InstructionDecoder::OPCode0xDF() {
    // RST 18H
    pushStack(PC);
    PC->setValue(0x0018);
}

void InstructionDecoder::OPCode0xE0() {
    // LDH (a8), A
    register16_t address = 0xFF00 + getCurrentData();
    ram->setData(address, A->getValue());
}

void InstructionDecoder::OPCode0xE1() {
    // POP HL
    popStack(HL);
}

void InstructionDecoder::OPCode0xE2() {
    // LD (C), A
    register16_t address = 0xFF00 + C->getValue();
    ram->setData(address, A->getValue());
}

void InstructionDecoder::OPCode0xE3() {
    // No instruction
    throw 1;
}

void InstructionDecoder::OPCode0xE4() {
    // No instruction
    throw 1;
}

void InstructionDecoder::OPCode0xE5() {
    // PUSH HL
    pushStack(HL);
}

void InstructionDecoder::OPCode0xE6() {
    // AND d8
    A->setValue(A->getValue() & getCurrentData());
}

void InstructionDecoder::OPCode0xE7() {
    // RST 20H
    pushStack(PC);
    PC->setValue(0x0020);
}

void InstructionDecoder::OPCode0xE8() {
    // ADD SP, r8
    // TODO FIX (signed)
}

void InstructionDecoder::OPCode0xE9() {
    // JP (HL)
    // TODO double check
    PC->getLowRegister()->setValue(HL->getLowRegister()->getValue());

    PC->getHighRegister()->setValue(HL->getHighRegister()->getValue());
}

void InstructionDecoder::OPCode0xEA() {
    // LD (a16), A
    // TODO double check
    ptr<Register16bit> tmp { std::make_shared<Register16bit>("tmp") };
    tmp->getLowRegister()->setValue(getCurrentData());
    tmp->getHighRegister()->setValue(getCurrentData());

    loadIntoMemory(tmp, A);
}

void InstructionDecoder::OPCode0xEB() {
    // No instruction
    throw 0xEB;
}

void InstructionDecoder::OPCode0xEC() {
    // No instruction
    throw 0xEC;
}

void InstructionDecoder::OPCode0xED() {
    // No instruction
    throw 0xED;
}

void InstructionDecoder::OPCode0xEE() {
    // XOR d8
    A->setValue(A->getValue() ^ getCurrentData());
}

void InstructionDecoder::OPCode0xEF() {
    // RST 28H
    pushStack(PC);
    PC->setValue(0x0028);
}

void InstructionDecoder::OPCode0xF0() {
    // LDH A, (a8)
    register16_t address = 0xFF00 + getCurrentData();
    byte_t data = ram->getData(address);
    A->setValue(data);
}

void InstructionDecoder::OPCode0xF1() {
    // POP AF
    popStack(AF);
}

void InstructionDecoder::OPCode0xF2() {
    // LD A, (C)
    register16_t address = 0xFF00 + C->getValue();
    byte_t data = ram->getData(address);
    A->setValue(data);
}

void InstructionDecoder::OPCode0xF3() {
    // DI
    // TODO disable interupts according to manual
}

void InstructionDecoder::OPCode0xF4() {
    // No instruction
    throw 0xF4;
}

void InstructionDecoder::OPCode0xF5() {
    // PUSH AF
    pushStack(AF);
}

void InstructionDecoder::OPCode0xF6() {
    // OR d8
    A->setValue(A->getValue() | getCurrentData());
}

void InstructionDecoder::OPCode0xF7() {
    // RST 30H
    pushStack(PC);
    PC->setValue(0x0030);
}

void InstructionDecoder::OPCode0xF8() {
    // LD HL, SP+r8
    // TODO double check
    int sp_value = SP->getValue();
    int8_t r8 = static_cast<int8_t>(getCurrentData());

    sp_value = sp_value + r8;

    HL->setValue(static_cast<register16_t>(sp_value));
}

void InstructionDecoder::OPCode0xF9() {
    // LD SP, HL
    copyRegister(SP->getLowRegister(), HL->getLowRegister());
    copyRegister(SP->getHighRegister(), HL->getHighRegister());
}

void InstructionDecoder::OPCode0xFA() {
    // LD A, (a16)
    // TODO double check
    std::shared_ptr<Register16bit> tmp { std::make_shared<Register16bit>(
        "tmp") };
    tmp->getLowRegister()->setValue(getCurrentData());
    tmp->getHighRegister()->setValue(getCurrentData());

    loadFromMemory(A, tmp);
}

void InstructionDecoder::OPCode0xFB() {
    // EI
    // TODO enable interupts according to manual
}

void InstructionDecoder::OPCode0xFC() {
    // No instruction
    throw 0xFC;
}

void InstructionDecoder::OPCode0xFD() {
    // No instruction
    throw 0xFD;
}

void InstructionDecoder::OPCode0xFE() {
    // CP d8
    // Double check
    ptr<Register8bit> tmp { std::make_shared<Register8bit>("tmp") };
    tmp->setValue(getCurrentData());
    cmpRegisters(tmp);
}

void InstructionDecoder::OPCode0xFF() {
    // RST 38H
    pushStack(PC);
    PC->setValue(0x0038);
}
