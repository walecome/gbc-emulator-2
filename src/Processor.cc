#include "Processor.hh"

Processor::Processor()
{
    setValuePC(PC_START);
    setValueSP(SP_START);
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
    delete stack;
    delete stack_pointer;
    delete program_counter;
}

/**
    Fetches the instruction currently pointed at by the program counter.
    Also increments the program counter to point at next instruction/data.
*/
opcode_t Processor::fetchInstruction()
{
    opcode_t opcode = program_memory->getData(program_counter->getValue());

    program_counter->increment();

    return opcode;
}

/** 
    Get the data pointed to by the program counter and increment the program
    counter.
*/
byte_t Processor::getCurrentData()
{
    byte_t data = program_memory->getData(program_counter->getValue());
    program_counter->increment();

    return data;
}

/**
    Loads data from where the program counter is pointing into the given register.
    Also increments the program counter.
*/
void Processor::loadRegister(Register8bit *reg)
{
    reg->setValue(getCurrentData());
}

/**
    Loads the data in data_reg into the address 0xFF00 + (value in address_reg).
*/
void Processor::loadIntoMemory(Register16bit *address_reg, Register8bit *data_reg)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    byte_t data = data_reg->getValue();

    ram->setData(address, data);
}

/**
    Loads the data in value into the address 0xFF00 + address_reg
*/
void Processor::loadIntoMemory(Register16bit *address_reg, byte_t value)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    ram->setData(address, value);
}

/**
    Loads data from the memory address stored in 0xFF00 + address_reg and stores
    it in data_reg.
*/
void Processor::loadFromMemory(Register8bit *data_reg, Register16bit *address_reg)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    byte_t data = ram->getData(address);

    data_reg->setValue(data);
}

/**
    Loads from memory address (0xFF00 + address_reg) and returns it.
*/
byte_t Processor::loadFromMemory(Register16bit *address_reg)
{
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    return ram->getData(address);
}

/**
    Increments the value of the passed register and sets flags accordingly.
    This function is only for 8bit registers since 16bit register increment
    doesn't affect flags.
*/
void Processor::incrementRegister(Register8bit *reg)
{
    reg->increment();

    register8_t result = reg->getValue();

    checkFlagZ(result);
    setFlagN(false);
    checkFlagH(result);
}

/**
    Decrements the value of the passed register and sets flags accordingly.
    This function is only for 8bit registers since 16bit register decrement
    doesn't affect flags.
*/
void Processor::decrementRegister(Register8bit *reg)
{
    reg->decrement();

    register8_t result = reg->getValue();

    checkFlagZ(result);
    setFlagN(true);
    checkFlagH(result);
}

/**
    Copies the value from the destination register to the source register
*/
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

/**
    Adds the values in the destination and source registers and stores the 
    result in the destination register
*/
void Processor::addRegisters(Register8bit *destination, Register8bit *source)
{
    int result = add_helper<Register8bit, register8_t>(destination, source);

    register8_t value_dest = (register8_t)result;

    checkFlagZ(value_dest);
    setFlagN(false);
    checkFlagC(result);
    checkFlagH(value_dest);
}

/**
    Adds the values in the destination and source registers and stores the 
    result in the destination register
*/
void Processor::addRegisters(Register16bit *destination, Register16bit *source)
{
    int result = add_helper<Register16bit, register16_t>(destination, source);

    register16_t value_dest = (register16_t)result;

    setFlagN(false);
    checkFlagC(result);
    checkFlagH(value_dest);
}

/**
    Subtracts the value in the accumulator (reg A) with the value in the
    source register and stores the results in the accumulator
*/
void Processor::subRegisters(Register8bit *source)
{
    register8_t value_dest = A->getValue();
    register8_t value_source = source->getValue();

    value_dest -= value_source;

    A->setValue(value_dest);

    setFlagN(true);
}

/**
    Adds registers with the carry included in the addition
*/
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

/**
    Subtracts the value in the accumulator (reg A) with the value in the source
    register. The subtraction also includes the carry value. Result is stored
    in the accumulator.
*/
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

/**
    Performs a bitwise AND operation with the accumulator (reg A) and the passed
    in register. The result is stored in the accumulator.
*/
void Processor::andRegisters(Register8bit *source)
{
    register8_t data_a = A->getValue();
    register8_t data_source = source->getValue();

    // AND
    register8_t result = data_a & data_source;

    A->setValue(result);
}

/**
    Performs a bitwise XOR operation with the accumulator (reg A) and the passed
    in register. The result is stored in the accumulator.
*/
void Processor::xorRegisters(Register8bit *source)
{
    register8_t data_a = A->getValue();
    register8_t data_source = source->getValue();

    // XOR
    register8_t result = data_a ^ data_source;

    A->setValue(result);
}

/**
    Performs a bitwise OR operation with the accumulator (reg A) and the passed
    in register. The result is store in the accumulator.
*/
void Processor::orRegisters(Register8bit *source)
{
    register8_t data_a = A->getValue();
    register8_t data_source = source->getValue();

    // XOR
    register8_t result = data_a | data_source;

    A->setValue(result);
}

/**
    Compares the value in the passed in register with the value in the
    accumulator (reg A). Sets flags as a regular subtraction but the result
    of the subtraction is not used.
*/
void Processor::cmpRegisters(Register8bit *source)
{
    register8_t value_a = A->getValue();
    register8_t value_source = source->getValue();

    setFlagZ(value_a == value_source);

    // TODO half carry flag
    setFlagN(true);

    setFlagC(value_a < value_source);
}

/**
    Pushes the value in the passed 16bit register into the program stack.
    This will decrement the stack pointer by 2.
*/
void Processor::pushStack(Register16bit *source)
{
    stack_pointer->decrement();
    stack->setData(stack_pointer->getValue(),
                   source->getHighRegister()->getValue());
    stack_pointer->decrement();
    stack->setData(stack_pointer->getValue(),
                   source->getLowRegister()->getValue());
}

/**
    Pops a 16bit value from the stack and stores the values in the passed in
    register. This will increment the stack pointer by 2.
*/
void Processor::popStack(Register16bit *destination)
{
    byte_t data_low = stack->getData(stack_pointer->getValue());
    destination->getLowRegister()->setValue(data_low);
    stack_pointer->increment();

    byte_t data_high = stack->getData(stack_pointer->getValue());
    destination->getHighRegister()->setValue(data_high);
    stack_pointer->increment();
}

/**
    Does the pop operation but with AF as destination register (altering flags).
*/
void Processor::popStackAF()
{
    popStack(AF);
    register8_t f_data = F->getValue();

    setFlagZ(bool((0x80 & f_data) != 0));
    setFlagN(bool((0x40 & f_data) != 0));
    setFlagH(bool((0x20 & f_data) != 0));
    setFlagC(bool((0x10 & f_data) != 0));
}

/**
    Perform jump. Modifies the program counter by adding it to the value that is currently
    pointed at by the program counter.
*/
void Processor::performJump()
{
    register16_t current_pc = program_counter->getValue();
    byte_t offset = program_memory->getData(current_pc);

    // TODO this this logic
    register16_t new_pc = (register16_t)((int16_t)current_pc + (int8_t)offset);

    program_counter->setValue(new_pc);
}

/** 
    Performs a jump to a 16bit address stored in the program memory.
*/
void Processor::jumpIm16bit()
{
    register8_t data_low = getCurrentData();
    register8_t data_high = getCurrentData();

    program_counter->getLowRegister()->setValue(data_low);
    program_counter->getHighRegister()->setValue(data_high);
}

/**
    Performs a left bitwise rotation on the given register. MSB will also be
    stored in the carry flag.
*/
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

/**
    Performs a right bitwise rotation on the given register. LSB will also be
    stored in the carry flag.
*/
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

/**
    Performs a left bitwise rotation through the carry given the register.
*/
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

/**
    Performs a right bitwise rotation through the carry given the register.
*/
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

/**
    Performs a left bitwise shift into the carry.
*/
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

/**
    Performs a right bitwise shift into carry. Perserves MSB.
*/
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

/**
    Swaps the upper and lower nibbles of given register.
*/
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

/**
    Performs a right bitwise shift into carry. MSB is set to 0.
*/
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

/**
    Tests bit b of register reg.
*/
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

/**
    Test bit b of data at address in address_reg.
*/
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

/**
    Sets bit b in reg to 0.
*/
void Processor::resetBit(int b, Register8bit *reg)
{
    register8_t reg_val = reg->getValue();

    reg_val &= ~(1 << b);

    reg->setValue(reg_val);
}

/**
    Sets bit b in data located at address of address_reg to 0.
*/
void Processor::resetBit(int b, Register16bit *reg)
{
    // TODO: Probably wrong, should not offset with FF00
    byte_t val = loadFromMemory(reg);

    val &= ~(1 << b);

    loadIntoMemory(reg, val);
}

/**
    Sets bit b in reg to 1.
*/
void Processor::setBit(int b, Register8bit *reg)
{
    register8_t reg_val = reg->getValue();

    reg_val |= (1 << b);

    reg->setValue(reg_val);
}

/**
    Sets bit b in data located at address of address_reg to 1.
*/
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
    // TODO double check
    setFlagH((result & 0x10) == 0x10);
}