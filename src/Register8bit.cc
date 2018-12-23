#include "Register8bit.hh"

void Register8bit::setValue(register8_t value)
{
    this->value = value;
}

register8_t Register8bit::getValue()
{
    return value;
}