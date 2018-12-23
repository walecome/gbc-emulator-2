#include "Register16bit.hh"

void Register16bit::setValue(register16_t value)
{
    high->setValue((register8_t)(value >> 8));
    low->setValue((register8_t)(value & 0x00FF));
}

register16_t Register16bit::getValue()
{
    return (high->getValue() << 8) + low->getValue();
}

void Register16bit::increment()
{
    register16_t current_value = getValue();

    setValue(++current_value);
}

void Register16bit::decrement()
{
    register16_t current_value = getValue();

    setValue(--current_value);
}