#include "Processor.hh"
#include "Processor.hh"

// 8bit

register8_t Processor::getA()
{
    return A->getValue();
}

register8_t Processor::getB()
{
    return B->getValue();
}

register8_t Processor::getC()
{
    return C->getValue();
}

register8_t Processor::getD()
{
    return D->getValue();
}

register8_t Processor::getE()
{
    return E->getValue();
}

register8_t Processor::getF()
{
    return F->getValue();
}

register8_t Processor::getH()
{
    return H->getValue();
}

register8_t Processor::getL()
{
    return L->getValue();
}

// 16bit

register16_t Processor::getAF()
{
    return AF->getValue();
}

register16_t Processor::getBC()
{
    return BC->getValue();
}

register16_t Processor::getDE()
{
    return DE->getValue();
}

register16_t Processor::getHL()
{
    return HL->getValue();
}