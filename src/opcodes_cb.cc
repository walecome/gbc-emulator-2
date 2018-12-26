#include "Processor.hh"
void Processor::OPCodeCB0x00()
{
    // RLC B
    rlcRegister(B);
}

void Processor::OPCodeCB0x01()
{
    // RLC C
    rlcRegister(C);
}

void Processor::OPCodeCB0x02()
{
    // RLC D
    rlcRegister(D);
}

void Processor::OPCodeCB0x03()
{
    // RLC E
    rlcRegister(E);
}

void Processor::OPCodeCB0x04()
{
    // RLC H
    rlcRegister(H);
}

void Processor::OPCodeCB0x05()
{
    // RLC L
    rlcRegister(L);
}

void Processor::OPCodeCB0x06()
{
    // RLC (HL)
    // TODO fix
}

void Processor::OPCodeCB0x07()
{
    // RLC A
    rlcRegister(A);
}

void Processor::OPCodeCB0x08()
{
    // RRC B
    rrcRegister(B);
}

void Processor::OPCodeCB0x09()
{
    // RRC C
    rrcRegister(C);
}

void Processor::OPCodeCB0x0A()
{
    // RRC D
    rrcRegister(D);
}

void Processor::OPCodeCB0x0B()
{
    // RRC E
    rrcRegister(E);
}

void Processor::OPCodeCB0x0C()
{
    // RRC H
    rrcRegister(H);
}

void Processor::OPCodeCB0x0D()
{
    // RRC L
    rrcRegister(L);
}

void Processor::OPCodeCB0x0E()
{
    // RRC (HL)
    // TODO fix
}

void Processor::OPCodeCB0x0F()
{
    // RRC A
    rrcRegister(A);
}

void Processor::OPCodeCB0x10()
{
    // RL B
    rlRegister(B);
}

void Processor::OPCodeCB0x11()
{
    // RL C
    rlRegister(C);
}

void Processor::OPCodeCB0x12()
{
    // RL D
    rlRegister(D);
}

void Processor::OPCodeCB0x13()
{
    // RL E
    rlRegister(E);
}

void Processor::OPCodeCB0x14()
{
    // RL H
    rlRegister(H);
}

void Processor::OPCodeCB0x15()
{
    // RL L
    rlRegister(L);
}

void Processor::OPCodeCB0x16()
{
    // RL (HL)
    // TODO fix
}

void Processor::OPCodeCB0x17()
{
    // RL A
    rlRegister(A);
}

void Processor::OPCodeCB0x18()
{
    // RR B
    rrRegister(B);
}

void Processor::OPCodeCB0x19()
{
    // RR C
    rrRegister(C);
}

void Processor::OPCodeCB0x1A()
{
    // RR D
    rrRegister(D);
}

void Processor::OPCodeCB0x1B()
{
    // RR E
    rrRegister(E);
}

void Processor::OPCodeCB0x1C()
{
    // RR H
    rrRegister(H);
}

void Processor::OPCodeCB0x1D()
{
    // RR L
    rrRegister(L);
}

void Processor::OPCodeCB0x1E()
{
    // RR (HL)
    // TODO fix
}

void Processor::OPCodeCB0x1F()
{
    // RR A
    rrRegister(A);
}

void Processor::OPCodeCB0x20()
{
    // SLA B
    slaRegister(B);
}

void Processor::OPCodeCB0x21()
{
    // SLA C
    slaRegister(C);
}

void Processor::OPCodeCB0x22()
{
    // SLA D
    slaRegister(D);
}

void Processor::OPCodeCB0x23()
{
    // SLA E
    slaRegister(E);
}

void Processor::OPCodeCB0x24()
{
    // SLA H
    slaRegister(H);
}

void Processor::OPCodeCB0x25()
{
    // SLA L
    slaRegister(L);
}

void Processor::OPCodeCB0x26()
{
    // SLA (HL)
    // TODO fix
}

void Processor::OPCodeCB0x27()
{
    // SLA A
    slaRegister(A);
}

void Processor::OPCodeCB0x28()
{
    // SRA B
    sraRegister(B);
}

void Processor::OPCodeCB0x29()
{
    // SRA C
    sraRegister(C);
}

void Processor::OPCodeCB0x2A()
{
    // SRA D
    sraRegister(D);
}

void Processor::OPCodeCB0x2B()
{
    // SRA E
    sraRegister(E);
}

void Processor::OPCodeCB0x2C()
{
    // SRA H
    sraRegister(H);
}

void Processor::OPCodeCB0x2D()
{
    // SRA L
    sraRegister(L);
}

void Processor::OPCodeCB0x2E()
{
    // SRA (HL)
    // TODO fix
}

void Processor::OPCodeCB0x2F()
{
    // SRA A
    sraRegister(A);
}

void Processor::OPCodeCB0x30()
{
    // SWAP B
    swapNibbles(B);
}

void Processor::OPCodeCB0x31()
{
    // SWAP C
    swapNibbles(C);
}

void Processor::OPCodeCB0x32()
{
    // SWAP D
    swapNibbles(D);
}

void Processor::OPCodeCB0x33()
{
    // SWAP E
    swapNibbles(E);
}

void Processor::OPCodeCB0x34()
{
    // SWAP H
    swapNibbles(H);
}

void Processor::OPCodeCB0x35()
{
    // SWAP L
    swapNibbles(L);
}

void Processor::OPCodeCB0x36()
{
    // SWAP (HL)
    // TODO fix
}

void Processor::OPCodeCB0x37()
{
    // SWAP A
    swapNibbles(A);
}

void Processor::OPCodeCB0x38()
{
    // SRL B
    srlRegister(B);
}

void Processor::OPCodeCB0x39()
{
    // SRL C
    srlRegister(C);
}

void Processor::OPCodeCB0x3A()
{
    // SRL D
    srlRegister(D);
}

void Processor::OPCodeCB0x3B()
{
    // SRL E
    srlRegister(E);
}

void Processor::OPCodeCB0x3C()
{
    // SRL H
    srlRegister(H);
}

void Processor::OPCodeCB0x3D()
{
    // SRL L
    srlRegister(L);
}

void Processor::OPCodeCB0x3E()
{
    // SRL (HL)
    // TODO fix
}

void Processor::OPCodeCB0x3F()
{
    // SRL A
    srlRegister(A);
}

void Processor::OPCodeCB0x40()
{
    // BIT 0, B
    testBit(0, B);
}

void Processor::OPCodeCB0x41()
{
    // BIT 0, C
    testBit(0, C);
}

void Processor::OPCodeCB0x42()
{
    // BIT 0, D
    testBit(0, D);
}

void Processor::OPCodeCB0x43()
{
    // BIT 0, E
    testBit(0, E);
}

void Processor::OPCodeCB0x44()
{
    // BIT 0, H
    testBit(0, H);
}

void Processor::OPCodeCB0x45()
{
    // BIT 0, L
    testBit(0, L);
}

void Processor::OPCodeCB0x46()
{
    // BIT 0, (HL)
    // TODO fix
}

void Processor::OPCodeCB0x47()
{
    // BIT 0, A
    testBit(0, A);
}

void Processor::OPCodeCB0x48()
{
    // BIT 1, B
    testBit(1, B);
}

void Processor::OPCodeCB0x49()
{
    // BIT 1, C
    testBit(1, C);
}

void Processor::OPCodeCB0x4A()
{
    // BIT 1, D
    testBit(1, D);
}

void Processor::OPCodeCB0x4B()
{
    // BIT 1, E
    testBit(1, E);
}

void Processor::OPCodeCB0x4C()
{
    // BIT 1, H
    testBit(1, H);
}

void Processor::OPCodeCB0x4D()
{
    // BIT 1, L
    testBit(1, L);
}

void Processor::OPCodeCB0x4E()
{
    // BIT 1, HL
    // TODO fix
}

void Processor::OPCodeCB0x4F()
{
    // BIT 1, A
    testBit(1, A);
}

void Processor::OPCodeCB0x50()
{
    // BIT 2, B
    testBit(2, B);
}

void Processor::OPCodeCB0x51()
{
    // BIT 2, C
    testBit(2, C);
}

void Processor::OPCodeCB0x52()
{
    // BIT 2, D
    testBit(2, D);
}

void Processor::OPCodeCB0x53()
{
    // BIT 2, E
    testBit(2, E);
}

void Processor::OPCodeCB0x54()
{
    // BIT 2, H
    testBit(2, H);
}

void Processor::OPCodeCB0x55()
{
    // BIT 2, L
    testBit(2, L);
}

void Processor::OPCodeCB0x56()
{
    // BIT 2, (HL)
    // TODO fix
}

void Processor::OPCodeCB0x57()
{
    // BIT 2, A
    testBit(2, A);
}

void Processor::OPCodeCB0x58()
{
    // BIT 3, B
    testBit(3, B);
}

void Processor::OPCodeCB0x59()
{
    // BIT 3, C
    testBit(3, C);
}

void Processor::OPCodeCB0x5A()
{
    // BIT 3, D
    testBit(3, D);
}

void Processor::OPCodeCB0x5B()
{
    // BIT 3, E
    testBit(3, E);
}

void Processor::OPCodeCB0x5C()
{
    // BIT 3, H
    testBit(3, H);
}

void Processor::OPCodeCB0x5D()
{
    // BIT 3, L
    testBit(3, L);
}

void Processor::OPCodeCB0x5E()
{
    // BIT 3, (HL)
    // TODO fix
}

void Processor::OPCodeCB0x5F()
{
    // BIT 3, A
    testBit(3, A);
}

void Processor::OPCodeCB0x60()
{
    // BIT 4, B
    testBit(4, B);
}

void Processor::OPCodeCB0x61()
{
    // BIT 4, C
    testBit(4, C);
}

void Processor::OPCodeCB0x62()
{
    // BIT 4, D
    testBit(4, D);
}

void Processor::OPCodeCB0x63()
{
    // BIT 4, E
    testBit(4, E);
}

void Processor::OPCodeCB0x64()
{
    // BIT 4, H
    testBit(4, H);
}

void Processor::OPCodeCB0x65()
{
    // BIT 4, L
    testBit(4, L);
}

void Processor::OPCodeCB0x66()
{
    // BIT 4, (HL)
    // TODO fix
}

void Processor::OPCodeCB0x67()
{
    // BIT 4, A
    testBit(4, A);
}

void Processor::OPCodeCB0x68()
{
    // BIT 5, B
    testBit(5, B);
}

void Processor::OPCodeCB0x69()
{
    // BIT 5, C
    testBit(5, C);
}

void Processor::OPCodeCB0x6A()
{
    // BIT 5, D
    testBit(5, D);
}

void Processor::OPCodeCB0x6B()
{
    // BIT 5, E
    testBit(5, E);
}

void Processor::OPCodeCB0x6C()
{
    // BIT 5, H
    testBit(5, H);
}

void Processor::OPCodeCB0x6D()
{
    // BIT 5, L
    testBit(5, L);
}

void Processor::OPCodeCB0x6E()
{
    // BIT 5, (HL)
    // TODO fix
}

void Processor::OPCodeCB0x6F()
{
    // BIT 5, A
    testBit(5, A);
}

void Processor::OPCodeCB0x70()
{
    // BIT 6, B
    testBit(6, B);
}

void Processor::OPCodeCB0x71()
{
    // BIT 6, C
    testBit(6, C);
}

void Processor::OPCodeCB0x72()
{
    // BIT 6, D
    testBit(6, D);
}

void Processor::OPCodeCB0x73()
{
    // BIT 6, E
    testBit(6, E);
}

void Processor::OPCodeCB0x74()
{
    // BIT 6, H
    testBit(6, H);
}

void Processor::OPCodeCB0x75()
{
    // BIT 6, L
    testBit(6, L);
}

void Processor::OPCodeCB0x76()
{
    // BIT 6, (HL)
    // TODO fix
}

void Processor::OPCodeCB0x77()
{
    // BIT 6, A
    testBit(6, A);
}

void Processor::OPCodeCB0x78()
{
    // BIT 7, B
    testBit(7, B);
}

void Processor::OPCodeCB0x79()
{
    // BIT 7, C
    testBit(7, C);
}

void Processor::OPCodeCB0x7A()
{
    // BIT 7, D
    testBit(7, D);
}

void Processor::OPCodeCB0x7B()
{
    // BIT 7, E
    testBit(7, E);
}

void Processor::OPCodeCB0x7C()
{
    // BIT 7, H
    testBit(7, H);
}

void Processor::OPCodeCB0x7D()
{
    // BIT 7, L
    testBit(7, L);
}

void Processor::OPCodeCB0x7E()
{
    // BIT 7, (HL)
    // TODO fix
}

void Processor::OPCodeCB0x7F()
{
    // BIT 7, A
    testBit(7, A);
}

void Processor::OPCodeCB0x80()
{
}

void Processor::OPCodeCB0x81()
{
}

void Processor::OPCodeCB0x82()
{
}

void Processor::OPCodeCB0x83()
{
}

void Processor::OPCodeCB0x84()
{
}

void Processor::OPCodeCB0x85()
{
}

void Processor::OPCodeCB0x86()
{
}

void Processor::OPCodeCB0x87()
{
}

void Processor::OPCodeCB0x88()
{
}

void Processor::OPCodeCB0x89()
{
}

void Processor::OPCodeCB0x8A()
{
}

void Processor::OPCodeCB0x8B()
{
}

void Processor::OPCodeCB0x8C()
{
}

void Processor::OPCodeCB0x8D()
{
}

void Processor::OPCodeCB0x8E()
{
}

void Processor::OPCodeCB0x8F()
{
}

void Processor::OPCodeCB0x90()
{
}

void Processor::OPCodeCB0x91()
{
}

void Processor::OPCodeCB0x92()
{
}

void Processor::OPCodeCB0x93()
{
}

void Processor::OPCodeCB0x94()
{
}

void Processor::OPCodeCB0x95()
{
}

void Processor::OPCodeCB0x96()
{
}

void Processor::OPCodeCB0x97()
{
}

void Processor::OPCodeCB0x98()
{
}

void Processor::OPCodeCB0x99()
{
}

void Processor::OPCodeCB0x9A()
{
}

void Processor::OPCodeCB0x9B()
{
}

void Processor::OPCodeCB0x9C()
{
}

void Processor::OPCodeCB0x9D()
{
}

void Processor::OPCodeCB0x9E()
{
}

void Processor::OPCodeCB0x9F()
{
}

void Processor::OPCodeCB0xA0()
{
}

void Processor::OPCodeCB0xA1()
{
}

void Processor::OPCodeCB0xA2()
{
}

void Processor::OPCodeCB0xA3()
{
}

void Processor::OPCodeCB0xA4()
{
}

void Processor::OPCodeCB0xA5()
{
}

void Processor::OPCodeCB0xA6()
{
}

void Processor::OPCodeCB0xA7()
{
}

void Processor::OPCodeCB0xA8()
{
}

void Processor::OPCodeCB0xA9()
{
}

void Processor::OPCodeCB0xAA()
{
}

void Processor::OPCodeCB0xAB()
{
}

void Processor::OPCodeCB0xAC()
{
}

void Processor::OPCodeCB0xAD()
{
}

void Processor::OPCodeCB0xAE()
{
}

void Processor::OPCodeCB0xAF()
{
}

void Processor::OPCodeCB0xB0()
{
}

void Processor::OPCodeCB0xB1()
{
}

void Processor::OPCodeCB0xB2()
{
}

void Processor::OPCodeCB0xB3()
{
}

void Processor::OPCodeCB0xB4()
{
}

void Processor::OPCodeCB0xB5()
{
}

void Processor::OPCodeCB0xB6()
{
}

void Processor::OPCodeCB0xB7()
{
}

void Processor::OPCodeCB0xB8()
{
}

void Processor::OPCodeCB0xB9()
{
}

void Processor::OPCodeCB0xBA()
{
}

void Processor::OPCodeCB0xBB()
{
}

void Processor::OPCodeCB0xBC()
{
}

void Processor::OPCodeCB0xBD()
{
}

void Processor::OPCodeCB0xBE()
{
}

void Processor::OPCodeCB0xBF()
{
}

void Processor::OPCodeCB0xC0()
{
}

void Processor::OPCodeCB0xC1()
{
}

void Processor::OPCodeCB0xC2()
{
}

void Processor::OPCodeCB0xC3()
{
}

void Processor::OPCodeCB0xC4()
{
}

void Processor::OPCodeCB0xC5()
{
}

void Processor::OPCodeCB0xC6()
{
}

void Processor::OPCodeCB0xC7()
{
}

void Processor::OPCodeCB0xC8()
{
}

void Processor::OPCodeCB0xC9()
{
}

void Processor::OPCodeCB0xCA()
{
}

void Processor::OPCodeCB0xCB()
{
}

void Processor::OPCodeCB0xCC()
{
}

void Processor::OPCodeCB0xCD()
{
}

void Processor::OPCodeCB0xCE()
{
}

void Processor::OPCodeCB0xCF()
{
}

void Processor::OPCodeCB0xD0()
{
}

void Processor::OPCodeCB0xD1()
{
}

void Processor::OPCodeCB0xD2()
{
}

void Processor::OPCodeCB0xD3()
{
}

void Processor::OPCodeCB0xD4()
{
}

void Processor::OPCodeCB0xD5()
{
}

void Processor::OPCodeCB0xD6()
{
}

void Processor::OPCodeCB0xD7()
{
}

void Processor::OPCodeCB0xD8()
{
}

void Processor::OPCodeCB0xD9()
{
}

void Processor::OPCodeCB0xDA()
{
}

void Processor::OPCodeCB0xDB()
{
}

void Processor::OPCodeCB0xDC()
{
}

void Processor::OPCodeCB0xDD()
{
}

void Processor::OPCodeCB0xDE()
{
}

void Processor::OPCodeCB0xDF()
{
}

void Processor::OPCodeCB0xE0()
{
}

void Processor::OPCodeCB0xE1()
{
}

void Processor::OPCodeCB0xE2()
{
}

void Processor::OPCodeCB0xE3()
{
}

void Processor::OPCodeCB0xE4()
{
}

void Processor::OPCodeCB0xE5()
{
}

void Processor::OPCodeCB0xE6()
{
}

void Processor::OPCodeCB0xE7()
{
}

void Processor::OPCodeCB0xE8()
{
}

void Processor::OPCodeCB0xE9()
{
}

void Processor::OPCodeCB0xEA()
{
}

void Processor::OPCodeCB0xEB()
{
}

void Processor::OPCodeCB0xEC()
{
}

void Processor::OPCodeCB0xED()
{
}

void Processor::OPCodeCB0xEE()
{
}

void Processor::OPCodeCB0xEF()
{
}

void Processor::OPCodeCB0xF0()
{
}

void Processor::OPCodeCB0xF1()
{
}

void Processor::OPCodeCB0xF2()
{
}

void Processor::OPCodeCB0xF3()
{
}

void Processor::OPCodeCB0xF4()
{
}

void Processor::OPCodeCB0xF5()
{
}

void Processor::OPCodeCB0xF6()
{
}

void Processor::OPCodeCB0xF7()
{
}

void Processor::OPCodeCB0xF8()
{
}

void Processor::OPCodeCB0xF9()
{
}

void Processor::OPCodeCB0xFA()
{
}

void Processor::OPCodeCB0xFB()
{
}

void Processor::OPCodeCB0xFC()
{
}

void Processor::OPCodeCB0xFD()
{
}

void Processor::OPCodeCB0xFE()
{
}

void Processor::OPCodeCB0xFF()
{
}
