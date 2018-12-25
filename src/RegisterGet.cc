#include "Processor.hh"
#include "Processor.hh"

// 8bit

Register8bit *Processor::getA()
{
    return A;
}

Register8bit *Processor::getB()
{
    return B;
}

Register8bit *Processor::getC()
{
    return C;
}

Register8bit *Processor::getD()
{
    return D;
}

Register8bit *Processor::getE()
{
    return E;
}

Register8bit *Processor::getF()
{
    return F;
}

Register8bit *Processor::getH()
{
    return H;
}

Register8bit *Processor::getL()
{
    return L;
}

register8_t Processor::getValueA()
{
    return A->getValue();
}

register8_t Processor::getValueB()
{
    return B->getValue();
}

register8_t Processor::getValueC()
{
    return C->getValue();
}

register8_t Processor::getValueD()
{
    return D->getValue();
}

register8_t Processor::getValueE()
{
    return E->getValue();
}

register8_t Processor::getValueF()
{
    return F->getValue();
}

register8_t Processor::getValueH()
{
    return H->getValue();
}

register8_t Processor::getValueL()
{
    return L->getValue();
}

// 16bit

Register16bit *Processor::getAF()
{
    return AF;
}

Register16bit *Processor::getBC()
{
    return BC;
}

Register16bit *Processor::getDE()
{
    return DE;
}

Register16bit *Processor::getHL()
{
    return HL;
}

register16_t Processor::getValueAF()
{
    return AF->getValue();
}

register16_t Processor::getValueBC()
{
    return BC->getValue();
}

register16_t Processor::getValueDE()
{
    return DE->getValue();
}

register16_t Processor::getValueHL()
{
    return HL->getValue();
}