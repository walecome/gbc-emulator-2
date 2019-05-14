#include "InstructionDecoder.hh"

/**
    Assign the member pointers for registers etc to those from the processor.
*/
InstructionDecoder::InstructionDecoder(ptr<Processor> processor)
    : cpu { processor },
      program_memory { cpu->program_memory },
      stack { cpu->stack },
      SP { cpu->SP },
      PC { cpu->PC },
      A { cpu->A },
      B { cpu->B },
      C { cpu->C },
      D { cpu->D },
      E { cpu->E },
      F { cpu->F },
      H { cpu->H },
      L { cpu->L },
      AF { cpu->AF },
      BC { cpu->BC },
      DE { cpu->DE },
      HL { cpu->HL } {
    this->map_opcode_functions();
}

void InstructionDecoder::executeInstruction(opcode_t opcode) {
    this->opcode_functions[(int)opcode]();
}

void InstructionDecoder::executeCBInstruction(opcode_t opcode) {
    this->opcode_cb_functions[opcode]();
}

void InstructionDecoder::step(bool verbose) {
    opcode_t instruction = cpu->fetchInstruction();
    PC->increment();
    // TODO check if cycles should be added after instruction execution
    cpu->add_machine_cycles(Timings::opcode_machine_cycles[instruction]);

    if (instruction == 0xCB) {
        if (verbose) std::cout << "Executing CB instruction" << std::endl;
        instruction = cpu->fetchInstruction();
        PC->increment();
        cpu->add_machine_cycles(Timings::opcode_machine_cycles[instruction]);
        executeCBInstruction(instruction);

        // Count CB instructions as two
        cpu->executed_instructions += 2;
    } else {
        if (verbose) std::cout << "Executing regular instruction" << std::endl;
        executeInstruction(instruction);
        cpu->executed_instructions++;
    }

    if (verbose) {
        cpu->dump();
        cpu->printStack();
        cpu->printProgramMemory();
    }
}

void InstructionDecoder::map_opcode_functions() {
    this->map_regular_opcodes();
    this->map_cb_opcodes();
}

opcode_t InstructionDecoder::fetchInstruction() {
    opcode_t opcode = cpu->program_memory->getData(cpu->PC->getValue());

    cpu->PC->increment();

    return opcode;
}

byte_t InstructionDecoder::getInstructionData() {
    byte_t data = cpu->program_memory->getData(cpu->PC->getValue());
    cpu->PC->increment();

    return data;
}

void InstructionDecoder::loadRegister(const ptr<Register8bit> &reg) {
    reg->setValue(getInstructionData());
}

void InstructionDecoder::loadIntoMemory(const ptr<Register16bit> &address_reg,
                                        const ptr<Register8bit> &data_reg) {
    register16_t address = address_reg->getValue();

    byte_t data = data_reg->getValue();

    cpu->program_memory->setData(address, data);
}

void InstructionDecoder::loadIntoMemory(const ptr<Register16bit> &address_reg,
                                        byte_t value) {
    register16_t address = address_reg->getValue();

    cpu->program_memory->setData(address, value);
}

void InstructionDecoder::loadFromMemory(const ptr<Register8bit> &data_reg,
                                        const ptr<Register16bit> &address_reg) {
    register16_t address = address_reg->getValue();

    byte_t data = cpu->program_memory->getData(address);

    data_reg->setValue(data);
}

byte_t InstructionDecoder::loadFromMemory(
    const ptr<Register16bit> &address_reg) {
    register16_t address = address_reg->getValue();

    return cpu->program_memory->getData(address);
}

void InstructionDecoder::incrementRegister(const ptr<Register8bit> &reg) {
    reg->increment();

    register8_t result = reg->getValue();

    cpu->checkFlagZ(result);
    cpu->resetFlagN();
    cpu->checkFlagH(result);
}

void InstructionDecoder::decrementRegister(const ptr<Register8bit> &reg) {
    reg->decrement();

    register8_t result = reg->getValue();

    cpu->checkFlagZ(result);
    cpu->setFlagN();
    cpu->checkFlagH(result);
}

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

void InstructionDecoder::addRegisters(const ptr<Register8bit> &destination,
                                      const ptr<Register8bit> &source) {
    int result = add_helper<Register8bit, register8_t>(destination, source);

    register8_t value_dest = (register8_t)result;

    cpu->checkFlagZ(value_dest);
    cpu->resetFlagN();
    cpu->checkFlagC(result);
    cpu->checkFlagH(value_dest);
}

void InstructionDecoder::addRegisters(const ptr<Register16bit> &destination,
                                      const ptr<Register16bit> &source) {
    int result = add_helper<Register16bit, register16_t>(destination, source);

    register16_t value_dest = (register16_t)result;

    cpu->resetFlagN();
    cpu->checkFlagC(result);
    cpu->checkFlagH(value_dest);
}

void InstructionDecoder::subRegisters(const ptr<Register8bit> &source) {
    register8_t value_dest = cpu->A->getValue();
    register8_t value_source = source->getValue();

    value_dest -= value_source;

    cpu->A->setValue(value_dest);

    cpu->setFlagN();
}

void InstructionDecoder::addWithCarry(const ptr<Register8bit> &destination,
                                      const ptr<Register8bit> &source) {
    register8_t value_dest = destination->getValue();
    register8_t value_source = source->getValue();

    int carry = (cpu->getFlagC() ? 1 : 0);

    int result = value_dest + value_source + carry;

    // Overflow
    if (result > 0xFF) {
        cpu->setFlagC();
    }

    destination->setValue((register8_t)result);
}

void InstructionDecoder::subWithCarry(const ptr<Register8bit> &source) {
    register8_t value_dest = cpu->A->getValue();
    register8_t value_source = source->getValue();
    int carry = (cpu->getFlagC() ? 1 : 0);

    value_dest = value_dest - value_source - carry;

    cpu->A->setValue(value_dest);

    // TODO fix rest of flags. How does negative carry work???
    cpu->setFlagN();
}

void InstructionDecoder::andRegisters(const ptr<Register8bit> &source) {
    register8_t data_a = cpu->A->getValue();
    register8_t data_source = source->getValue();

    // AND
    register8_t result = data_a & data_source;

    cpu->A->setValue(result);
}

void InstructionDecoder::xorRegisters(const ptr<Register8bit> &source) {
    register8_t data_a = cpu->A->getValue();
    register8_t data_source = source->getValue();

    // XOR
    register8_t result = data_a ^ data_source;

    cpu->A->setValue(result);
}

void InstructionDecoder::orRegisters(const ptr<Register8bit> &source) {
    register8_t data_a = cpu->A->getValue();
    register8_t data_source = source->getValue();

    // OR
    register8_t result = data_a | data_source;

    cpu->A->setValue(result);
}

void InstructionDecoder::cmpRegisters(const ptr<Register8bit> &source) {
    register8_t value_a = cpu->A->getValue();
    register8_t value_source = source->getValue();

    if (value_a == value_source)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    // TODO half carry flag
    cpu->setFlagN();

    if (value_a < value_source)
        cpu->setFlagC();
    else
        cpu->resetFlagC();
}

void InstructionDecoder::pushStack(const ptr<Register16bit> &source) {
    cpu->SP->decrement();
    cpu->stack->setData(cpu->SP->getValue(),
                        source->getHighRegister()->getValue());
    cpu->SP->decrement();
    cpu->stack->setData(cpu->SP->getValue(),
                        source->getLowRegister()->getValue());
}

void InstructionDecoder::popStack(const ptr<Register16bit> &destination) {
    byte_t data_low = cpu->stack->getData(cpu->SP->getValue());
    destination->getLowRegister()->setValue(data_low);
    cpu->SP->increment();

    byte_t data_high = cpu->stack->getData(cpu->SP->getValue());
    destination->getHighRegister()->setValue(data_high);
    cpu->SP->increment();
}

void InstructionDecoder::performJump() {
    register16_t current_pc = cpu->PC->getValue();
    // byte_t offset = cpu->program_memory->getData(current_pc);
    byte_t offset = getInstructionData();

    // TODO check this logic
    register16_t new_pc = (register16_t)((int16_t)current_pc + (int8_t)offset);

    cpu->PC->setValue(new_pc);
}

void InstructionDecoder::jumpIm16bit() {
    register8_t data_low = getInstructionData();
    register8_t data_high = getInstructionData();

    cpu->PC->getLowRegister()->setValue(data_low);
    cpu->PC->getHighRegister()->setValue(data_high);
}

void InstructionDecoder::rlcRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();

    if ((data_source & MSB_8BIT) == 0) {
        // MSB is 0
        data_source <<= 1;
        cpu->resetFlagC();
    } else {
        // MSB is 1
        data_source <<= 1;
        data_source |= LSB_8BIT;
        cpu->setFlagC();
    }

    source->setValue(data_source);

    if (data_source == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::rrcRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();

    if ((data_source & LSB_8BIT) == 0) {
        // LSB is 0
        data_source >>= 1;
        cpu->resetFlagC();
    } else {
        // LSB is 1
        data_source >>= 1;
        data_source |= MSB_8BIT;
        cpu->setFlagC();
    }

    source->setValue(data_source);

    if (data_source == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::rlRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();
    register8_t carry = (cpu->getFlagC() ? 1 : 0);

    if ((data_source & MSB_8BIT) == 0) {
        // MSB is 0
        cpu->resetFlagC();
    } else {
        // MSB is 1
        cpu->setFlagC();
    }

    data_source <<= 1;
    data_source |= carry;
    source->setValue(data_source);

    if (data_source == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::rrRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();
    register8_t carry = (cpu->getFlagC() ? MSB_8BIT : 0);

    if ((data_source & LSB_8BIT) == 0) {
        // LSB is 0
        cpu->resetFlagC();
    } else {
        // LSB is 1
        cpu->setFlagC();
    }

    data_source >>= 1;
    data_source |= carry;
    source->setValue(data_source);

    if (data_source == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::slaRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();

    if ((data_source & MSB_8BIT) == 0) {
        // MSB is 0
        cpu->resetFlagC();
    } else {
        cpu->setFlagC();
    }

    data_source <<= 1;

    source->setValue(data_source);

    if (data_source == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::sraRegister(const ptr<Register8bit> &source) {
    register8_t data_source = source->getValue();
    register8_t msb_val = data_source & MSB_8BIT;

    if ((data_source & LSB_8BIT) == 0) {
        // LSB is 0
        cpu->resetFlagC();
    } else {
        // LSB is 1
        cpu->setFlagC();
    }

    data_source >>= 1;
    data_source |= msb_val;

    source->setValue(data_source);

    if (data_source == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::swapNibbles(const ptr<Register8bit> &reg) {
    register8_t val = reg->getValue();

    register8_t new_low = (val & 0xF0) >> 4;
    register8_t new_high = (val & 0x0F) << 4;

    val = new_high + new_low;

    reg->setValue(val);

    if (val == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
    cpu->resetFlagC();
}

void InstructionDecoder::srlRegister(const ptr<Register8bit> &reg) {
    register8_t data_reg = reg->getValue();

    if ((data_reg & LSB_8BIT) == 0) {
        // LSB is 0
        cpu->resetFlagC();
    } else {
        // LSB is 1
        cpu->setFlagC();
    }

    data_reg >>= 1;

    reg->setValue(data_reg);

    if (data_reg == 0x00)
        cpu->setFlagZ();
    else
        cpu->resetFlagZ();

    cpu->resetFlagN();
    cpu->resetFlagH();
}

void InstructionDecoder::testBit(int b, const ptr<Register8bit> &reg) {
    register8_t reg_val = reg->getValue();
    register8_t mask = (1 << b);

    if ((reg_val & mask) == 0) {
        // Bit b is 0
        cpu->setFlagZ();
    } else {
        // Bit b is 1
        cpu->resetFlagZ();
    }

    cpu->resetFlagN();
    cpu->setFlagH();
}

void InstructionDecoder::testBit(int b, const ptr<Register16bit> &reg) {
    byte_t reg_val = loadFromMemory(reg);
    register8_t mask = (1 << b);

    if ((reg_val & mask) == 0) {
        // Bit b is 0
        cpu->setFlagZ();
    } else {
        // Bit b is 1
        cpu->resetFlagZ();
    }

    cpu->resetFlagN();
    cpu->setFlagH();
}

void InstructionDecoder::resetBit(int b, const ptr<Register8bit> &reg) {
    register8_t reg_val = reg->getValue();

    reg_val &= ~(1 << b);

    reg->setValue(reg_val);
}

void InstructionDecoder::resetBit(int b, const ptr<Register16bit> &reg) {
    // TODO: Probably wrong, should not offset with FF00
    byte_t val = loadFromMemory(reg);

    val &= ~(1 << b);

    loadIntoMemory(reg, val);
}

void InstructionDecoder::setBit(int b, const ptr<Register8bit> &reg) {
    register8_t reg_val = reg->getValue();

    reg_val |= (1 << b);

    reg->setValue(reg_val);
}

void InstructionDecoder::setBit(int b, const ptr<Register16bit> &reg) {
    byte_t val = loadFromMemory(reg);

    val |= (1 << b);

    loadIntoMemory(reg, val);
}

void InstructionDecoder::map_regular_opcodes() {
    opcode_functions[0x00] = [&]() { this->OPCode0x00(); };
    opcode_functions[0x01] = [&]() { this->OPCode0x01(); };
    opcode_functions[0x02] = [&]() { this->OPCode0x02(); };
    opcode_functions[0x03] = [&]() { this->OPCode0x03(); };
    opcode_functions[0x04] = [&]() { this->OPCode0x04(); };
    opcode_functions[0x05] = [&]() { this->OPCode0x05(); };
    opcode_functions[0x06] = [&]() { this->OPCode0x06(); };
    opcode_functions[0x07] = [&]() { this->OPCode0x07(); };
    opcode_functions[0x08] = [&]() { this->OPCode0x08(); };
    opcode_functions[0x09] = [&]() { this->OPCode0x09(); };
    opcode_functions[0x0A] = [&]() { this->OPCode0x0A(); };
    opcode_functions[0x0B] = [&]() { this->OPCode0x0B(); };
    opcode_functions[0x0C] = [&]() { this->OPCode0x0C(); };
    opcode_functions[0x0D] = [&]() { this->OPCode0x0D(); };
    opcode_functions[0x0E] = [&]() { this->OPCode0x0E(); };
    opcode_functions[0x0F] = [&]() { this->OPCode0x0F(); };
    opcode_functions[0x10] = [&]() { this->OPCode0x10(); };
    opcode_functions[0x11] = [&]() { this->OPCode0x11(); };
    opcode_functions[0x12] = [&]() { this->OPCode0x12(); };
    opcode_functions[0x13] = [&]() { this->OPCode0x13(); };
    opcode_functions[0x14] = [&]() { this->OPCode0x14(); };
    opcode_functions[0x15] = [&]() { this->OPCode0x15(); };
    opcode_functions[0x16] = [&]() { this->OPCode0x16(); };
    opcode_functions[0x17] = [&]() { this->OPCode0x17(); };
    opcode_functions[0x18] = [&]() { this->OPCode0x18(); };
    opcode_functions[0x19] = [&]() { this->OPCode0x19(); };
    opcode_functions[0x1A] = [&]() { this->OPCode0x1A(); };
    opcode_functions[0x1B] = [&]() { this->OPCode0x1B(); };
    opcode_functions[0x1C] = [&]() { this->OPCode0x1C(); };
    opcode_functions[0x1D] = [&]() { this->OPCode0x1D(); };
    opcode_functions[0x1E] = [&]() { this->OPCode0x1E(); };
    opcode_functions[0x1F] = [&]() { this->OPCode0x1F(); };
    opcode_functions[0x20] = [&]() { this->OPCode0x20(); };
    opcode_functions[0x21] = [&]() { this->OPCode0x21(); };
    opcode_functions[0x22] = [&]() { this->OPCode0x22(); };
    opcode_functions[0x23] = [&]() { this->OPCode0x23(); };
    opcode_functions[0x24] = [&]() { this->OPCode0x24(); };
    opcode_functions[0x25] = [&]() { this->OPCode0x25(); };
    opcode_functions[0x26] = [&]() { this->OPCode0x26(); };
    opcode_functions[0x27] = [&]() { this->OPCode0x27(); };
    opcode_functions[0x28] = [&]() { this->OPCode0x28(); };
    opcode_functions[0x29] = [&]() { this->OPCode0x29(); };
    opcode_functions[0x2A] = [&]() { this->OPCode0x2A(); };
    opcode_functions[0x2B] = [&]() { this->OPCode0x2B(); };
    opcode_functions[0x2C] = [&]() { this->OPCode0x2C(); };
    opcode_functions[0x2D] = [&]() { this->OPCode0x2D(); };
    opcode_functions[0x2E] = [&]() { this->OPCode0x2E(); };
    opcode_functions[0x2F] = [&]() { this->OPCode0x2F(); };
    opcode_functions[0x30] = [&]() { this->OPCode0x30(); };
    opcode_functions[0x31] = [&]() { this->OPCode0x31(); };
    opcode_functions[0x32] = [&]() { this->OPCode0x32(); };
    opcode_functions[0x33] = [&]() { this->OPCode0x33(); };
    opcode_functions[0x34] = [&]() { this->OPCode0x34(); };
    opcode_functions[0x35] = [&]() { this->OPCode0x35(); };
    opcode_functions[0x36] = [&]() { this->OPCode0x36(); };
    opcode_functions[0x37] = [&]() { this->OPCode0x37(); };
    opcode_functions[0x38] = [&]() { this->OPCode0x38(); };
    opcode_functions[0x39] = [&]() { this->OPCode0x39(); };
    opcode_functions[0x3A] = [&]() { this->OPCode0x3A(); };
    opcode_functions[0x3B] = [&]() { this->OPCode0x3B(); };
    opcode_functions[0x3C] = [&]() { this->OPCode0x3C(); };
    opcode_functions[0x3D] = [&]() { this->OPCode0x3D(); };
    opcode_functions[0x3E] = [&]() { this->OPCode0x3E(); };
    opcode_functions[0x3F] = [&]() { this->OPCode0x3F(); };
    opcode_functions[0x40] = [&]() { this->OPCode0x40(); };
    opcode_functions[0x41] = [&]() { this->OPCode0x41(); };
    opcode_functions[0x42] = [&]() { this->OPCode0x42(); };
    opcode_functions[0x43] = [&]() { this->OPCode0x43(); };
    opcode_functions[0x44] = [&]() { this->OPCode0x44(); };
    opcode_functions[0x45] = [&]() { this->OPCode0x45(); };
    opcode_functions[0x46] = [&]() { this->OPCode0x46(); };
    opcode_functions[0x47] = [&]() { this->OPCode0x47(); };
    opcode_functions[0x48] = [&]() { this->OPCode0x48(); };
    opcode_functions[0x49] = [&]() { this->OPCode0x49(); };
    opcode_functions[0x4A] = [&]() { this->OPCode0x4A(); };
    opcode_functions[0x4B] = [&]() { this->OPCode0x4B(); };
    opcode_functions[0x4C] = [&]() { this->OPCode0x4C(); };
    opcode_functions[0x4D] = [&]() { this->OPCode0x4D(); };
    opcode_functions[0x4E] = [&]() { this->OPCode0x4E(); };
    opcode_functions[0x4F] = [&]() { this->OPCode0x4F(); };
    opcode_functions[0x50] = [&]() { this->OPCode0x50(); };
    opcode_functions[0x51] = [&]() { this->OPCode0x51(); };
    opcode_functions[0x52] = [&]() { this->OPCode0x52(); };
    opcode_functions[0x53] = [&]() { this->OPCode0x53(); };
    opcode_functions[0x54] = [&]() { this->OPCode0x54(); };
    opcode_functions[0x55] = [&]() { this->OPCode0x55(); };
    opcode_functions[0x56] = [&]() { this->OPCode0x56(); };
    opcode_functions[0x57] = [&]() { this->OPCode0x57(); };
    opcode_functions[0x58] = [&]() { this->OPCode0x58(); };
    opcode_functions[0x59] = [&]() { this->OPCode0x59(); };
    opcode_functions[0x5A] = [&]() { this->OPCode0x5A(); };
    opcode_functions[0x5B] = [&]() { this->OPCode0x5B(); };
    opcode_functions[0x5C] = [&]() { this->OPCode0x5C(); };
    opcode_functions[0x5D] = [&]() { this->OPCode0x5D(); };
    opcode_functions[0x5E] = [&]() { this->OPCode0x5E(); };
    opcode_functions[0x5F] = [&]() { this->OPCode0x5F(); };
    opcode_functions[0x60] = [&]() { this->OPCode0x60(); };
    opcode_functions[0x61] = [&]() { this->OPCode0x61(); };
    opcode_functions[0x62] = [&]() { this->OPCode0x62(); };
    opcode_functions[0x63] = [&]() { this->OPCode0x63(); };
    opcode_functions[0x64] = [&]() { this->OPCode0x64(); };
    opcode_functions[0x65] = [&]() { this->OPCode0x65(); };
    opcode_functions[0x66] = [&]() { this->OPCode0x66(); };
    opcode_functions[0x67] = [&]() { this->OPCode0x67(); };
    opcode_functions[0x68] = [&]() { this->OPCode0x68(); };
    opcode_functions[0x69] = [&]() { this->OPCode0x69(); };
    opcode_functions[0x6A] = [&]() { this->OPCode0x6A(); };
    opcode_functions[0x6B] = [&]() { this->OPCode0x6B(); };
    opcode_functions[0x6C] = [&]() { this->OPCode0x6C(); };
    opcode_functions[0x6D] = [&]() { this->OPCode0x6D(); };
    opcode_functions[0x6E] = [&]() { this->OPCode0x6E(); };
    opcode_functions[0x6F] = [&]() { this->OPCode0x6F(); };
    opcode_functions[0x70] = [&]() { this->OPCode0x70(); };
    opcode_functions[0x71] = [&]() { this->OPCode0x71(); };
    opcode_functions[0x72] = [&]() { this->OPCode0x72(); };
    opcode_functions[0x73] = [&]() { this->OPCode0x73(); };
    opcode_functions[0x74] = [&]() { this->OPCode0x74(); };
    opcode_functions[0x75] = [&]() { this->OPCode0x75(); };
    opcode_functions[0x76] = [&]() { this->OPCode0x76(); };
    opcode_functions[0x77] = [&]() { this->OPCode0x77(); };
    opcode_functions[0x78] = [&]() { this->OPCode0x78(); };
    opcode_functions[0x79] = [&]() { this->OPCode0x79(); };
    opcode_functions[0x7A] = [&]() { this->OPCode0x7A(); };
    opcode_functions[0x7B] = [&]() { this->OPCode0x7B(); };
    opcode_functions[0x7C] = [&]() { this->OPCode0x7C(); };
    opcode_functions[0x7D] = [&]() { this->OPCode0x7D(); };
    opcode_functions[0x7E] = [&]() { this->OPCode0x7E(); };
    opcode_functions[0x7F] = [&]() { this->OPCode0x7F(); };
    opcode_functions[0x80] = [&]() { this->OPCode0x80(); };
    opcode_functions[0x81] = [&]() { this->OPCode0x81(); };
    opcode_functions[0x82] = [&]() { this->OPCode0x82(); };
    opcode_functions[0x83] = [&]() { this->OPCode0x83(); };
    opcode_functions[0x84] = [&]() { this->OPCode0x84(); };
    opcode_functions[0x85] = [&]() { this->OPCode0x85(); };
    opcode_functions[0x86] = [&]() { this->OPCode0x86(); };
    opcode_functions[0x87] = [&]() { this->OPCode0x87(); };
    opcode_functions[0x88] = [&]() { this->OPCode0x88(); };
    opcode_functions[0x89] = [&]() { this->OPCode0x89(); };
    opcode_functions[0x8A] = [&]() { this->OPCode0x8A(); };
    opcode_functions[0x8B] = [&]() { this->OPCode0x8B(); };
    opcode_functions[0x8C] = [&]() { this->OPCode0x8C(); };
    opcode_functions[0x8D] = [&]() { this->OPCode0x8D(); };
    opcode_functions[0x8E] = [&]() { this->OPCode0x8E(); };
    opcode_functions[0x8F] = [&]() { this->OPCode0x8F(); };
    opcode_functions[0x90] = [&]() { this->OPCode0x90(); };
    opcode_functions[0x91] = [&]() { this->OPCode0x91(); };
    opcode_functions[0x92] = [&]() { this->OPCode0x92(); };
    opcode_functions[0x93] = [&]() { this->OPCode0x93(); };
    opcode_functions[0x94] = [&]() { this->OPCode0x94(); };
    opcode_functions[0x95] = [&]() { this->OPCode0x95(); };
    opcode_functions[0x96] = [&]() { this->OPCode0x96(); };
    opcode_functions[0x97] = [&]() { this->OPCode0x97(); };
    opcode_functions[0x98] = [&]() { this->OPCode0x98(); };
    opcode_functions[0x99] = [&]() { this->OPCode0x99(); };
    opcode_functions[0x9A] = [&]() { this->OPCode0x9A(); };
    opcode_functions[0x9B] = [&]() { this->OPCode0x9B(); };
    opcode_functions[0x9C] = [&]() { this->OPCode0x9C(); };
    opcode_functions[0x9D] = [&]() { this->OPCode0x9D(); };
    opcode_functions[0x9E] = [&]() { this->OPCode0x9E(); };
    opcode_functions[0x9F] = [&]() { this->OPCode0x9F(); };
    opcode_functions[0xA0] = [&]() { this->OPCode0xA0(); };
    opcode_functions[0xA1] = [&]() { this->OPCode0xA1(); };
    opcode_functions[0xA2] = [&]() { this->OPCode0xA2(); };
    opcode_functions[0xA3] = [&]() { this->OPCode0xA3(); };
    opcode_functions[0xA4] = [&]() { this->OPCode0xA4(); };
    opcode_functions[0xA5] = [&]() { this->OPCode0xA5(); };
    opcode_functions[0xA6] = [&]() { this->OPCode0xA6(); };
    opcode_functions[0xA7] = [&]() { this->OPCode0xA7(); };
    opcode_functions[0xA8] = [&]() { this->OPCode0xA8(); };
    opcode_functions[0xA9] = [&]() { this->OPCode0xA9(); };
    opcode_functions[0xAA] = [&]() { this->OPCode0xAA(); };
    opcode_functions[0xAB] = [&]() { this->OPCode0xAB(); };
    opcode_functions[0xAC] = [&]() { this->OPCode0xAC(); };
    opcode_functions[0xAD] = [&]() { this->OPCode0xAD(); };
    opcode_functions[0xAE] = [&]() { this->OPCode0xAE(); };
    opcode_functions[0xAF] = [&]() { this->OPCode0xAF(); };
    opcode_functions[0xB0] = [&]() { this->OPCode0xB0(); };
    opcode_functions[0xB1] = [&]() { this->OPCode0xB1(); };
    opcode_functions[0xB2] = [&]() { this->OPCode0xB2(); };
    opcode_functions[0xB3] = [&]() { this->OPCode0xB3(); };
    opcode_functions[0xB4] = [&]() { this->OPCode0xB4(); };
    opcode_functions[0xB5] = [&]() { this->OPCode0xB5(); };
    opcode_functions[0xB6] = [&]() { this->OPCode0xB6(); };
    opcode_functions[0xB7] = [&]() { this->OPCode0xB7(); };
    opcode_functions[0xB8] = [&]() { this->OPCode0xB8(); };
    opcode_functions[0xB9] = [&]() { this->OPCode0xB9(); };
    opcode_functions[0xBA] = [&]() { this->OPCode0xBA(); };
    opcode_functions[0xBB] = [&]() { this->OPCode0xBB(); };
    opcode_functions[0xBC] = [&]() { this->OPCode0xBC(); };
    opcode_functions[0xBD] = [&]() { this->OPCode0xBD(); };
    opcode_functions[0xBE] = [&]() { this->OPCode0xBE(); };
    opcode_functions[0xBF] = [&]() { this->OPCode0xBF(); };
    opcode_functions[0xC0] = [&]() { this->OPCode0xC0(); };
    opcode_functions[0xC1] = [&]() { this->OPCode0xC1(); };
    opcode_functions[0xC2] = [&]() { this->OPCode0xC2(); };
    opcode_functions[0xC3] = [&]() { this->OPCode0xC3(); };
    opcode_functions[0xC4] = [&]() { this->OPCode0xC4(); };
    opcode_functions[0xC5] = [&]() { this->OPCode0xC5(); };
    opcode_functions[0xC6] = [&]() { this->OPCode0xC6(); };
    opcode_functions[0xC7] = [&]() { this->OPCode0xC7(); };
    opcode_functions[0xC8] = [&]() { this->OPCode0xC8(); };
    opcode_functions[0xC9] = [&]() { this->OPCode0xC9(); };
    opcode_functions[0xCA] = [&]() { this->OPCode0xCA(); };
    opcode_functions[0xCB] = [&]() { this->OPCode0xCB(); };
    opcode_functions[0xCC] = [&]() { this->OPCode0xCC(); };
    opcode_functions[0xCD] = [&]() { this->OPCode0xCD(); };
    opcode_functions[0xCE] = [&]() { this->OPCode0xCE(); };
    opcode_functions[0xCF] = [&]() { this->OPCode0xCF(); };
    opcode_functions[0xD0] = [&]() { this->OPCode0xD0(); };
    opcode_functions[0xD1] = [&]() { this->OPCode0xD1(); };
    opcode_functions[0xD2] = [&]() { this->OPCode0xD2(); };
    opcode_functions[0xD3] = [&]() { this->OPCode0xD3(); };
    opcode_functions[0xD4] = [&]() { this->OPCode0xD4(); };
    opcode_functions[0xD5] = [&]() { this->OPCode0xD5(); };
    opcode_functions[0xD6] = [&]() { this->OPCode0xD6(); };
    opcode_functions[0xD7] = [&]() { this->OPCode0xD7(); };
    opcode_functions[0xD8] = [&]() { this->OPCode0xD8(); };
    opcode_functions[0xD9] = [&]() { this->OPCode0xD9(); };
    opcode_functions[0xDA] = [&]() { this->OPCode0xDA(); };
    opcode_functions[0xDB] = [&]() { this->OPCode0xDB(); };
    opcode_functions[0xDC] = [&]() { this->OPCode0xDC(); };
    opcode_functions[0xDD] = [&]() { this->OPCode0xDD(); };
    opcode_functions[0xDE] = [&]() { this->OPCode0xDE(); };
    opcode_functions[0xDF] = [&]() { this->OPCode0xDF(); };
    opcode_functions[0xE0] = [&]() { this->OPCode0xE0(); };
    opcode_functions[0xE1] = [&]() { this->OPCode0xE1(); };
    opcode_functions[0xE2] = [&]() { this->OPCode0xE2(); };
    opcode_functions[0xE3] = [&]() { this->OPCode0xE3(); };
    opcode_functions[0xE4] = [&]() { this->OPCode0xE4(); };
    opcode_functions[0xE5] = [&]() { this->OPCode0xE5(); };
    opcode_functions[0xE6] = [&]() { this->OPCode0xE6(); };
    opcode_functions[0xE7] = [&]() { this->OPCode0xE7(); };
    opcode_functions[0xE8] = [&]() { this->OPCode0xE8(); };
    opcode_functions[0xE9] = [&]() { this->OPCode0xE9(); };
    opcode_functions[0xEA] = [&]() { this->OPCode0xEA(); };
    opcode_functions[0xEB] = [&]() { this->OPCode0xEB(); };
    opcode_functions[0xEC] = [&]() { this->OPCode0xEC(); };
    opcode_functions[0xED] = [&]() { this->OPCode0xED(); };
    opcode_functions[0xEE] = [&]() { this->OPCode0xEE(); };
    opcode_functions[0xEF] = [&]() { this->OPCode0xEF(); };
    opcode_functions[0xF0] = [&]() { this->OPCode0xF0(); };
    opcode_functions[0xF1] = [&]() { this->OPCode0xF1(); };
    opcode_functions[0xF2] = [&]() { this->OPCode0xF2(); };
    opcode_functions[0xF3] = [&]() { this->OPCode0xF3(); };
    opcode_functions[0xF4] = [&]() { this->OPCode0xF4(); };
    opcode_functions[0xF5] = [&]() { this->OPCode0xF5(); };
    opcode_functions[0xF6] = [&]() { this->OPCode0xF6(); };
    opcode_functions[0xF7] = [&]() { this->OPCode0xF7(); };
    opcode_functions[0xF8] = [&]() { this->OPCode0xF8(); };
    opcode_functions[0xF9] = [&]() { this->OPCode0xF9(); };
    opcode_functions[0xFA] = [&]() { this->OPCode0xFA(); };
    opcode_functions[0xFB] = [&]() { this->OPCode0xFB(); };
    opcode_functions[0xFC] = [&]() { this->OPCode0xFC(); };
    opcode_functions[0xFD] = [&]() { this->OPCode0xFD(); };
    opcode_functions[0xFE] = [&]() { this->OPCode0xFE(); };
    opcode_functions[0xFF] = [&]() { this->OPCode0xFF(); };
}

void InstructionDecoder::map_cb_opcodes() {
    opcode_cb_functions[0x00] = [&]() { this->OPCodeCB0x00(); };
    opcode_cb_functions[0x01] = [&]() { this->OPCodeCB0x01(); };
    opcode_cb_functions[0x02] = [&]() { this->OPCodeCB0x02(); };
    opcode_cb_functions[0x03] = [&]() { this->OPCodeCB0x03(); };
    opcode_cb_functions[0x04] = [&]() { this->OPCodeCB0x04(); };
    opcode_cb_functions[0x05] = [&]() { this->OPCodeCB0x05(); };
    opcode_cb_functions[0x06] = [&]() { this->OPCodeCB0x06(); };
    opcode_cb_functions[0x07] = [&]() { this->OPCodeCB0x07(); };
    opcode_cb_functions[0x08] = [&]() { this->OPCodeCB0x08(); };
    opcode_cb_functions[0x09] = [&]() { this->OPCodeCB0x09(); };
    opcode_cb_functions[0x0A] = [&]() { this->OPCodeCB0x0A(); };
    opcode_cb_functions[0x0B] = [&]() { this->OPCodeCB0x0B(); };
    opcode_cb_functions[0x0C] = [&]() { this->OPCodeCB0x0C(); };
    opcode_cb_functions[0x0D] = [&]() { this->OPCodeCB0x0D(); };
    opcode_cb_functions[0x0E] = [&]() { this->OPCodeCB0x0E(); };
    opcode_cb_functions[0x0F] = [&]() { this->OPCodeCB0x0F(); };
    opcode_cb_functions[0x10] = [&]() { this->OPCodeCB0x10(); };
    opcode_cb_functions[0x11] = [&]() { this->OPCodeCB0x11(); };
    opcode_cb_functions[0x12] = [&]() { this->OPCodeCB0x12(); };
    opcode_cb_functions[0x13] = [&]() { this->OPCodeCB0x13(); };
    opcode_cb_functions[0x14] = [&]() { this->OPCodeCB0x14(); };
    opcode_cb_functions[0x15] = [&]() { this->OPCodeCB0x15(); };
    opcode_cb_functions[0x16] = [&]() { this->OPCodeCB0x16(); };
    opcode_cb_functions[0x17] = [&]() { this->OPCodeCB0x17(); };
    opcode_cb_functions[0x18] = [&]() { this->OPCodeCB0x18(); };
    opcode_cb_functions[0x19] = [&]() { this->OPCodeCB0x19(); };
    opcode_cb_functions[0x1A] = [&]() { this->OPCodeCB0x1A(); };
    opcode_cb_functions[0x1B] = [&]() { this->OPCodeCB0x1B(); };
    opcode_cb_functions[0x1C] = [&]() { this->OPCodeCB0x1C(); };
    opcode_cb_functions[0x1D] = [&]() { this->OPCodeCB0x1D(); };
    opcode_cb_functions[0x1E] = [&]() { this->OPCodeCB0x1E(); };
    opcode_cb_functions[0x1F] = [&]() { this->OPCodeCB0x1F(); };
    opcode_cb_functions[0x20] = [&]() { this->OPCodeCB0x20(); };
    opcode_cb_functions[0x21] = [&]() { this->OPCodeCB0x21(); };
    opcode_cb_functions[0x22] = [&]() { this->OPCodeCB0x22(); };
    opcode_cb_functions[0x23] = [&]() { this->OPCodeCB0x23(); };
    opcode_cb_functions[0x24] = [&]() { this->OPCodeCB0x24(); };
    opcode_cb_functions[0x25] = [&]() { this->OPCodeCB0x25(); };
    opcode_cb_functions[0x26] = [&]() { this->OPCodeCB0x26(); };
    opcode_cb_functions[0x27] = [&]() { this->OPCodeCB0x27(); };
    opcode_cb_functions[0x28] = [&]() { this->OPCodeCB0x28(); };
    opcode_cb_functions[0x29] = [&]() { this->OPCodeCB0x29(); };
    opcode_cb_functions[0x2A] = [&]() { this->OPCodeCB0x2A(); };
    opcode_cb_functions[0x2B] = [&]() { this->OPCodeCB0x2B(); };
    opcode_cb_functions[0x2C] = [&]() { this->OPCodeCB0x2C(); };
    opcode_cb_functions[0x2D] = [&]() { this->OPCodeCB0x2D(); };
    opcode_cb_functions[0x2E] = [&]() { this->OPCodeCB0x2E(); };
    opcode_cb_functions[0x2F] = [&]() { this->OPCodeCB0x2F(); };
    opcode_cb_functions[0x30] = [&]() { this->OPCodeCB0x30(); };
    opcode_cb_functions[0x31] = [&]() { this->OPCodeCB0x31(); };
    opcode_cb_functions[0x32] = [&]() { this->OPCodeCB0x32(); };
    opcode_cb_functions[0x33] = [&]() { this->OPCodeCB0x33(); };
    opcode_cb_functions[0x34] = [&]() { this->OPCodeCB0x34(); };
    opcode_cb_functions[0x35] = [&]() { this->OPCodeCB0x35(); };
    opcode_cb_functions[0x36] = [&]() { this->OPCodeCB0x36(); };
    opcode_cb_functions[0x37] = [&]() { this->OPCodeCB0x37(); };
    opcode_cb_functions[0x38] = [&]() { this->OPCodeCB0x38(); };
    opcode_cb_functions[0x39] = [&]() { this->OPCodeCB0x39(); };
    opcode_cb_functions[0x3A] = [&]() { this->OPCodeCB0x3A(); };
    opcode_cb_functions[0x3B] = [&]() { this->OPCodeCB0x3B(); };
    opcode_cb_functions[0x3C] = [&]() { this->OPCodeCB0x3C(); };
    opcode_cb_functions[0x3D] = [&]() { this->OPCodeCB0x3D(); };
    opcode_cb_functions[0x3E] = [&]() { this->OPCodeCB0x3E(); };
    opcode_cb_functions[0x3F] = [&]() { this->OPCodeCB0x3F(); };
    opcode_cb_functions[0x40] = [&]() { this->OPCodeCB0x40(); };
    opcode_cb_functions[0x41] = [&]() { this->OPCodeCB0x41(); };
    opcode_cb_functions[0x42] = [&]() { this->OPCodeCB0x42(); };
    opcode_cb_functions[0x43] = [&]() { this->OPCodeCB0x43(); };
    opcode_cb_functions[0x44] = [&]() { this->OPCodeCB0x44(); };
    opcode_cb_functions[0x45] = [&]() { this->OPCodeCB0x45(); };
    opcode_cb_functions[0x46] = [&]() { this->OPCodeCB0x46(); };
    opcode_cb_functions[0x47] = [&]() { this->OPCodeCB0x47(); };
    opcode_cb_functions[0x48] = [&]() { this->OPCodeCB0x48(); };
    opcode_cb_functions[0x49] = [&]() { this->OPCodeCB0x49(); };
    opcode_cb_functions[0x4A] = [&]() { this->OPCodeCB0x4A(); };
    opcode_cb_functions[0x4B] = [&]() { this->OPCodeCB0x4B(); };
    opcode_cb_functions[0x4C] = [&]() { this->OPCodeCB0x4C(); };
    opcode_cb_functions[0x4D] = [&]() { this->OPCodeCB0x4D(); };
    opcode_cb_functions[0x4E] = [&]() { this->OPCodeCB0x4E(); };
    opcode_cb_functions[0x4F] = [&]() { this->OPCodeCB0x4F(); };
    opcode_cb_functions[0x50] = [&]() { this->OPCodeCB0x50(); };
    opcode_cb_functions[0x51] = [&]() { this->OPCodeCB0x51(); };
    opcode_cb_functions[0x52] = [&]() { this->OPCodeCB0x52(); };
    opcode_cb_functions[0x53] = [&]() { this->OPCodeCB0x53(); };
    opcode_cb_functions[0x54] = [&]() { this->OPCodeCB0x54(); };
    opcode_cb_functions[0x55] = [&]() { this->OPCodeCB0x55(); };
    opcode_cb_functions[0x56] = [&]() { this->OPCodeCB0x56(); };
    opcode_cb_functions[0x57] = [&]() { this->OPCodeCB0x57(); };
    opcode_cb_functions[0x58] = [&]() { this->OPCodeCB0x58(); };
    opcode_cb_functions[0x59] = [&]() { this->OPCodeCB0x59(); };
    opcode_cb_functions[0x5A] = [&]() { this->OPCodeCB0x5A(); };
    opcode_cb_functions[0x5B] = [&]() { this->OPCodeCB0x5B(); };
    opcode_cb_functions[0x5C] = [&]() { this->OPCodeCB0x5C(); };
    opcode_cb_functions[0x5D] = [&]() { this->OPCodeCB0x5D(); };
    opcode_cb_functions[0x5E] = [&]() { this->OPCodeCB0x5E(); };
    opcode_cb_functions[0x5F] = [&]() { this->OPCodeCB0x5F(); };
    opcode_cb_functions[0x60] = [&]() { this->OPCodeCB0x60(); };
    opcode_cb_functions[0x61] = [&]() { this->OPCodeCB0x61(); };
    opcode_cb_functions[0x62] = [&]() { this->OPCodeCB0x62(); };
    opcode_cb_functions[0x63] = [&]() { this->OPCodeCB0x63(); };
    opcode_cb_functions[0x64] = [&]() { this->OPCodeCB0x64(); };
    opcode_cb_functions[0x65] = [&]() { this->OPCodeCB0x65(); };
    opcode_cb_functions[0x66] = [&]() { this->OPCodeCB0x66(); };
    opcode_cb_functions[0x67] = [&]() { this->OPCodeCB0x67(); };
    opcode_cb_functions[0x68] = [&]() { this->OPCodeCB0x68(); };
    opcode_cb_functions[0x69] = [&]() { this->OPCodeCB0x69(); };
    opcode_cb_functions[0x6A] = [&]() { this->OPCodeCB0x6A(); };
    opcode_cb_functions[0x6B] = [&]() { this->OPCodeCB0x6B(); };
    opcode_cb_functions[0x6C] = [&]() { this->OPCodeCB0x6C(); };
    opcode_cb_functions[0x6D] = [&]() { this->OPCodeCB0x6D(); };
    opcode_cb_functions[0x6E] = [&]() { this->OPCodeCB0x6E(); };
    opcode_cb_functions[0x6F] = [&]() { this->OPCodeCB0x6F(); };
    opcode_cb_functions[0x70] = [&]() { this->OPCodeCB0x70(); };
    opcode_cb_functions[0x71] = [&]() { this->OPCodeCB0x71(); };
    opcode_cb_functions[0x72] = [&]() { this->OPCodeCB0x72(); };
    opcode_cb_functions[0x73] = [&]() { this->OPCodeCB0x73(); };
    opcode_cb_functions[0x74] = [&]() { this->OPCodeCB0x74(); };
    opcode_cb_functions[0x75] = [&]() { this->OPCodeCB0x75(); };
    opcode_cb_functions[0x76] = [&]() { this->OPCodeCB0x76(); };
    opcode_cb_functions[0x77] = [&]() { this->OPCodeCB0x77(); };
    opcode_cb_functions[0x78] = [&]() { this->OPCodeCB0x78(); };
    opcode_cb_functions[0x79] = [&]() { this->OPCodeCB0x79(); };
    opcode_cb_functions[0x7A] = [&]() { this->OPCodeCB0x7A(); };
    opcode_cb_functions[0x7B] = [&]() { this->OPCodeCB0x7B(); };
    opcode_cb_functions[0x7C] = [&]() { this->OPCodeCB0x7C(); };
    opcode_cb_functions[0x7D] = [&]() { this->OPCodeCB0x7D(); };
    opcode_cb_functions[0x7E] = [&]() { this->OPCodeCB0x7E(); };
    opcode_cb_functions[0x7F] = [&]() { this->OPCodeCB0x7F(); };
    opcode_cb_functions[0x80] = [&]() { this->OPCodeCB0x80(); };
    opcode_cb_functions[0x81] = [&]() { this->OPCodeCB0x81(); };
    opcode_cb_functions[0x82] = [&]() { this->OPCodeCB0x82(); };
    opcode_cb_functions[0x83] = [&]() { this->OPCodeCB0x83(); };
    opcode_cb_functions[0x84] = [&]() { this->OPCodeCB0x84(); };
    opcode_cb_functions[0x85] = [&]() { this->OPCodeCB0x85(); };
    opcode_cb_functions[0x86] = [&]() { this->OPCodeCB0x86(); };
    opcode_cb_functions[0x87] = [&]() { this->OPCodeCB0x87(); };
    opcode_cb_functions[0x88] = [&]() { this->OPCodeCB0x88(); };
    opcode_cb_functions[0x89] = [&]() { this->OPCodeCB0x89(); };
    opcode_cb_functions[0x8A] = [&]() { this->OPCodeCB0x8A(); };
    opcode_cb_functions[0x8B] = [&]() { this->OPCodeCB0x8B(); };
    opcode_cb_functions[0x8C] = [&]() { this->OPCodeCB0x8C(); };
    opcode_cb_functions[0x8D] = [&]() { this->OPCodeCB0x8D(); };
    opcode_cb_functions[0x8E] = [&]() { this->OPCodeCB0x8E(); };
    opcode_cb_functions[0x8F] = [&]() { this->OPCodeCB0x8F(); };
    opcode_cb_functions[0x90] = [&]() { this->OPCodeCB0x90(); };
    opcode_cb_functions[0x91] = [&]() { this->OPCodeCB0x91(); };
    opcode_cb_functions[0x92] = [&]() { this->OPCodeCB0x92(); };
    opcode_cb_functions[0x93] = [&]() { this->OPCodeCB0x93(); };
    opcode_cb_functions[0x94] = [&]() { this->OPCodeCB0x94(); };
    opcode_cb_functions[0x95] = [&]() { this->OPCodeCB0x95(); };
    opcode_cb_functions[0x96] = [&]() { this->OPCodeCB0x96(); };
    opcode_cb_functions[0x97] = [&]() { this->OPCodeCB0x97(); };
    opcode_cb_functions[0x98] = [&]() { this->OPCodeCB0x98(); };
    opcode_cb_functions[0x99] = [&]() { this->OPCodeCB0x99(); };
    opcode_cb_functions[0x9A] = [&]() { this->OPCodeCB0x9A(); };
    opcode_cb_functions[0x9B] = [&]() { this->OPCodeCB0x9B(); };
    opcode_cb_functions[0x9C] = [&]() { this->OPCodeCB0x9C(); };
    opcode_cb_functions[0x9D] = [&]() { this->OPCodeCB0x9D(); };
    opcode_cb_functions[0x9E] = [&]() { this->OPCodeCB0x9E(); };
    opcode_cb_functions[0x9F] = [&]() { this->OPCodeCB0x9F(); };
    opcode_cb_functions[0xA0] = [&]() { this->OPCodeCB0xA0(); };
    opcode_cb_functions[0xA1] = [&]() { this->OPCodeCB0xA1(); };
    opcode_cb_functions[0xA2] = [&]() { this->OPCodeCB0xA2(); };
    opcode_cb_functions[0xA3] = [&]() { this->OPCodeCB0xA3(); };
    opcode_cb_functions[0xA4] = [&]() { this->OPCodeCB0xA4(); };
    opcode_cb_functions[0xA5] = [&]() { this->OPCodeCB0xA5(); };
    opcode_cb_functions[0xA6] = [&]() { this->OPCodeCB0xA6(); };
    opcode_cb_functions[0xA7] = [&]() { this->OPCodeCB0xA7(); };
    opcode_cb_functions[0xA8] = [&]() { this->OPCodeCB0xA8(); };
    opcode_cb_functions[0xA9] = [&]() { this->OPCodeCB0xA9(); };
    opcode_cb_functions[0xAA] = [&]() { this->OPCodeCB0xAA(); };
    opcode_cb_functions[0xAB] = [&]() { this->OPCodeCB0xAB(); };
    opcode_cb_functions[0xAC] = [&]() { this->OPCodeCB0xAC(); };
    opcode_cb_functions[0xAD] = [&]() { this->OPCodeCB0xAD(); };
    opcode_cb_functions[0xAE] = [&]() { this->OPCodeCB0xAE(); };
    opcode_cb_functions[0xAF] = [&]() { this->OPCodeCB0xAF(); };
    opcode_cb_functions[0xB0] = [&]() { this->OPCodeCB0xB0(); };
    opcode_cb_functions[0xB1] = [&]() { this->OPCodeCB0xB1(); };
    opcode_cb_functions[0xB2] = [&]() { this->OPCodeCB0xB2(); };
    opcode_cb_functions[0xB3] = [&]() { this->OPCodeCB0xB3(); };
    opcode_cb_functions[0xB4] = [&]() { this->OPCodeCB0xB4(); };
    opcode_cb_functions[0xB5] = [&]() { this->OPCodeCB0xB5(); };
    opcode_cb_functions[0xB6] = [&]() { this->OPCodeCB0xB6(); };
    opcode_cb_functions[0xB7] = [&]() { this->OPCodeCB0xB7(); };
    opcode_cb_functions[0xB8] = [&]() { this->OPCodeCB0xB8(); };
    opcode_cb_functions[0xB9] = [&]() { this->OPCodeCB0xB9(); };
    opcode_cb_functions[0xBA] = [&]() { this->OPCodeCB0xBA(); };
    opcode_cb_functions[0xBB] = [&]() { this->OPCodeCB0xBB(); };
    opcode_cb_functions[0xBC] = [&]() { this->OPCodeCB0xBC(); };
    opcode_cb_functions[0xBD] = [&]() { this->OPCodeCB0xBD(); };
    opcode_cb_functions[0xBE] = [&]() { this->OPCodeCB0xBE(); };
    opcode_cb_functions[0xBF] = [&]() { this->OPCodeCB0xBF(); };
    opcode_cb_functions[0xC0] = [&]() { this->OPCodeCB0xC0(); };
    opcode_cb_functions[0xC1] = [&]() { this->OPCodeCB0xC1(); };
    opcode_cb_functions[0xC2] = [&]() { this->OPCodeCB0xC2(); };
    opcode_cb_functions[0xC3] = [&]() { this->OPCodeCB0xC3(); };
    opcode_cb_functions[0xC4] = [&]() { this->OPCodeCB0xC4(); };
    opcode_cb_functions[0xC5] = [&]() { this->OPCodeCB0xC5(); };
    opcode_cb_functions[0xC6] = [&]() { this->OPCodeCB0xC6(); };
    opcode_cb_functions[0xC7] = [&]() { this->OPCodeCB0xC7(); };
    opcode_cb_functions[0xC8] = [&]() { this->OPCodeCB0xC8(); };
    opcode_cb_functions[0xC9] = [&]() { this->OPCodeCB0xC9(); };
    opcode_cb_functions[0xCA] = [&]() { this->OPCodeCB0xCA(); };
    opcode_cb_functions[0xCB] = [&]() { this->OPCodeCB0xCB(); };
    opcode_cb_functions[0xCC] = [&]() { this->OPCodeCB0xCC(); };
    opcode_cb_functions[0xCD] = [&]() { this->OPCodeCB0xCD(); };
    opcode_cb_functions[0xCE] = [&]() { this->OPCodeCB0xCE(); };
    opcode_cb_functions[0xCF] = [&]() { this->OPCodeCB0xCF(); };
    opcode_cb_functions[0xD0] = [&]() { this->OPCodeCB0xD0(); };
    opcode_cb_functions[0xD1] = [&]() { this->OPCodeCB0xD1(); };
    opcode_cb_functions[0xD2] = [&]() { this->OPCodeCB0xD2(); };
    opcode_cb_functions[0xD3] = [&]() { this->OPCodeCB0xD3(); };
    opcode_cb_functions[0xD4] = [&]() { this->OPCodeCB0xD4(); };
    opcode_cb_functions[0xD5] = [&]() { this->OPCodeCB0xD5(); };
    opcode_cb_functions[0xD6] = [&]() { this->OPCodeCB0xD6(); };
    opcode_cb_functions[0xD7] = [&]() { this->OPCodeCB0xD7(); };
    opcode_cb_functions[0xD8] = [&]() { this->OPCodeCB0xD8(); };
    opcode_cb_functions[0xD9] = [&]() { this->OPCodeCB0xD9(); };
    opcode_cb_functions[0xDA] = [&]() { this->OPCodeCB0xDA(); };
    opcode_cb_functions[0xDB] = [&]() { this->OPCodeCB0xDB(); };
    opcode_cb_functions[0xDC] = [&]() { this->OPCodeCB0xDC(); };
    opcode_cb_functions[0xDD] = [&]() { this->OPCodeCB0xDD(); };
    opcode_cb_functions[0xDE] = [&]() { this->OPCodeCB0xDE(); };
    opcode_cb_functions[0xDF] = [&]() { this->OPCodeCB0xDF(); };
    opcode_cb_functions[0xE0] = [&]() { this->OPCodeCB0xE0(); };
    opcode_cb_functions[0xE1] = [&]() { this->OPCodeCB0xE1(); };
    opcode_cb_functions[0xE2] = [&]() { this->OPCodeCB0xE2(); };
    opcode_cb_functions[0xE3] = [&]() { this->OPCodeCB0xE3(); };
    opcode_cb_functions[0xE4] = [&]() { this->OPCodeCB0xE4(); };
    opcode_cb_functions[0xE5] = [&]() { this->OPCodeCB0xE5(); };
    opcode_cb_functions[0xE6] = [&]() { this->OPCodeCB0xE6(); };
    opcode_cb_functions[0xE7] = [&]() { this->OPCodeCB0xE7(); };
    opcode_cb_functions[0xE8] = [&]() { this->OPCodeCB0xE8(); };
    opcode_cb_functions[0xE9] = [&]() { this->OPCodeCB0xE9(); };
    opcode_cb_functions[0xEA] = [&]() { this->OPCodeCB0xEA(); };
    opcode_cb_functions[0xEB] = [&]() { this->OPCodeCB0xEB(); };
    opcode_cb_functions[0xEC] = [&]() { this->OPCodeCB0xEC(); };
    opcode_cb_functions[0xED] = [&]() { this->OPCodeCB0xED(); };
    opcode_cb_functions[0xEE] = [&]() { this->OPCodeCB0xEE(); };
    opcode_cb_functions[0xEF] = [&]() { this->OPCodeCB0xEF(); };
    opcode_cb_functions[0xF0] = [&]() { this->OPCodeCB0xF0(); };
    opcode_cb_functions[0xF1] = [&]() { this->OPCodeCB0xF1(); };
    opcode_cb_functions[0xF2] = [&]() { this->OPCodeCB0xF2(); };
    opcode_cb_functions[0xF3] = [&]() { this->OPCodeCB0xF3(); };
    opcode_cb_functions[0xF4] = [&]() { this->OPCodeCB0xF4(); };
    opcode_cb_functions[0xF5] = [&]() { this->OPCodeCB0xF5(); };
    opcode_cb_functions[0xF6] = [&]() { this->OPCodeCB0xF6(); };
    opcode_cb_functions[0xF7] = [&]() { this->OPCodeCB0xF7(); };
    opcode_cb_functions[0xF8] = [&]() { this->OPCodeCB0xF8(); };
    opcode_cb_functions[0xF9] = [&]() { this->OPCodeCB0xF9(); };
    opcode_cb_functions[0xFA] = [&]() { this->OPCodeCB0xFA(); };
    opcode_cb_functions[0xFB] = [&]() { this->OPCodeCB0xFB(); };
    opcode_cb_functions[0xFC] = [&]() { this->OPCodeCB0xFC(); };
    opcode_cb_functions[0xFD] = [&]() { this->OPCodeCB0xFD(); };
    opcode_cb_functions[0xFE] = [&]() { this->OPCodeCB0xFE(); };
    opcode_cb_functions[0xFF] = [&]() { this->OPCodeCB0xFF(); };
}
