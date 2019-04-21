#include "InstructionDecoder.hh"

/**
    Fetches the instruction currently pointed at by the program counter.
    Also increments the program counter to point at next instruction/data.
*/
opcode_t InstructionDecoder::fetchInstruction() {
    opcode_t opcode = cpu->program_memory->getData(cpu->PC->getValue());

    cpu->PC->increment();

    return opcode;
}

/**
    Get the data pointed to by the program counter and increment the program
    counter.
*/
byte_t InstructionDecoder::getCurrentData() {
    byte_t data = cpu->program_memory->getData(cpu->PC->getValue());
    cpu->PC->increment();

    return data;
}

/**
    Loads data from where the program counter is pointing into the given
   register. Also increments the program counter.
*/
void InstructionDecoder::loadRegister(const ptr<Register8bit> &reg) {
    reg->setValue(getCurrentData());
}

/**
    Loads the data in data_reg into the address 0xFF00 + (value in address_reg).
*/
void InstructionDecoder::loadIntoMemory(const ptr<Register16bit> &address_reg,
                                        const ptr<Register8bit> &data_reg) {
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    byte_t data = data_reg->getValue();

    cpu->ram->setData(address, data);
}

/**
    Loads the data in value into the address 0xFF00 + address_reg
*/
void InstructionDecoder::loadIntoMemory(const ptr<Register16bit> &address_reg,
                                        byte_t value) {
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    cpu->ram->setData(address, value);
}

/**
    Loads data from the memory address stored in 0xFF00 + address_reg and stores
    it in data_reg.
*/
void InstructionDecoder::loadFromMemory(const ptr<Register8bit> &data_reg,
                                        const ptr<Register16bit> &address_reg) {
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    byte_t data = cpu->ram->getData(address);

    data_reg->setValue(data);
}

/**
    Loads from memory address (0xFF00 + address_reg) and returns it.
*/
byte_t InstructionDecoder::loadFromMemory(
    const ptr<Register16bit> &address_reg) {
    register16_t address = RAM_DATA_OFFSET + address_reg->getValue();

    return cpu->ram->getData(address);
}

/**
    Increments the value of the passed register and sets flags accordingly.
    This function is only for 8bit registers since 16bit register increment
    doesn't affect flags.
*/
void InstructionDecoder::incrementRegister(const ptr<Register8bit> &reg) {
    reg->increment();

    register8_t result = reg->getValue();

    cpu->checkFlagZ(result);
    cpu->setFlagN(false);
    cpu->checkFlagH(result);
}

/**
    Decrements the value of the passed register and sets flags accordingly.
    This function is only for 8bit registers since 16bit register decrement
    doesn't affect flags.
*/
void InstructionDecoder::decrementRegister(const ptr<Register8bit> &reg) {
    reg->decrement();

    register8_t result = reg->getValue();

    cpu->checkFlagZ(result);
    cpu->setFlagN(true);
    cpu->checkFlagH(result);
}

/**
    Copies the value from the destination register to the source register
*/
void InstructionDecoder::copyRegister(const ptr<Register8bit> &destination,
                                      const ptr<Register8bit> &source) {
    destination->setValue(source->getValue());
}

template <class reg_type, class value_type>
int add_helper(const ptr<reg_type> &a, const ptr<reg_type> &b) {
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
void InstructionDecoder::addRegisters(const ptr<Register8bit> &destination,
                                      const ptr<Register8bit> &source) {
    int result = add_helper<Register8bit, register8_t>(destination, source);

    register8_t value_dest = (register8_t)result;

    cpu->checkFlagZ(value_dest);
    cpu->setFlagN(false);
    cpu->checkFlagC(result);
    cpu->checkFlagH(value_dest);
}

/**
    Adds the values in the destination and source registers and stores the
    result in the destination register
*/
void InstructionDecoder::addRegisters(const ptr<Register16bit> &destination,
                                      const ptr<Register16bit> &source) {
    int result = add_helper<Register16bit, register16_t>(destination, source);

    register16_t value_dest = (register16_t)result;

    cpu->setFlagN(false);
    cpu->checkFlagC(result);
    cpu->checkFlagH(value_dest);
}

/**
    Subtracts the value in the accumulator (reg A) with the value in the
    source register and stores the results in the accumulator
*/
void InstructionDecoder::subRegisters(const ptr<Register8bit> &source) {
    register8_t value_dest = cpu->A->getValue();
    register8_t value_source = source->getValue();

    value_dest -= value_source;

    cpu->A->setValue(value_dest);

    cpu->setFlagN(true);
}

/**
    Adds registers with the carry included in the addition
*/
void InstructionDecoder::addWithCarry(const ptr<Register8bit> &destination,
                                      const ptr<Register8bit> &source) {
    register8_t value_dest = destination->getValue();
    register8_t value_source = source->getValue();

    int carry = (cpu->getFlagC() ? 1 : 0);

    int result = value_dest + value_source + carry;

    // Overflow
    if (result > 0xFF) {
        cpu->setFlagC(true);
    }

    destination->setValue((register8_t)result);
}

/**
    Subtracts the value in the accumulator (reg A) with the value in the source
    register. The subtraction also includes the carry value. Result is stored
    in the accumulator.
*/
void InstructionDecoder::subWithCarry(const ptr<Register8bit> &source) {
    register8_t value_dest = cpu->A->getValue();
    register8_t value_source = source->getValue();
    int carry = (cpu->getFlagC() ? 1 : 0);

    value_dest = value_dest - value_source - carry;

    cpu->A->setValue(value_dest);

    // TODO fix rest of flags. How does negative carry work???
    cpu->setFlagN(true);
}

/**
    Performs a bitwise AND operation with the accumulator (reg A) and the passed
    in register. The result is stored in the accumulator.
*/
void InstructionDecoder::andRegisters(const ptr<Register8bit> &source) {
    register8_t data_a = cpu->A->getValue();
    register8_t data_source = source->getValue();

    // AND
    register8_t result = data_a & data_source;

    cpu->A->setValue(result);
}

/**
    Performs a bitwise XOR operation with the accumulator (reg A) and the passed
    in register. The result is stored in the accumulator.
*/
void InstructionDecoder::xorRegisters(const ptr<Register8bit> &source) {
    register8_t data_a = cpu->A->getValue();
    register8_t data_source = source->getValue();

    // XOR
    register8_t result = data_a ^ data_source;

    cpu->A->setValue(result);
}

/**
    Performs a bitwise OR operation with the accumulator (reg A) and the passed
    in register. The result is store in the accumulator.
*/
void InstructionDecoder::orRegisters(const ptr<Register8bit> &source) {
    register8_t data_a = cpu->A->getValue();
    register8_t data_source = source->getValue();

    // OR
    register8_t result = data_a | data_source;

    cpu->A->setValue(result);
}

/**
    Compares the value in the passed in register with the value in the
    accumulator (reg A). Sets flags as a regular subtraction but the result
    of the subtraction is not used.
*/
void InstructionDecoder::cmpRegisters(const ptr<Register8bit> &source) {
    register8_t value_a = cpu->A->getValue();
    register8_t value_source = source->getValue();

    cpu->setFlagZ(value_a == value_source);

    // TODO half carry flag
    cpu->setFlagN(true);
    cpu->setFlagC(value_a < value_source);
}

/**
    Pushes the value in the passed 16bit register into the program stack.
    This will decrement the stack pointer by 2.
*/
void InstructionDecoder::pushStack(const ptr<Register16bit> &source) {
    cpu->SP->decrement();
    cpu->stack->setData(cpu->SP->getValue(),
                        source->getHighRegister()->getValue());
    cpu->SP->decrement();
    cpu->stack->setData(cpu->SP->getValue(),
                        source->getLowRegister()->getValue());
}

/**
    Pops a 16bit value from the stack and stores the values in the passed in
    register. This will increment the stack pointer by 2.
*/
void InstructionDecoder::popStack(const ptr<Register16bit> &destination) {
    byte_t data_low = cpu->stack->getData(SP->getValue());
    destination->getLowRegister()->setValue(data_low);
    cpu->SP->increment();

    byte_t data_high = cpu->stack->getData(cpu->SP->getValue());
    destination->getHighRegister()->setValue(data_high);
    cpu->SP->increment();
}

/**
    Does the pop operation but with AF as destination register (altering flags).
*/
void InstructionDecoder::popStackAF() {
    popStack(cpu->AF);
    register8_t f_data = cpu->F->getValue();

    cpu->setFlagZ(bool((0x80 & f_data) != 0));
    cpu->setFlagN(bool((0x40 & f_data) != 0));
    cpu->setFlagH(bool((0x20 & f_data) != 0));
    cpu->setFlagC(bool((0x10 & f_data) != 0));
}

/**
    Perform jump. Modifies the program counter by adding it to the value that is
   currently pointed at by the program counter.
*/
void InstructionDecoder::performJump() {
    register16_t current_pc = cpu->PC->getValue();
    byte_t offset = cpu->program_memory->getData(current_pc);

    // TODO check this logic
    register16_t new_pc = (register16_t)((int16_t)current_pc + (int8_t)offset);

    cpu->PC->setValue(new_pc);
}

/**
    Performs a jump to a 16bit address stored in the program memory.
*/
void InstructionDecoder::jumpIm16bit() {
    register8_t data_low = getCurrentData();
    register8_t data_high = getCurrentData();

    cpu->PC->getLowRegister()->setValue(data_low);
    cpu->PC->getHighRegister()->setValue(data_high);
}

/**
    Performs a left bitwise rotation on the given register. MSB will also be
    stored in the carry flag.
*/
void InstructionDecoder::rlcRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();

    if ((data_source & MSB_8BIT) == 0) {
        // MSB is 0
        data_source <<= 1;
        cpu->setFlagC(false);
    } else {
        // MSB is 1
        data_source <<= 1;
        data_source |= LSB_8BIT;
        cpu->setFlagC(true);
    }

    source->setValue(data_source);

    cpu->setFlagZ(data_source == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
}

/**
    Performs a right bitwise rotation on the given register. LSB will also be
    stored in the carry flag.
*/
void InstructionDecoder::rrcRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();

    if ((data_source & LSB_8BIT) == 0) {
        // LSB is 0
        data_source >>= 1;
        cpu->setFlagC(false);
    } else {
        // LSB is 1
        data_source >>= 1;
        data_source |= MSB_8BIT;
        cpu->setFlagC(true);
    }

    source->setValue(data_source);

    cpu->setFlagZ(data_source == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
}

/**
    Performs a left bitwise rotation through the carry given the register.
*/
void InstructionDecoder::rlRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();
    register8_t carry = (cpu->getFlagC() ? 1 : 0);

    if ((data_source & MSB_8BIT) == 0) {
        // MSB is 0
        cpu->setFlagC(false);
    } else {
        // MSB is 1
        cpu->setFlagC(true);
    }

    data_source <<= 1;
    data_source |= carry;
    source->setValue(data_source);

    cpu->setFlagZ(data_source == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
}

/**
    Performs a right bitwise rotation through the carry given the register.
*/
void InstructionDecoder::rrRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();
    register8_t carry = (cpu->getFlagC() ? MSB_8BIT : 0);

    if ((data_source & LSB_8BIT) == 0) {
        // LSB is 0
        cpu->setFlagC(false);
    } else {
        // LSB is 1
        cpu->setFlagC(true);
    }

    data_source >>= 1;
    data_source |= carry;
    source->setValue(data_source);

    cpu->setFlagZ(data_source == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
}

/**
    Performs a left bitwise shift into the carry.
*/
void InstructionDecoder::slaRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();

    if ((data_source & MSB_8BIT) == 0) {
        // MSB is 0
        cpu->setFlagC(false);
    } else {
        cpu->setFlagC(true);
    }

    data_source <<= 1;

    source->setValue(data_source);

    cpu->setFlagZ(data_source == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
}

/**
    Performs a right bitwise shift into carry. Perserves MSB.
*/
void InstructionDecoder::sraRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();
    register8_t msb_val = data_source & MSB_8BIT;

    if ((data_source & LSB_8BIT) == 0) {
        // LSB is 0
        cpu->setFlagC(false);
    } else {
        // LSB is 1
        cpu->setFlagC(true);
    }

    data_source >>= 1;
    data_source |= msb_val;

    source->setValue(data_source);

    cpu->setFlagZ(data_source == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
}

/**
    Swaps the upper and lower nibbles of given register.
*/
void InstructionDecoder::swapNibbles(const ptr<Register8bit> &reg) {
    register8_t val = reg->getValue();

    register8_t new_low = (val & 0xF0) >> 4;
    register8_t new_high = (val & 0x0F) << 4;

    val = new_high + new_low;

    reg->setValue(val);

    cpu->setFlagZ(val == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
    cpu->setFlagC(false);
}

/**
    Performs a right bitwise shift into carry. MSB is set to 0.
*/
void InstructionDecoder::srlRegister(const ptr<Register8bit> &reg) {
    register8_t data_reg = reg->getValue();

    if ((data_reg & LSB_8BIT) == 0) {
        // LSB is 0
        cpu->setFlagC(false);
    } else {
        // LSB is 1
        cpu->setFlagC(true);
    }

    data_reg >>= 1;

    reg->setValue(data_reg);

    cpu->setFlagZ(data_reg == 0x00);
    cpu->setFlagN(false);
    cpu->setFlagH(false);
}

/**
    Tests bit b of register reg.
*/
void InstructionDecoder::testBit(int b, const ptr<Register8bit> &reg) {
    register8_t reg_val = reg->getValue();
    register8_t mask = (1 << b);

    if ((reg_val & mask) == 0) {
        // Bit b is 0
        cpu->setFlagZ(true);
    } else {
        // Bit b is 1
        cpu->setFlagZ(false);
    }

    cpu->setFlagN(false);
    cpu->setFlagH(true);
}

/**
    Test bit b of data at address in address_reg.
*/
void InstructionDecoder::testBit(int b, const ptr<Register16bit> &reg) {
    byte_t reg_val = loadFromMemory(reg);
    register8_t mask = (1 << b);

    if ((reg_val & mask) == 0) {
        // Bit b is 0
        cpu->setFlagZ(true);
    } else {
        // Bit b is 1
        cpu->setFlagZ(false);
    }

    cpu->setFlagN(false);
    cpu->setFlagH(true);
}

/**
    Sets bit b in reg to 0.
*/
void InstructionDecoder::resetBit(int b, const ptr<Register8bit> &reg) {
    register8_t reg_val = reg->getValue();

    reg_val &= ~(1 << b);

    reg->setValue(reg_val);
}

/**
    Sets bit b in data located at address of address_reg to 0.
*/
void InstructionDecoder::resetBit(int b, const ptr<Register16bit> &reg) {
    // TODO: Probably wrong, should not offset with FF00
    byte_t val = loadFromMemory(reg);

    val &= ~(1 << b);

    loadIntoMemory(reg, val);
}

/**
    Sets bit b in reg to 1.
*/
void InstructionDecoder::setBit(int b, const ptr<Register8bit> &reg) {
    register8_t reg_val = reg->getValue();

    reg_val |= (1 << b);

    reg->setValue(reg_val);
}

/**
    Sets bit b in data located at address of address_reg to 1.
*/
void InstructionDecoder::setBit(int b, const ptr<Register16bit> &reg) {
    byte_t val = loadFromMemory(reg);

    val |= (1 << b);

    loadIntoMemory(reg, val);
}
