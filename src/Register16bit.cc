#include "Register16bit.hh"

Register16bit::~Register16bit() {
    if (free_self) {
        delete high;
        delete low;
    }
}

void Register16bit::setValue(register16_t value) {
    high->setValue((register8_t)(value >> 8));
    low->setValue((register8_t)(value & 0x00FF));
}

register16_t Register16bit::getValue() const {
    return (high->getValue() << 8) + low->getValue();
}

void Register16bit::increment() {
    register16_t current_value = getValue();
    ++current_value;
    setValue(current_value);
}

void Register16bit::decrement() {
    register16_t current_value = getValue();

    setValue(--current_value);
}

Register8bit *Register16bit::getHighRegister() { return high; }

Register8bit *Register16bit::getLowRegister() { return low; }

std::ostream &operator<<(std::ostream &os, const Register16bit &reg) {
    os << reg.getName() << ": 0x" << std::setfill('0') << std::setw(4)
       << std::uppercase << std::hex << (unsigned)reg.getValue();

    return os;
}