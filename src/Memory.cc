#include "Memory.hh"

Memory::Memory(const unsigned size) { this->memory.reserve(size); }

byte_t Memory::getData(const register16_t address) {
    return this->memory[address];
}

void Memory::setData(const register16_t address, byte_t data) {
    this->memory[address] = data;
}