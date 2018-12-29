#include "Processor.hh"

Processor::Processor()
{
    setValuePC(PC_START);
    setValueSP(SP_START);
    // TODO Change program memory into a "real" memory
    this->program_memory.reserve((unsigned)PC_MAX_SIZE);
}

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
    opcode_t opcode = program_memory[program_counter->getValue()];

    program_counter->increment();

    return opcode;
}

byte_t Processor::getCurrentData()
{
    byte_t data = program_memory[program_counter->getValue()];
    program_counter->increment();

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

void Processor::loadIntoMemory(Register16bit *address_reg, byte_t value)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    ram->setData(address, value);
}

void Processor::loadFromMemory(Register8bit *data_reg, Register16bit *address_reg)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    byte_t data = ram->getData(address);

    data_reg->setValue(data);
}

byte_t Processor::loadFromMemory(Register16bit *address_reg)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    return ram->getData(address);
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

void Processor::subWithCarry(Register8bit *source)
{

    register8_t value_dest = A->getValue();
    register8_t value_source = source->getValue();
    int carry = (getFlagC() ? 1 : 0);

    value_dest = value_dest - value_source - carry;

    A->setValue(value_dest);

    // TODO fix rest of flags. How does negative carry work???
    setFlagN(true);
}

void Processor::andRegisters(Register8bit *source)
{
    register8_t data_a = A->getValue();
    register8_t data_source = source->getValue();

    // AND
    register8_t result = data_a & data_source;

    A->setValue(result);
}

void Processor::xorRegisters(Register8bit *source)
{
    register8_t data_a = A->getValue();
    register8_t data_source = source->getValue();

    // XOR
    register8_t result = data_a ^ data_source;

    A->setValue(result);
}

void Processor::orRegisters(Register8bit *source)
{
    register8_t data_a = A->getValue();
    register8_t data_source = source->getValue();

    // XOR
    register8_t result = data_a | data_source;

    A->setValue(result);
}

void Processor::cmpRegisters(Register8bit *source)
{
    register8_t value_a = A->getValue();
    register8_t value_source = source->getValue();

    setFlagZ(value_a == value_source);

    // TODO half carry flag
    setFlagN(true);

    setFlagC(value_a < value_source);
}

void Processor::pushStack(Register16bit *source)
{
    stack_pointer->decrement();
    stack->setData(stack_pointer->getValue(),
                   source->getHighRegister()->getValue());
    stack_pointer->decrement();
    stack->setData(stack_pointer->getValue(),
                   source->getLowRegister()->getValue());
}

void Processor::popStack(Register16bit *destination)
{
    byte_t data_low = stack->getData(stack_pointer->getValue());
    destination->getLowRegister()->setValue(data_low);
    stack_pointer->increment();

    byte_t data_high = stack->getData(stack_pointer->getValue());
    destination->getHighRegister()->setValue(data_high);
    stack_pointer->increment();
}

void Processor::performJump()
{
    register16_t current_pc = program_counter->getValue();
    byte_t offset = program_memory[current_pc];

    // TODO this this logic
    register16_t new_pc = (register16_t)((int16_t)current_pc + (int8_t)offset);

    program_counter->setValue(new_pc);
}

void Processor::jumpIm16bit()
{
    register8_t data_low = getCurrentData();
    register8_t data_high = getCurrentData();

    program_counter->getLowRegister()->setValue(data_low);
    program_counter->getHighRegister()->setValue(data_high);
}

void Processor::rlcRegister(Register8bit *source)
{
    register8_t data_source = source->getValue();

    if ((data_source & MSB_8BIT) == 0)
    {
        // MSB is 0
        data_source <<= 1;
        setFlagC(false);
    }
    else
    {
        // MSB is 1
        data_source <<= 1;
        data_source |= LSB_8BIT;
        setFlagC(true);
    }

    source->setValue(data_source);

    setFlagZ(data_source == 0x00);
    setFlagN(false);
    setFlagH(false);
}

void Processor::rrcRegister(Register8bit *source)
{
    register8_t data_source = source->getValue();

    if ((data_source & LSB_8BIT) == 0)
    {
        // LSB is 0
        data_source >>= 1;
        setFlagC(false);
    }
    else
    {
        // LSB is 1
        data_source >>= 1;
        data_source |= MSB_8BIT;
        setFlagC(true);
    }

    source->setValue(data_source);

    setFlagZ(data_source == 0x00);
    setFlagN(false);
    setFlagH(false);
}

void Processor::rlRegister(Register8bit *source)
{
    register8_t data_source = source->getValue();
    register8_t carry = (getFlagC() ? 1 : 0);

    if ((data_source & MSB_8BIT) == 0)
    {
        // MSB is 0
        setFlagC(false);
    }
    else
    {
        // MSB is 1
        setFlagC(true);
    }

    data_source <<= 1;
    data_source |= carry;
    source->setValue(data_source);

    setFlagZ(data_source == 0x00);
    setFlagN(false);
    setFlagH(false);
}

void Processor::rrRegister(Register8bit *source)
{
    register8_t data_source = source->getValue();
    register8_t carry = (getFlagC() ? MSB_8BIT : 0);

    if ((data_source & LSB_8BIT) == 0)
    {
        // LSB is 0
        setFlagC(false);
    }
    else
    {
        // LSB is 1
        setFlagC(true);
    }

    data_source >>= 1;
    data_source |= carry;
    source->setValue(data_source);

    setFlagZ(data_source == 0x00);
    setFlagN(false);
    setFlagH(false);
}

void Processor::slaRegister(Register8bit *source)
{
    register8_t data_source = source->getValue();

    if ((data_source & MSB_8BIT) == 0)
    {
        // MSB is 0
        setFlagC(false);
    }
    else
    {
        setFlagC(true);
    }

    data_source <<= 1;

    source->setValue(data_source);

    setFlagZ(data_source == 0x00);
    setFlagN(false);
    setFlagH(false);
}

void Processor::sraRegister(Register8bit *source)
{
    register8_t data_source = source->getValue();
    register8_t msb_val = data_source & MSB_8BIT;

    if ((data_source & LSB_8BIT) == 0)
    {
        // LSB is 0
        setFlagC(false);
    }
    else
    {
        // LSB is 1
        setFlagC(true);
    }

    data_source >>= 1;
    data_source |= msb_val;

    source->setValue(data_source);

    setFlagZ(data_source == 0x00);
    setFlagN(false);
    setFlagH(false);
}

void Processor::swapNibbles(Register8bit *reg)
{
    register8_t val = reg->getValue();

    register8_t new_low = (val & 0xF0) >> 4;
    register8_t new_high = (val & 0x0F) << 4;

    val = new_high + new_low;

    reg->setValue(val);

    setFlagZ(val == 0x00);
    setFlagN(false);
    setFlagH(false);
    setFlagC(false);
}

void Processor::srlRegister(Register8bit *reg)
{
    register8_t data_reg = reg->getValue();

    if ((data_reg & LSB_8BIT) == 0)
    {
        // LSB is 0
        setFlagC(false);
    }
    else
    {
        // LSB is 1
        setFlagC(true);
    }

    data_reg >>= 1;

    reg->setValue(data_reg);

    setFlagZ(data_reg == 0x00);
    setFlagN(false);
    setFlagH(false);
}

void Processor::testBit(int b, Register8bit *reg)
{
    register8_t reg_val = reg->getValue();
    register8_t mask = (1 << b);

    if ((reg_val & mask) == 0)
    {
        // Bit b is 0
        setFlagZ(true);
    }
    else
    {
        // Bit b is 1
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(true);
}

void Processor::testBit(int b, Register16bit *reg)
{
    byte_t reg_val = loadFromMemory(reg);
    register8_t mask = (1 << b);

    if ((reg_val & mask) == 0)
    {
        // Bit b is 0
        setFlagZ(true);
    }
    else
    {
        // Bit b is 1
        setFlagZ(false);
    }

    setFlagN(false);
    setFlagH(true);
}

void Processor::resetBit(int b, Register8bit *reg)
{
    register8_t reg_val = reg->getValue();

    reg_val &= ~(1 << b);

    reg->setValue(reg_val);
}

void Processor::resetBit(int b, Register16bit *reg)
{
    // TODO: Probably wrong, should not offset with FF00
    byte_t val = loadFromMemory(reg);

    val &= ~(1 << b);

    loadIntoMemory(reg, val);
}

void Processor::setBit(int b, Register8bit *reg)
{
    register8_t reg_val = reg->getValue();

    reg_val |= (1 << b);

    reg->setValue(reg_val);
}

void Processor::setBit(int b, Register16bit *reg)
{
    byte_t val = loadFromMemory(reg);

    val |= (1 << b);

    loadIntoMemory(reg, val);
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

// PC

register16_t Processor::getValuePC()
{
    return program_counter->getValue();
}

void Processor::setValuePC(register16_t value)
{
    program_counter->setValue(value);
}

// SP

register16_t Processor::getValueSP()
{
    return stack_pointer->getValue();
}

void Processor::setValueSP(register16_t value)
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

Memory *Processor::getRAM()
{
    return this->ram;
}

Memory *Processor::getStack()
{
    return this->stack;
}

std::vector<opcode_t> &Processor::getProgramMem()
{
    return this->program_memory;
}
