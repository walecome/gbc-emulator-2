#include "Processor.hh"

Processor::Processor() {
    PC->setValue(PC_START);
    SP->setValue(SP_START);
}

// Flags
void Processor::checkFlagZ(register8_t result) {
    if (result == 0x00)
        setFlagZ();
    else
        resetFlagZ();
}

void Processor::checkFlagC(int result) {
    if (result > 0xFF)
        setFlagC();
    else
        resetFlagC();
}

void Processor::checkFlagH(register8_t result) {
    // TODO double check
    if ((result & 0x10) == 0x10)
        setFlagH();
    else
        resetFlagH();
}

bool Processor::getFlagC() {
    register8_t flags = F->getValue();
    return static_cast<bool>(flags & bit_c);
}

bool Processor::getFlagH() {
    register8_t flags = F->getValue();
    return static_cast<bool>(flags & bit_h);
}

bool Processor::getFlagN() {
    register8_t flags = F->getValue();
    return static_cast<bool>(flags & bit_n);
}

bool Processor::getFlagZ() {
    register8_t flags = F->getValue();
    return static_cast<bool>(flags & bit_z);
}

void Processor::setFlagC() {
    register8_t current = F->getValue();
    F->setValue(current | bit_c);
}

void Processor::setFlagH() {
    register8_t current = F->getValue();
    F->setValue(current | bit_h);
}

void Processor::setFlagN() {
    register8_t current = F->getValue();
    F->setValue(current | bit_n);
}

void Processor::setFlagZ() {
    register8_t current = F->getValue();
    F->setValue(current | bit_z);
}

void Processor::resetFlagC() {
    register8_t current = F->getValue();
    F->setValue(current & (~bit_c));
}

void Processor::resetFlagH() {
    register8_t current = F->getValue();
    F->setValue(current & ~(bit_h));
}

void Processor::resetFlagN() {
    register8_t current = F->getValue();
    F->setValue(current & ~(bit_n));
}

void Processor::resetFlagZ() {
    register8_t current = F->getValue();
    F->setValue(current & ~(bit_z));
}

opcode_t Processor::fetchInstruction() {
    return program_memory->getData(PC->getValue());
}

register8_t Processor::get_interrupt_data() { return ram->getData(0xFFFF); }
void Processor::set_interrupt_data(byte_t data) { ram->setData(0xFFFF, data); }
