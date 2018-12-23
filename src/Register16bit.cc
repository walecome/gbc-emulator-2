#include "Register16bit.hh"

Register16bit::Register16bit(std::string name)
{
    this->name = name;

    this->high = new Register8bit(name + "_high");
    this->low = new Register8bit(name + "_low");

    this->free_self = true;
}

Register16bit::~Register16bit()
{
    if (this->free_self)
    {
        delete this->high;
        delete this->low;
    }
}

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

Register8bit *Register16bit::getHighRegister()
{
    return this->high;
}

Register8bit *Register16bit::getLowRegister()
{
    return this->low;
}