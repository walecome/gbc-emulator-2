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

    delete ram;
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

void Processor::loadIntoMemory(Register16bit *address_reg, Register8bit *data_reg)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    byte_t data = data_reg->getValue();

    ram->setData(address, data);
}

void Processor::incrementRegister(Register8bit *reg)
{
    reg->increment();

    register8_t result = reg->getValue();

    checkFlagZ(result);
    setFlagN(false);
    checkFlagH(result);
}

void Processor::decrementRegister(Register8bit *reg)
{
    reg->decrement();

    register8_t result = reg->getValue();

    checkFlagZ(result);
    setFlagN(true);
    checkFlagH(result);
}

void Processor::copyRegister(Register8bit *destination, Register8bit *source)
{
    destination->setValue(source->getValue());
}

template <class reg_type, class value_type>
int add_helper(reg_type *a, reg_type *b)
{
    value_type value_dest = a->getValue();
    value_type value_source = b->getValue();

    int result = value_dest + value_source;

    value_dest = (value_type)result;

    a->setValue(value_dest);

    return result;
}

void Processor::addRegisters(Register8bit *destination, Register8bit *source)
{
    int result = add_helper<Register8bit, register8_t>(destination, source);

    register8_t value_dest = (register8_t)result;

    checkFlagZ(value_dest);
    setFlagN(false);
    checkFlagC(result);
    checkFlagH(value_dest);
}

void Processor::addRegisters(Register16bit *destination, Register16bit *source)
{
    int result = add_helper<Register16bit, register16_t>(destination, source);

    register16_t value_dest = (register16_t)result;

    setFlagN(false);
    checkFlagC(result);
    checkFlagH(value_dest);
}

void Processor::subRegisters(Register8bit *source)
{
    register8_t value_dest = A->getValue();
    register8_t value_source = source->getValue();

    value_dest -= value_source;

    A->setValue(value_dest);

    setFlagN(true);
}

void Processor::addWithCarry(Register8bit *destination, Register8bit *source)
{
    register8_t value_dest = destination->getValue();
    register8_t value_source = source->getValue();

    int carry = (getFlagC() ? 1 : 0);

    int result = value_dest + value_source + carry;

    // Overflow
    if (result > 0xFF)
    {
        setFlagC(true);
    }

    destination->setValue((register8_t)result);
}

// Flags

template <class T>
void Processor::checkFlagZ(T result)
{
    setFlagZ(result == 0x00);
}

void Processor::checkFlagC(int result)
{
    setFlagC(result > 0xFF);
}

void Processor::checkFlagH(register8_t result)
{
    // TODO
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