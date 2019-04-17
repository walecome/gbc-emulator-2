#include "Register8bit.hh"

void Register8bit::setValue(register8_t value) { this->value = value; }

register8_t Register8bit::getValue() const { return value; }

void Register8bit::increment() {
    register8_t current_value = getValue();

    setValue(++current_value);
}

void Register8bit::decrement() {
    register8_t current_value = getValue();

    setValue(--current_value);
}

std::ostream &operator<<(std::ostream &os, const Register8bit &reg) {
    os << reg.getName() << ": 0x" << std::setfill('0') << std::setw(2)
       << std::uppercase << std::hex << (unsigned)reg.getValue();

    return os;
}