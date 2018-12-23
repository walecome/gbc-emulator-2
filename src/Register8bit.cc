#include "Register8bit.hh"

void Register8bit::setValue(register8_t value)
{
    this->value = value;
}

register8_t Register8bit::getValue()
{
    return value;
}

void Register8bit::increment()
{
    register8_t current_value = getValue();

    setValue(++current_value);
}

void Register8bit::decrement()
{
    register8_t current_value = getValue();

    setValue(--current_value);
}