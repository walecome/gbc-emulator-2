#include "Processor.hh"

Processor::~Processor()
{
    delete A;
    delete B;
    delete C;
    delete D;
    delete E;
    delete F;
    delete H;
    delete L;

    delete AF;
    delete BC;
    delete DE;
    delete HL;
}

void Processor::readInstructions(const char *filename)
{
    // open the file as binary
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode
    file.unsetf(std::ios::skipws);

    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    program_memory.reserve(fileSize);

    // read the data:
    program_memory.insert(program_memory.begin(),
                          std::istream_iterator<opcode_t>(file),
                          std::istream_iterator<opcode_t>());
}

opcode_t Processor::fetchInstruction()
{
    opcode_t opcode = program_memory[program_counter];

    ++program_counter;

    return opcode;
}

byte_t Processor::getCurrentData()
{
    byte_t data = program_memory[program_counter];
    ++program_counter;

    return data;
}

void Processor::loadRegister(Register8bit *reg)
{
    reg->setValue(getCurrentData());
}

void Processor::copyRegister(Register8bit *destination, Register8bit *source)
{
    destination->setValue(source->getValue());
}

void Processor::addRegisters(Register8bit *destination, Register8bit *source)
{
    register8_t value_dest = destination->getValue();
    register8_t value_source = source->getValue();

    value_dest += value_source;

    destination->setValue(value_dest);
}

void Processor::subRegisters(Register8bit *source)
{
    register8_t value_dest = A->getValue();
    register8_t value_source = source->getValue();

    value_dest -= value_source;

    A->setValue(value_dest);
}

// SP

register16_t Processor::getSP()
{
    return stack_pointer->getValue();
}

void Processor::setSP(register16_t value)
{
    stack_pointer->setValue(value);
}

// Flag getters

bool Processor::getFlagC()
{
    return flagC;
}

bool Processor::getFlagH()
{
    return flagH;
}

bool Processor::getFlagN()
{
    return flagN;
}

bool Processor::getFlagZ()
{
    return flagZ;
}

// Flag setters

void Processor::setFlagC(bool value)
{
    flagC = value;
}

void Processor::setFlagH(bool value)
{
    flagH = value;
}

void Processor::setFlagN(bool value)
{
    flagN = value;
}

void Processor::setFlagZ(bool value)
{
    flagZ = value;
}