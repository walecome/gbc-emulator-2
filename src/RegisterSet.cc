#include "Processor.hh"

// 8bit

void Processor::setA(register8_t value)
{
    A->setValue(value);
}

void Processor::setB(register8_t value)
{
    B->setValue(value);
}

void Processor::setC(register8_t value)
{
    C->setValue(value);
}

void Processor::setD(register8_t value)
{
    D->setValue(value);
}

void Processor::setE(register8_t value)
{
    E->setValue(value);
}

void Processor::setF(register8_t value)
{
    F->setValue(value);
}

void Processor::setH(register8_t value)
{
    H->setValue(value);
}

void Processor::setL(register8_t value)
{
    L->setValue(value);
}

// 16bit

void Processor::setAF(register16_t value)
{
    AF->setValue(value);
}

void Processor::setBC(register16_t value)
{
    BC->setValue(value);
}

void Processor::setDE(register16_t value)
{
    DE->setValue(value);
}

void Processor::setHL(register16_t value)
{
    HL->setValue(value);
}