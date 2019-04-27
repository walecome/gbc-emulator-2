#include "InstructionDecoder.hh"

/**
    Assign the member pointers for registers etc to those from the processor.
*/
InstructionDecoder::InstructionDecoder(ptr<Processor> processor)
    : cpu { processor },
      program_memory { cpu->program_memory },
      ram { cpu->ram },
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
    } else {
        if (verbose) std::cout << "Executing regular instruction" << std::endl;
        executeInstruction(instruction);
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

byte_t InstructionDecoder::getCurrentData() {
    byte_t data = cpu->program_memory->getData(cpu->PC->getValue());
    cpu->PC->increment();

    return data;
}

void InstructionDecoder::loadRegister(const ptr<Register8bit> &reg) {
    reg->setValue(getCurrentData());
}

void InstructionDecoder::loadIntoMemory(const ptr<Register16bit> &address_reg,
                                        const ptr<Register8bit> &data_reg) {
    register16_t address = address_reg->getValue();

    byte_t data = data_reg->getValue();

    cpu->ram->setData(address, data);
}

void InstructionDecoder::loadIntoMemory(const ptr<Register16bit> &address_reg,
                                        byte_t value) {
    register16_t address = address_reg->getValue();

    cpu->ram->setData(address, value);
}

void InstructionDecoder::loadFromMemory(const ptr<Register8bit> &data_reg,
                                        const ptr<Register16bit> &address_reg) {
    register16_t address = address_reg->getValue();

    byte_t data = cpu->ram->getData(address);

    data_reg->setValue(data);
}

byte_t InstructionDecoder::loadFromMemory(
    const ptr<Register16bit> &address_reg) {
    register16_t address = address_reg->getValue();

    return cpu->ram->getData(address);
}

void InstructionDecoder::incrementRegister(const ptr<Register8bit> &reg) {
    reg->increment();

    register8_t result = reg->getValue();

    cpu->checkFlagZ(result);
    cpu->setFlagN(false);
    cpu->checkFlagH(result);
}

void InstructionDecoder::decrementRegister(const ptr<Register8bit> &reg) {
    reg->decrement();

    register8_t result = reg->getValue();

    cpu->checkFlagZ(result);
    cpu->setFlagN(true);
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
    cpu->setFlagN(false);
    cpu->checkFlagC(result);
    cpu->checkFlagH(value_dest);
}

void InstructionDecoder::addRegisters(const ptr<Register16bit> &destination,
                                      const ptr<Register16bit> &source) {
    int result = add_helper<Register16bit, register16_t>(destination, source);

    register16_t value_dest = (register16_t)result;

    cpu->setFlagN(false);
    cpu->checkFlagC(result);
    cpu->checkFlagH(value_dest);
}

void InstructionDecoder::subRegisters(const ptr<Register8bit> &source) {
    register8_t value_dest = cpu->A->getValue();
    register8_t value_source = source->getValue();

    value_dest -= value_source;

    cpu->A->setValue(value_dest);

    cpu->setFlagN(true);
}

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

void InstructionDecoder::subWithCarry(const ptr<Register8bit> &source) {
    register8_t value_dest = cpu->A->getValue();
    register8_t value_source = source->getValue();
    int carry = (cpu->getFlagC() ? 1 : 0);

    value_dest = value_dest - value_source - carry;

    cpu->A->setValue(value_dest);

    // TODO fix rest of flags. How does negative carry work???
    cpu->setFlagN(true);
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

    cpu->setFlagZ(value_a == value_source);

    // TODO half carry flag
    cpu->setFlagN(true);
    cpu->setFlagC(value_a < value_source);
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

void InstructionDecoder::popStackAF() {
    popStack(cpu->AF);
    register8_t f_data = cpu->F->getValue();

    cpu->setFlagZ(bool((0x80 & f_data) != 0));
    cpu->setFlagN(bool((0x40 & f_data) != 0));
    cpu->setFlagH(bool((0x20 & f_data) != 0));
    cpu->setFlagC(bool((0x10 & f_data) != 0));
}

void InstructionDecoder::performJump() {
    register16_t current_pc = cpu->PC->getValue();
    byte_t offset = cpu->program_memory->getData(current_pc);

    // TODO check this logic
    register16_t new_pc = (register16_t)((int16_t)current_pc + (int8_t)offset);

    cpu->PC->setValue(new_pc);
}

void InstructionDecoder::jumpIm16bit() {
    register8_t data_low = getCurrentData();
    register8_t data_high = getCurrentData();

    cpu->PC->getLowRegister()->setValue(data_low);
    cpu->PC->getHighRegister()->setValue(data_high);
}

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
    opcode_functions[0] = std::bind(&InstructionDecoder::OPCode0x00, this);
    opcode_functions[1] = std::bind(&InstructionDecoder::OPCode0x01, this);
    opcode_functions[2] = std::bind(&InstructionDecoder::OPCode0x02, this);
    opcode_functions[3] = std::bind(&InstructionDecoder::OPCode0x03, this);
    opcode_functions[4] = std::bind(&InstructionDecoder::OPCode0x04, this);
    opcode_functions[5] = std::bind(&InstructionDecoder::OPCode0x05, this);
    opcode_functions[6] = std::bind(&InstructionDecoder::OPCode0x06, this);
    opcode_functions[7] = std::bind(&InstructionDecoder::OPCode0x07, this);
    opcode_functions[8] = std::bind(&InstructionDecoder::OPCode0x08, this);
    opcode_functions[9] = std::bind(&InstructionDecoder::OPCode0x09, this);
    opcode_functions[10] = std::bind(&InstructionDecoder::OPCode0x0A, this);
    opcode_functions[11] = std::bind(&InstructionDecoder::OPCode0x0B, this);
    opcode_functions[12] = std::bind(&InstructionDecoder::OPCode0x0C, this);
    opcode_functions[13] = std::bind(&InstructionDecoder::OPCode0x0D, this);
    opcode_functions[14] = std::bind(&InstructionDecoder::OPCode0x0E, this);
    opcode_functions[15] = std::bind(&InstructionDecoder::OPCode0x0F, this);
    opcode_functions[16] = std::bind(&InstructionDecoder::OPCode0x10, this);
    opcode_functions[17] = std::bind(&InstructionDecoder::OPCode0x11, this);
    opcode_functions[18] = std::bind(&InstructionDecoder::OPCode0x12, this);
    opcode_functions[19] = std::bind(&InstructionDecoder::OPCode0x13, this);
    opcode_functions[20] = std::bind(&InstructionDecoder::OPCode0x14, this);
    opcode_functions[21] = std::bind(&InstructionDecoder::OPCode0x15, this);
    opcode_functions[22] = std::bind(&InstructionDecoder::OPCode0x16, this);
    opcode_functions[23] = std::bind(&InstructionDecoder::OPCode0x17, this);
    opcode_functions[24] = std::bind(&InstructionDecoder::OPCode0x18, this);
    opcode_functions[25] = std::bind(&InstructionDecoder::OPCode0x19, this);
    opcode_functions[26] = std::bind(&InstructionDecoder::OPCode0x1A, this);
    opcode_functions[27] = std::bind(&InstructionDecoder::OPCode0x1B, this);
    opcode_functions[28] = std::bind(&InstructionDecoder::OPCode0x1C, this);
    opcode_functions[29] = std::bind(&InstructionDecoder::OPCode0x1D, this);
    opcode_functions[30] = std::bind(&InstructionDecoder::OPCode0x1E, this);
    opcode_functions[31] = std::bind(&InstructionDecoder::OPCode0x1F, this);
    opcode_functions[32] = std::bind(&InstructionDecoder::OPCode0x20, this);
    opcode_functions[33] = std::bind(&InstructionDecoder::OPCode0x21, this);
    opcode_functions[34] = std::bind(&InstructionDecoder::OPCode0x22, this);
    opcode_functions[35] = std::bind(&InstructionDecoder::OPCode0x23, this);
    opcode_functions[36] = std::bind(&InstructionDecoder::OPCode0x24, this);
    opcode_functions[37] = std::bind(&InstructionDecoder::OPCode0x25, this);
    opcode_functions[38] = std::bind(&InstructionDecoder::OPCode0x26, this);
    opcode_functions[39] = std::bind(&InstructionDecoder::OPCode0x27, this);
    opcode_functions[40] = std::bind(&InstructionDecoder::OPCode0x28, this);
    opcode_functions[41] = std::bind(&InstructionDecoder::OPCode0x29, this);
    opcode_functions[42] = std::bind(&InstructionDecoder::OPCode0x2A, this);
    opcode_functions[43] = std::bind(&InstructionDecoder::OPCode0x2B, this);
    opcode_functions[44] = std::bind(&InstructionDecoder::OPCode0x2C, this);
    opcode_functions[45] = std::bind(&InstructionDecoder::OPCode0x2D, this);
    opcode_functions[46] = std::bind(&InstructionDecoder::OPCode0x2E, this);
    opcode_functions[47] = std::bind(&InstructionDecoder::OPCode0x2F, this);
    opcode_functions[48] = std::bind(&InstructionDecoder::OPCode0x30, this);
    opcode_functions[49] = std::bind(&InstructionDecoder::OPCode0x31, this);
    opcode_functions[50] = std::bind(&InstructionDecoder::OPCode0x32, this);
    opcode_functions[51] = std::bind(&InstructionDecoder::OPCode0x33, this);
    opcode_functions[52] = std::bind(&InstructionDecoder::OPCode0x34, this);
    opcode_functions[53] = std::bind(&InstructionDecoder::OPCode0x35, this);
    opcode_functions[54] = std::bind(&InstructionDecoder::OPCode0x36, this);
    opcode_functions[55] = std::bind(&InstructionDecoder::OPCode0x37, this);
    opcode_functions[56] = std::bind(&InstructionDecoder::OPCode0x38, this);
    opcode_functions[57] = std::bind(&InstructionDecoder::OPCode0x39, this);
    opcode_functions[58] = std::bind(&InstructionDecoder::OPCode0x3A, this);
    opcode_functions[59] = std::bind(&InstructionDecoder::OPCode0x3B, this);
    opcode_functions[60] = std::bind(&InstructionDecoder::OPCode0x3C, this);
    opcode_functions[61] = std::bind(&InstructionDecoder::OPCode0x3D, this);
    opcode_functions[62] = std::bind(&InstructionDecoder::OPCode0x3E, this);
    opcode_functions[63] = std::bind(&InstructionDecoder::OPCode0x3F, this);
    opcode_functions[64] = std::bind(&InstructionDecoder::OPCode0x40, this);
    opcode_functions[65] = std::bind(&InstructionDecoder::OPCode0x41, this);
    opcode_functions[66] = std::bind(&InstructionDecoder::OPCode0x42, this);
    opcode_functions[67] = std::bind(&InstructionDecoder::OPCode0x43, this);
    opcode_functions[68] = std::bind(&InstructionDecoder::OPCode0x44, this);
    opcode_functions[69] = std::bind(&InstructionDecoder::OPCode0x45, this);
    opcode_functions[70] = std::bind(&InstructionDecoder::OPCode0x46, this);
    opcode_functions[71] = std::bind(&InstructionDecoder::OPCode0x47, this);
    opcode_functions[72] = std::bind(&InstructionDecoder::OPCode0x48, this);
    opcode_functions[73] = std::bind(&InstructionDecoder::OPCode0x49, this);
    opcode_functions[74] = std::bind(&InstructionDecoder::OPCode0x4A, this);
    opcode_functions[75] = std::bind(&InstructionDecoder::OPCode0x4B, this);
    opcode_functions[76] = std::bind(&InstructionDecoder::OPCode0x4C, this);
    opcode_functions[77] = std::bind(&InstructionDecoder::OPCode0x4D, this);
    opcode_functions[78] = std::bind(&InstructionDecoder::OPCode0x4E, this);
    opcode_functions[79] = std::bind(&InstructionDecoder::OPCode0x4F, this);
    opcode_functions[80] = std::bind(&InstructionDecoder::OPCode0x50, this);
    opcode_functions[81] = std::bind(&InstructionDecoder::OPCode0x51, this);
    opcode_functions[82] = std::bind(&InstructionDecoder::OPCode0x52, this);
    opcode_functions[83] = std::bind(&InstructionDecoder::OPCode0x53, this);
    opcode_functions[84] = std::bind(&InstructionDecoder::OPCode0x54, this);
    opcode_functions[85] = std::bind(&InstructionDecoder::OPCode0x55, this);
    opcode_functions[86] = std::bind(&InstructionDecoder::OPCode0x56, this);
    opcode_functions[87] = std::bind(&InstructionDecoder::OPCode0x57, this);
    opcode_functions[88] = std::bind(&InstructionDecoder::OPCode0x58, this);
    opcode_functions[89] = std::bind(&InstructionDecoder::OPCode0x59, this);
    opcode_functions[90] = std::bind(&InstructionDecoder::OPCode0x5A, this);
    opcode_functions[91] = std::bind(&InstructionDecoder::OPCode0x5B, this);
    opcode_functions[92] = std::bind(&InstructionDecoder::OPCode0x5C, this);
    opcode_functions[93] = std::bind(&InstructionDecoder::OPCode0x5D, this);
    opcode_functions[94] = std::bind(&InstructionDecoder::OPCode0x5E, this);
    opcode_functions[95] = std::bind(&InstructionDecoder::OPCode0x5F, this);
    opcode_functions[96] = std::bind(&InstructionDecoder::OPCode0x60, this);
    opcode_functions[97] = std::bind(&InstructionDecoder::OPCode0x61, this);
    opcode_functions[98] = std::bind(&InstructionDecoder::OPCode0x62, this);
    opcode_functions[99] = std::bind(&InstructionDecoder::OPCode0x63, this);
    opcode_functions[100] = std::bind(&InstructionDecoder::OPCode0x64, this);
    opcode_functions[101] = std::bind(&InstructionDecoder::OPCode0x65, this);
    opcode_functions[102] = std::bind(&InstructionDecoder::OPCode0x66, this);
    opcode_functions[103] = std::bind(&InstructionDecoder::OPCode0x67, this);
    opcode_functions[104] = std::bind(&InstructionDecoder::OPCode0x68, this);
    opcode_functions[105] = std::bind(&InstructionDecoder::OPCode0x69, this);
    opcode_functions[106] = std::bind(&InstructionDecoder::OPCode0x6A, this);
    opcode_functions[107] = std::bind(&InstructionDecoder::OPCode0x6B, this);
    opcode_functions[108] = std::bind(&InstructionDecoder::OPCode0x6C, this);
    opcode_functions[109] = std::bind(&InstructionDecoder::OPCode0x6D, this);
    opcode_functions[110] = std::bind(&InstructionDecoder::OPCode0x6E, this);
    opcode_functions[111] = std::bind(&InstructionDecoder::OPCode0x6F, this);
    opcode_functions[112] = std::bind(&InstructionDecoder::OPCode0x70, this);
    opcode_functions[113] = std::bind(&InstructionDecoder::OPCode0x71, this);
    opcode_functions[114] = std::bind(&InstructionDecoder::OPCode0x72, this);
    opcode_functions[115] = std::bind(&InstructionDecoder::OPCode0x73, this);
    opcode_functions[116] = std::bind(&InstructionDecoder::OPCode0x74, this);
    opcode_functions[117] = std::bind(&InstructionDecoder::OPCode0x75, this);
    opcode_functions[118] = std::bind(&InstructionDecoder::OPCode0x76, this);
    opcode_functions[119] = std::bind(&InstructionDecoder::OPCode0x77, this);
    opcode_functions[120] = std::bind(&InstructionDecoder::OPCode0x78, this);
    opcode_functions[121] = std::bind(&InstructionDecoder::OPCode0x79, this);
    opcode_functions[122] = std::bind(&InstructionDecoder::OPCode0x7A, this);
    opcode_functions[123] = std::bind(&InstructionDecoder::OPCode0x7B, this);
    opcode_functions[124] = std::bind(&InstructionDecoder::OPCode0x7C, this);
    opcode_functions[125] = std::bind(&InstructionDecoder::OPCode0x7D, this);
    opcode_functions[126] = std::bind(&InstructionDecoder::OPCode0x7E, this);
    opcode_functions[127] = std::bind(&InstructionDecoder::OPCode0x7F, this);
    opcode_functions[128] = std::bind(&InstructionDecoder::OPCode0x80, this);
    opcode_functions[129] = std::bind(&InstructionDecoder::OPCode0x81, this);
    opcode_functions[130] = std::bind(&InstructionDecoder::OPCode0x82, this);
    opcode_functions[131] = std::bind(&InstructionDecoder::OPCode0x83, this);
    opcode_functions[132] = std::bind(&InstructionDecoder::OPCode0x84, this);
    opcode_functions[133] = std::bind(&InstructionDecoder::OPCode0x85, this);
    opcode_functions[134] = std::bind(&InstructionDecoder::OPCode0x86, this);
    opcode_functions[135] = std::bind(&InstructionDecoder::OPCode0x87, this);
    opcode_functions[136] = std::bind(&InstructionDecoder::OPCode0x88, this);
    opcode_functions[137] = std::bind(&InstructionDecoder::OPCode0x89, this);
    opcode_functions[138] = std::bind(&InstructionDecoder::OPCode0x8A, this);
    opcode_functions[139] = std::bind(&InstructionDecoder::OPCode0x8B, this);
    opcode_functions[140] = std::bind(&InstructionDecoder::OPCode0x8C, this);
    opcode_functions[141] = std::bind(&InstructionDecoder::OPCode0x8D, this);
    opcode_functions[142] = std::bind(&InstructionDecoder::OPCode0x8E, this);
    opcode_functions[143] = std::bind(&InstructionDecoder::OPCode0x8F, this);
    opcode_functions[144] = std::bind(&InstructionDecoder::OPCode0x90, this);
    opcode_functions[145] = std::bind(&InstructionDecoder::OPCode0x91, this);
    opcode_functions[146] = std::bind(&InstructionDecoder::OPCode0x92, this);
    opcode_functions[147] = std::bind(&InstructionDecoder::OPCode0x93, this);
    opcode_functions[148] = std::bind(&InstructionDecoder::OPCode0x94, this);
    opcode_functions[149] = std::bind(&InstructionDecoder::OPCode0x95, this);
    opcode_functions[150] = std::bind(&InstructionDecoder::OPCode0x96, this);
    opcode_functions[151] = std::bind(&InstructionDecoder::OPCode0x97, this);
    opcode_functions[152] = std::bind(&InstructionDecoder::OPCode0x98, this);
    opcode_functions[153] = std::bind(&InstructionDecoder::OPCode0x99, this);
    opcode_functions[154] = std::bind(&InstructionDecoder::OPCode0x9A, this);
    opcode_functions[155] = std::bind(&InstructionDecoder::OPCode0x9B, this);
    opcode_functions[156] = std::bind(&InstructionDecoder::OPCode0x9C, this);
    opcode_functions[157] = std::bind(&InstructionDecoder::OPCode0x9D, this);
    opcode_functions[158] = std::bind(&InstructionDecoder::OPCode0x9E, this);
    opcode_functions[159] = std::bind(&InstructionDecoder::OPCode0x9F, this);
    opcode_functions[160] = std::bind(&InstructionDecoder::OPCode0xA0, this);
    opcode_functions[161] = std::bind(&InstructionDecoder::OPCode0xA1, this);
    opcode_functions[162] = std::bind(&InstructionDecoder::OPCode0xA2, this);
    opcode_functions[163] = std::bind(&InstructionDecoder::OPCode0xA3, this);
    opcode_functions[164] = std::bind(&InstructionDecoder::OPCode0xA4, this);
    opcode_functions[165] = std::bind(&InstructionDecoder::OPCode0xA5, this);
    opcode_functions[166] = std::bind(&InstructionDecoder::OPCode0xA6, this);
    opcode_functions[167] = std::bind(&InstructionDecoder::OPCode0xA7, this);
    opcode_functions[168] = std::bind(&InstructionDecoder::OPCode0xA8, this);
    opcode_functions[169] = std::bind(&InstructionDecoder::OPCode0xA9, this);
    opcode_functions[170] = std::bind(&InstructionDecoder::OPCode0xAA, this);
    opcode_functions[171] = std::bind(&InstructionDecoder::OPCode0xAB, this);
    opcode_functions[172] = std::bind(&InstructionDecoder::OPCode0xAC, this);
    opcode_functions[173] = std::bind(&InstructionDecoder::OPCode0xAD, this);
    opcode_functions[174] = std::bind(&InstructionDecoder::OPCode0xAE, this);
    opcode_functions[175] = std::bind(&InstructionDecoder::OPCode0xAF, this);
    opcode_functions[176] = std::bind(&InstructionDecoder::OPCode0xB0, this);
    opcode_functions[177] = std::bind(&InstructionDecoder::OPCode0xB1, this);
    opcode_functions[178] = std::bind(&InstructionDecoder::OPCode0xB2, this);
    opcode_functions[179] = std::bind(&InstructionDecoder::OPCode0xB3, this);
    opcode_functions[180] = std::bind(&InstructionDecoder::OPCode0xB4, this);
    opcode_functions[181] = std::bind(&InstructionDecoder::OPCode0xB5, this);
    opcode_functions[182] = std::bind(&InstructionDecoder::OPCode0xB6, this);
    opcode_functions[183] = std::bind(&InstructionDecoder::OPCode0xB7, this);
    opcode_functions[184] = std::bind(&InstructionDecoder::OPCode0xB8, this);
    opcode_functions[185] = std::bind(&InstructionDecoder::OPCode0xB9, this);
    opcode_functions[186] = std::bind(&InstructionDecoder::OPCode0xBA, this);
    opcode_functions[187] = std::bind(&InstructionDecoder::OPCode0xBB, this);
    opcode_functions[188] = std::bind(&InstructionDecoder::OPCode0xBC, this);
    opcode_functions[189] = std::bind(&InstructionDecoder::OPCode0xBD, this);
    opcode_functions[190] = std::bind(&InstructionDecoder::OPCode0xBE, this);
    opcode_functions[191] = std::bind(&InstructionDecoder::OPCode0xBF, this);
    opcode_functions[192] = std::bind(&InstructionDecoder::OPCode0xC0, this);
    opcode_functions[193] = std::bind(&InstructionDecoder::OPCode0xC1, this);
    opcode_functions[194] = std::bind(&InstructionDecoder::OPCode0xC2, this);
    opcode_functions[195] = std::bind(&InstructionDecoder::OPCode0xC3, this);
    opcode_functions[196] = std::bind(&InstructionDecoder::OPCode0xC4, this);
    opcode_functions[197] = std::bind(&InstructionDecoder::OPCode0xC5, this);
    opcode_functions[198] = std::bind(&InstructionDecoder::OPCode0xC6, this);
    opcode_functions[199] = std::bind(&InstructionDecoder::OPCode0xC7, this);
    opcode_functions[200] = std::bind(&InstructionDecoder::OPCode0xC8, this);
    opcode_functions[201] = std::bind(&InstructionDecoder::OPCode0xC9, this);
    opcode_functions[202] = std::bind(&InstructionDecoder::OPCode0xCA, this);
    opcode_functions[203] = std::bind(&InstructionDecoder::OPCode0xCB, this);
    opcode_functions[204] = std::bind(&InstructionDecoder::OPCode0xCC, this);
    opcode_functions[205] = std::bind(&InstructionDecoder::OPCode0xCD, this);
    opcode_functions[206] = std::bind(&InstructionDecoder::OPCode0xCE, this);
    opcode_functions[207] = std::bind(&InstructionDecoder::OPCode0xCF, this);
    opcode_functions[208] = std::bind(&InstructionDecoder::OPCode0xD0, this);
    opcode_functions[209] = std::bind(&InstructionDecoder::OPCode0xD1, this);
    opcode_functions[210] = std::bind(&InstructionDecoder::OPCode0xD2, this);
    opcode_functions[211] = std::bind(&InstructionDecoder::OPCode0xD3, this);
    opcode_functions[212] = std::bind(&InstructionDecoder::OPCode0xD4, this);
    opcode_functions[213] = std::bind(&InstructionDecoder::OPCode0xD5, this);
    opcode_functions[214] = std::bind(&InstructionDecoder::OPCode0xD6, this);
    opcode_functions[215] = std::bind(&InstructionDecoder::OPCode0xD7, this);
    opcode_functions[216] = std::bind(&InstructionDecoder::OPCode0xD8, this);
    opcode_functions[217] = std::bind(&InstructionDecoder::OPCode0xD9, this);
    opcode_functions[218] = std::bind(&InstructionDecoder::OPCode0xDA, this);
    opcode_functions[219] = std::bind(&InstructionDecoder::OPCode0xDB, this);
    opcode_functions[220] = std::bind(&InstructionDecoder::OPCode0xDC, this);
    opcode_functions[221] = std::bind(&InstructionDecoder::OPCode0xDD, this);
    opcode_functions[222] = std::bind(&InstructionDecoder::OPCode0xDE, this);
    opcode_functions[223] = std::bind(&InstructionDecoder::OPCode0xDF, this);
    opcode_functions[224] = std::bind(&InstructionDecoder::OPCode0xE0, this);
    opcode_functions[225] = std::bind(&InstructionDecoder::OPCode0xE1, this);
    opcode_functions[226] = std::bind(&InstructionDecoder::OPCode0xE2, this);
    opcode_functions[227] = std::bind(&InstructionDecoder::OPCode0xE3, this);
    opcode_functions[228] = std::bind(&InstructionDecoder::OPCode0xE4, this);
    opcode_functions[229] = std::bind(&InstructionDecoder::OPCode0xE5, this);
    opcode_functions[230] = std::bind(&InstructionDecoder::OPCode0xE6, this);
    opcode_functions[231] = std::bind(&InstructionDecoder::OPCode0xE7, this);
    opcode_functions[232] = std::bind(&InstructionDecoder::OPCode0xE8, this);
    opcode_functions[233] = std::bind(&InstructionDecoder::OPCode0xE9, this);
    opcode_functions[234] = std::bind(&InstructionDecoder::OPCode0xEA, this);
    opcode_functions[235] = std::bind(&InstructionDecoder::OPCode0xEB, this);
    opcode_functions[236] = std::bind(&InstructionDecoder::OPCode0xEC, this);
    opcode_functions[237] = std::bind(&InstructionDecoder::OPCode0xED, this);
    opcode_functions[238] = std::bind(&InstructionDecoder::OPCode0xEE, this);
    opcode_functions[239] = std::bind(&InstructionDecoder::OPCode0xEF, this);
    opcode_functions[240] = std::bind(&InstructionDecoder::OPCode0xF0, this);
    opcode_functions[241] = std::bind(&InstructionDecoder::OPCode0xF1, this);
    opcode_functions[242] = std::bind(&InstructionDecoder::OPCode0xF2, this);
    opcode_functions[243] = std::bind(&InstructionDecoder::OPCode0xF3, this);
    opcode_functions[244] = std::bind(&InstructionDecoder::OPCode0xF4, this);
    opcode_functions[245] = std::bind(&InstructionDecoder::OPCode0xF5, this);
    opcode_functions[246] = std::bind(&InstructionDecoder::OPCode0xF6, this);
    opcode_functions[247] = std::bind(&InstructionDecoder::OPCode0xF7, this);
    opcode_functions[248] = std::bind(&InstructionDecoder::OPCode0xF8, this);
    opcode_functions[249] = std::bind(&InstructionDecoder::OPCode0xF9, this);
    opcode_functions[250] = std::bind(&InstructionDecoder::OPCode0xFA, this);
    opcode_functions[251] = std::bind(&InstructionDecoder::OPCode0xFB, this);
    opcode_functions[252] = std::bind(&InstructionDecoder::OPCode0xFC, this);
    opcode_functions[253] = std::bind(&InstructionDecoder::OPCode0xFD, this);
    opcode_functions[254] = std::bind(&InstructionDecoder::OPCode0xFE, this);
    opcode_functions[255] = std::bind(&InstructionDecoder::OPCode0xFF, this);
}

void InstructionDecoder::map_cb_opcodes() {
    opcode_cb_functions[0] = std::bind(&InstructionDecoder::OPCodeCB0x00, this);
    opcode_cb_functions[1] = std::bind(&InstructionDecoder::OPCodeCB0x01, this);
    opcode_cb_functions[2] = std::bind(&InstructionDecoder::OPCodeCB0x02, this);
    opcode_cb_functions[3] = std::bind(&InstructionDecoder::OPCodeCB0x03, this);
    opcode_cb_functions[4] = std::bind(&InstructionDecoder::OPCodeCB0x04, this);
    opcode_cb_functions[5] = std::bind(&InstructionDecoder::OPCodeCB0x05, this);
    opcode_cb_functions[6] = std::bind(&InstructionDecoder::OPCodeCB0x06, this);
    opcode_cb_functions[7] = std::bind(&InstructionDecoder::OPCodeCB0x07, this);
    opcode_cb_functions[8] = std::bind(&InstructionDecoder::OPCodeCB0x08, this);
    opcode_cb_functions[9] = std::bind(&InstructionDecoder::OPCodeCB0x09, this);
    opcode_cb_functions[10] =
        std::bind(&InstructionDecoder::OPCodeCB0x0A, this);
    opcode_cb_functions[11] =
        std::bind(&InstructionDecoder::OPCodeCB0x0B, this);
    opcode_cb_functions[12] =
        std::bind(&InstructionDecoder::OPCodeCB0x0C, this);
    opcode_cb_functions[13] =
        std::bind(&InstructionDecoder::OPCodeCB0x0D, this);
    opcode_cb_functions[14] =
        std::bind(&InstructionDecoder::OPCodeCB0x0E, this);
    opcode_cb_functions[15] =
        std::bind(&InstructionDecoder::OPCodeCB0x0F, this);
    opcode_cb_functions[16] =
        std::bind(&InstructionDecoder::OPCodeCB0x10, this);
    opcode_cb_functions[17] =
        std::bind(&InstructionDecoder::OPCodeCB0x11, this);
    opcode_cb_functions[18] =
        std::bind(&InstructionDecoder::OPCodeCB0x12, this);
    opcode_cb_functions[19] =
        std::bind(&InstructionDecoder::OPCodeCB0x13, this);
    opcode_cb_functions[20] =
        std::bind(&InstructionDecoder::OPCodeCB0x14, this);
    opcode_cb_functions[21] =
        std::bind(&InstructionDecoder::OPCodeCB0x15, this);
    opcode_cb_functions[22] =
        std::bind(&InstructionDecoder::OPCodeCB0x16, this);
    opcode_cb_functions[23] =
        std::bind(&InstructionDecoder::OPCodeCB0x17, this);
    opcode_cb_functions[24] =
        std::bind(&InstructionDecoder::OPCodeCB0x18, this);
    opcode_cb_functions[25] =
        std::bind(&InstructionDecoder::OPCodeCB0x19, this);
    opcode_cb_functions[26] =
        std::bind(&InstructionDecoder::OPCodeCB0x1A, this);
    opcode_cb_functions[27] =
        std::bind(&InstructionDecoder::OPCodeCB0x1B, this);
    opcode_cb_functions[28] =
        std::bind(&InstructionDecoder::OPCodeCB0x1C, this);
    opcode_cb_functions[29] =
        std::bind(&InstructionDecoder::OPCodeCB0x1D, this);
    opcode_cb_functions[30] =
        std::bind(&InstructionDecoder::OPCodeCB0x1E, this);
    opcode_cb_functions[31] =
        std::bind(&InstructionDecoder::OPCodeCB0x1F, this);
    opcode_cb_functions[32] =
        std::bind(&InstructionDecoder::OPCodeCB0x20, this);
    opcode_cb_functions[33] =
        std::bind(&InstructionDecoder::OPCodeCB0x21, this);
    opcode_cb_functions[34] =
        std::bind(&InstructionDecoder::OPCodeCB0x22, this);
    opcode_cb_functions[35] =
        std::bind(&InstructionDecoder::OPCodeCB0x23, this);
    opcode_cb_functions[36] =
        std::bind(&InstructionDecoder::OPCodeCB0x24, this);
    opcode_cb_functions[37] =
        std::bind(&InstructionDecoder::OPCodeCB0x25, this);
    opcode_cb_functions[38] =
        std::bind(&InstructionDecoder::OPCodeCB0x26, this);
    opcode_cb_functions[39] =
        std::bind(&InstructionDecoder::OPCodeCB0x27, this);
    opcode_cb_functions[40] =
        std::bind(&InstructionDecoder::OPCodeCB0x28, this);
    opcode_cb_functions[41] =
        std::bind(&InstructionDecoder::OPCodeCB0x29, this);
    opcode_cb_functions[42] =
        std::bind(&InstructionDecoder::OPCodeCB0x2A, this);
    opcode_cb_functions[43] =
        std::bind(&InstructionDecoder::OPCodeCB0x2B, this);
    opcode_cb_functions[44] =
        std::bind(&InstructionDecoder::OPCodeCB0x2C, this);
    opcode_cb_functions[45] =
        std::bind(&InstructionDecoder::OPCodeCB0x2D, this);
    opcode_cb_functions[46] =
        std::bind(&InstructionDecoder::OPCodeCB0x2E, this);
    opcode_cb_functions[47] =
        std::bind(&InstructionDecoder::OPCodeCB0x2F, this);
    opcode_cb_functions[48] =
        std::bind(&InstructionDecoder::OPCodeCB0x30, this);
    opcode_cb_functions[49] =
        std::bind(&InstructionDecoder::OPCodeCB0x31, this);
    opcode_cb_functions[50] =
        std::bind(&InstructionDecoder::OPCodeCB0x32, this);
    opcode_cb_functions[51] =
        std::bind(&InstructionDecoder::OPCodeCB0x33, this);
    opcode_cb_functions[52] =
        std::bind(&InstructionDecoder::OPCodeCB0x34, this);
    opcode_cb_functions[53] =
        std::bind(&InstructionDecoder::OPCodeCB0x35, this);
    opcode_cb_functions[54] =
        std::bind(&InstructionDecoder::OPCodeCB0x36, this);
    opcode_cb_functions[55] =
        std::bind(&InstructionDecoder::OPCodeCB0x37, this);
    opcode_cb_functions[56] =
        std::bind(&InstructionDecoder::OPCodeCB0x38, this);
    opcode_cb_functions[57] =
        std::bind(&InstructionDecoder::OPCodeCB0x39, this);
    opcode_cb_functions[58] =
        std::bind(&InstructionDecoder::OPCodeCB0x3A, this);
    opcode_cb_functions[59] =
        std::bind(&InstructionDecoder::OPCodeCB0x3B, this);
    opcode_cb_functions[60] =
        std::bind(&InstructionDecoder::OPCodeCB0x3C, this);
    opcode_cb_functions[61] =
        std::bind(&InstructionDecoder::OPCodeCB0x3D, this);
    opcode_cb_functions[62] =
        std::bind(&InstructionDecoder::OPCodeCB0x3E, this);
    opcode_cb_functions[63] =
        std::bind(&InstructionDecoder::OPCodeCB0x3F, this);
    opcode_cb_functions[64] =
        std::bind(&InstructionDecoder::OPCodeCB0x40, this);
    opcode_cb_functions[65] =
        std::bind(&InstructionDecoder::OPCodeCB0x41, this);
    opcode_cb_functions[66] =
        std::bind(&InstructionDecoder::OPCodeCB0x42, this);
    opcode_cb_functions[67] =
        std::bind(&InstructionDecoder::OPCodeCB0x43, this);
    opcode_cb_functions[68] =
        std::bind(&InstructionDecoder::OPCodeCB0x44, this);
    opcode_cb_functions[69] =
        std::bind(&InstructionDecoder::OPCodeCB0x45, this);
    opcode_cb_functions[70] =
        std::bind(&InstructionDecoder::OPCodeCB0x46, this);
    opcode_cb_functions[71] =
        std::bind(&InstructionDecoder::OPCodeCB0x47, this);
    opcode_cb_functions[72] =
        std::bind(&InstructionDecoder::OPCodeCB0x48, this);
    opcode_cb_functions[73] =
        std::bind(&InstructionDecoder::OPCodeCB0x49, this);
    opcode_cb_functions[74] =
        std::bind(&InstructionDecoder::OPCodeCB0x4A, this);
    opcode_cb_functions[75] =
        std::bind(&InstructionDecoder::OPCodeCB0x4B, this);
    opcode_cb_functions[76] =
        std::bind(&InstructionDecoder::OPCodeCB0x4C, this);
    opcode_cb_functions[77] =
        std::bind(&InstructionDecoder::OPCodeCB0x4D, this);
    opcode_cb_functions[78] =
        std::bind(&InstructionDecoder::OPCodeCB0x4E, this);
    opcode_cb_functions[79] =
        std::bind(&InstructionDecoder::OPCodeCB0x4F, this);
    opcode_cb_functions[80] =
        std::bind(&InstructionDecoder::OPCodeCB0x50, this);
    opcode_cb_functions[81] =
        std::bind(&InstructionDecoder::OPCodeCB0x51, this);
    opcode_cb_functions[82] =
        std::bind(&InstructionDecoder::OPCodeCB0x52, this);
    opcode_cb_functions[83] =
        std::bind(&InstructionDecoder::OPCodeCB0x53, this);
    opcode_cb_functions[84] =
        std::bind(&InstructionDecoder::OPCodeCB0x54, this);
    opcode_cb_functions[85] =
        std::bind(&InstructionDecoder::OPCodeCB0x55, this);
    opcode_cb_functions[86] =
        std::bind(&InstructionDecoder::OPCodeCB0x56, this);
    opcode_cb_functions[87] =
        std::bind(&InstructionDecoder::OPCodeCB0x57, this);
    opcode_cb_functions[88] =
        std::bind(&InstructionDecoder::OPCodeCB0x58, this);
    opcode_cb_functions[89] =
        std::bind(&InstructionDecoder::OPCodeCB0x59, this);
    opcode_cb_functions[90] =
        std::bind(&InstructionDecoder::OPCodeCB0x5A, this);
    opcode_cb_functions[91] =
        std::bind(&InstructionDecoder::OPCodeCB0x5B, this);
    opcode_cb_functions[92] =
        std::bind(&InstructionDecoder::OPCodeCB0x5C, this);
    opcode_cb_functions[93] =
        std::bind(&InstructionDecoder::OPCodeCB0x5D, this);
    opcode_cb_functions[94] =
        std::bind(&InstructionDecoder::OPCodeCB0x5E, this);
    opcode_cb_functions[95] =
        std::bind(&InstructionDecoder::OPCodeCB0x5F, this);
    opcode_cb_functions[96] =
        std::bind(&InstructionDecoder::OPCodeCB0x60, this);
    opcode_cb_functions[97] =
        std::bind(&InstructionDecoder::OPCodeCB0x61, this);
    opcode_cb_functions[98] =
        std::bind(&InstructionDecoder::OPCodeCB0x62, this);
    opcode_cb_functions[99] =
        std::bind(&InstructionDecoder::OPCodeCB0x63, this);
    opcode_cb_functions[100] =
        std::bind(&InstructionDecoder::OPCodeCB0x64, this);
    opcode_cb_functions[101] =
        std::bind(&InstructionDecoder::OPCodeCB0x65, this);
    opcode_cb_functions[102] =
        std::bind(&InstructionDecoder::OPCodeCB0x66, this);
    opcode_cb_functions[103] =
        std::bind(&InstructionDecoder::OPCodeCB0x67, this);
    opcode_cb_functions[104] =
        std::bind(&InstructionDecoder::OPCodeCB0x68, this);
    opcode_cb_functions[105] =
        std::bind(&InstructionDecoder::OPCodeCB0x69, this);
    opcode_cb_functions[106] =
        std::bind(&InstructionDecoder::OPCodeCB0x6A, this);
    opcode_cb_functions[107] =
        std::bind(&InstructionDecoder::OPCodeCB0x6B, this);
    opcode_cb_functions[108] =
        std::bind(&InstructionDecoder::OPCodeCB0x6C, this);
    opcode_cb_functions[109] =
        std::bind(&InstructionDecoder::OPCodeCB0x6D, this);
    opcode_cb_functions[110] =
        std::bind(&InstructionDecoder::OPCodeCB0x6E, this);
    opcode_cb_functions[111] =
        std::bind(&InstructionDecoder::OPCodeCB0x6F, this);
    opcode_cb_functions[112] =
        std::bind(&InstructionDecoder::OPCodeCB0x70, this);
    opcode_cb_functions[113] =
        std::bind(&InstructionDecoder::OPCodeCB0x71, this);
    opcode_cb_functions[114] =
        std::bind(&InstructionDecoder::OPCodeCB0x72, this);
    opcode_cb_functions[115] =
        std::bind(&InstructionDecoder::OPCodeCB0x73, this);
    opcode_cb_functions[116] =
        std::bind(&InstructionDecoder::OPCodeCB0x74, this);
    opcode_cb_functions[117] =
        std::bind(&InstructionDecoder::OPCodeCB0x75, this);
    opcode_cb_functions[118] =
        std::bind(&InstructionDecoder::OPCodeCB0x76, this);
    opcode_cb_functions[119] =
        std::bind(&InstructionDecoder::OPCodeCB0x77, this);
    opcode_cb_functions[120] =
        std::bind(&InstructionDecoder::OPCodeCB0x78, this);
    opcode_cb_functions[121] =
        std::bind(&InstructionDecoder::OPCodeCB0x79, this);
    opcode_cb_functions[122] =
        std::bind(&InstructionDecoder::OPCodeCB0x7A, this);
    opcode_cb_functions[123] =
        std::bind(&InstructionDecoder::OPCodeCB0x7B, this);
    opcode_cb_functions[124] =
        std::bind(&InstructionDecoder::OPCodeCB0x7C, this);
    opcode_cb_functions[125] =
        std::bind(&InstructionDecoder::OPCodeCB0x7D, this);
    opcode_cb_functions[126] =
        std::bind(&InstructionDecoder::OPCodeCB0x7E, this);
    opcode_cb_functions[127] =
        std::bind(&InstructionDecoder::OPCodeCB0x7F, this);
    opcode_cb_functions[128] =
        std::bind(&InstructionDecoder::OPCodeCB0x80, this);
    opcode_cb_functions[129] =
        std::bind(&InstructionDecoder::OPCodeCB0x81, this);
    opcode_cb_functions[130] =
        std::bind(&InstructionDecoder::OPCodeCB0x82, this);
    opcode_cb_functions[131] =
        std::bind(&InstructionDecoder::OPCodeCB0x83, this);
    opcode_cb_functions[132] =
        std::bind(&InstructionDecoder::OPCodeCB0x84, this);
    opcode_cb_functions[133] =
        std::bind(&InstructionDecoder::OPCodeCB0x85, this);
    opcode_cb_functions[134] =
        std::bind(&InstructionDecoder::OPCodeCB0x86, this);
    opcode_cb_functions[135] =
        std::bind(&InstructionDecoder::OPCodeCB0x87, this);
    opcode_cb_functions[136] =
        std::bind(&InstructionDecoder::OPCodeCB0x88, this);
    opcode_cb_functions[137] =
        std::bind(&InstructionDecoder::OPCodeCB0x89, this);
    opcode_cb_functions[138] =
        std::bind(&InstructionDecoder::OPCodeCB0x8A, this);
    opcode_cb_functions[139] =
        std::bind(&InstructionDecoder::OPCodeCB0x8B, this);
    opcode_cb_functions[140] =
        std::bind(&InstructionDecoder::OPCodeCB0x8C, this);
    opcode_cb_functions[141] =
        std::bind(&InstructionDecoder::OPCodeCB0x8D, this);
    opcode_cb_functions[142] =
        std::bind(&InstructionDecoder::OPCodeCB0x8E, this);
    opcode_cb_functions[143] =
        std::bind(&InstructionDecoder::OPCodeCB0x8F, this);
    opcode_cb_functions[144] =
        std::bind(&InstructionDecoder::OPCodeCB0x90, this);
    opcode_cb_functions[145] =
        std::bind(&InstructionDecoder::OPCodeCB0x91, this);
    opcode_cb_functions[146] =
        std::bind(&InstructionDecoder::OPCodeCB0x92, this);
    opcode_cb_functions[147] =
        std::bind(&InstructionDecoder::OPCodeCB0x93, this);
    opcode_cb_functions[148] =
        std::bind(&InstructionDecoder::OPCodeCB0x94, this);
    opcode_cb_functions[149] =
        std::bind(&InstructionDecoder::OPCodeCB0x95, this);
    opcode_cb_functions[150] =
        std::bind(&InstructionDecoder::OPCodeCB0x96, this);
    opcode_cb_functions[151] =
        std::bind(&InstructionDecoder::OPCodeCB0x97, this);
    opcode_cb_functions[152] =
        std::bind(&InstructionDecoder::OPCodeCB0x98, this);
    opcode_cb_functions[153] =
        std::bind(&InstructionDecoder::OPCodeCB0x99, this);
    opcode_cb_functions[154] =
        std::bind(&InstructionDecoder::OPCodeCB0x9A, this);
    opcode_cb_functions[155] =
        std::bind(&InstructionDecoder::OPCodeCB0x9B, this);
    opcode_cb_functions[156] =
        std::bind(&InstructionDecoder::OPCodeCB0x9C, this);
    opcode_cb_functions[157] =
        std::bind(&InstructionDecoder::OPCodeCB0x9D, this);
    opcode_cb_functions[158] =
        std::bind(&InstructionDecoder::OPCodeCB0x9E, this);
    opcode_cb_functions[159] =
        std::bind(&InstructionDecoder::OPCodeCB0x9F, this);
    opcode_cb_functions[160] =
        std::bind(&InstructionDecoder::OPCodeCB0xA0, this);
    opcode_cb_functions[161] =
        std::bind(&InstructionDecoder::OPCodeCB0xA1, this);
    opcode_cb_functions[162] =
        std::bind(&InstructionDecoder::OPCodeCB0xA2, this);
    opcode_cb_functions[163] =
        std::bind(&InstructionDecoder::OPCodeCB0xA3, this);
    opcode_cb_functions[164] =
        std::bind(&InstructionDecoder::OPCodeCB0xA4, this);
    opcode_cb_functions[165] =
        std::bind(&InstructionDecoder::OPCodeCB0xA5, this);
    opcode_cb_functions[166] =
        std::bind(&InstructionDecoder::OPCodeCB0xA6, this);
    opcode_cb_functions[167] =
        std::bind(&InstructionDecoder::OPCodeCB0xA7, this);
    opcode_cb_functions[168] =
        std::bind(&InstructionDecoder::OPCodeCB0xA8, this);
    opcode_cb_functions[169] =
        std::bind(&InstructionDecoder::OPCodeCB0xA9, this);
    opcode_cb_functions[170] =
        std::bind(&InstructionDecoder::OPCodeCB0xAA, this);
    opcode_cb_functions[171] =
        std::bind(&InstructionDecoder::OPCodeCB0xAB, this);
    opcode_cb_functions[172] =
        std::bind(&InstructionDecoder::OPCodeCB0xAC, this);
    opcode_cb_functions[173] =
        std::bind(&InstructionDecoder::OPCodeCB0xAD, this);
    opcode_cb_functions[174] =
        std::bind(&InstructionDecoder::OPCodeCB0xAE, this);
    opcode_cb_functions[175] =
        std::bind(&InstructionDecoder::OPCodeCB0xAF, this);
    opcode_cb_functions[176] =
        std::bind(&InstructionDecoder::OPCodeCB0xB0, this);
    opcode_cb_functions[177] =
        std::bind(&InstructionDecoder::OPCodeCB0xB1, this);
    opcode_cb_functions[178] =
        std::bind(&InstructionDecoder::OPCodeCB0xB2, this);
    opcode_cb_functions[179] =
        std::bind(&InstructionDecoder::OPCodeCB0xB3, this);
    opcode_cb_functions[180] =
        std::bind(&InstructionDecoder::OPCodeCB0xB4, this);
    opcode_cb_functions[181] =
        std::bind(&InstructionDecoder::OPCodeCB0xB5, this);
    opcode_cb_functions[182] =
        std::bind(&InstructionDecoder::OPCodeCB0xB6, this);
    opcode_cb_functions[183] =
        std::bind(&InstructionDecoder::OPCodeCB0xB7, this);
    opcode_cb_functions[184] =
        std::bind(&InstructionDecoder::OPCodeCB0xB8, this);
    opcode_cb_functions[185] =
        std::bind(&InstructionDecoder::OPCodeCB0xB9, this);
    opcode_cb_functions[186] =
        std::bind(&InstructionDecoder::OPCodeCB0xBA, this);
    opcode_cb_functions[187] =
        std::bind(&InstructionDecoder::OPCodeCB0xBB, this);
    opcode_cb_functions[188] =
        std::bind(&InstructionDecoder::OPCodeCB0xBC, this);
    opcode_cb_functions[189] =
        std::bind(&InstructionDecoder::OPCodeCB0xBD, this);
    opcode_cb_functions[190] =
        std::bind(&InstructionDecoder::OPCodeCB0xBE, this);
    opcode_cb_functions[191] =
        std::bind(&InstructionDecoder::OPCodeCB0xBF, this);
    opcode_cb_functions[192] =
        std::bind(&InstructionDecoder::OPCodeCB0xC0, this);
    opcode_cb_functions[193] =
        std::bind(&InstructionDecoder::OPCodeCB0xC1, this);
    opcode_cb_functions[194] =
        std::bind(&InstructionDecoder::OPCodeCB0xC2, this);
    opcode_cb_functions[195] =
        std::bind(&InstructionDecoder::OPCodeCB0xC3, this);
    opcode_cb_functions[196] =
        std::bind(&InstructionDecoder::OPCodeCB0xC4, this);
    opcode_cb_functions[197] =
        std::bind(&InstructionDecoder::OPCodeCB0xC5, this);
    opcode_cb_functions[198] =
        std::bind(&InstructionDecoder::OPCodeCB0xC6, this);
    opcode_cb_functions[199] =
        std::bind(&InstructionDecoder::OPCodeCB0xC7, this);
    opcode_cb_functions[200] =
        std::bind(&InstructionDecoder::OPCodeCB0xC8, this);
    opcode_cb_functions[201] =
        std::bind(&InstructionDecoder::OPCodeCB0xC9, this);
    opcode_cb_functions[202] =
        std::bind(&InstructionDecoder::OPCodeCB0xCA, this);
    opcode_cb_functions[203] =
        std::bind(&InstructionDecoder::OPCodeCB0xCB, this);
    opcode_cb_functions[204] =
        std::bind(&InstructionDecoder::OPCodeCB0xCC, this);
    opcode_cb_functions[205] =
        std::bind(&InstructionDecoder::OPCodeCB0xCD, this);
    opcode_cb_functions[206] =
        std::bind(&InstructionDecoder::OPCodeCB0xCE, this);
    opcode_cb_functions[207] =
        std::bind(&InstructionDecoder::OPCodeCB0xCF, this);
    opcode_cb_functions[208] =
        std::bind(&InstructionDecoder::OPCodeCB0xD0, this);
    opcode_cb_functions[209] =
        std::bind(&InstructionDecoder::OPCodeCB0xD1, this);
    opcode_cb_functions[210] =
        std::bind(&InstructionDecoder::OPCodeCB0xD2, this);
    opcode_cb_functions[211] =
        std::bind(&InstructionDecoder::OPCodeCB0xD3, this);
    opcode_cb_functions[212] =
        std::bind(&InstructionDecoder::OPCodeCB0xD4, this);
    opcode_cb_functions[213] =
        std::bind(&InstructionDecoder::OPCodeCB0xD5, this);
    opcode_cb_functions[214] =
        std::bind(&InstructionDecoder::OPCodeCB0xD6, this);
    opcode_cb_functions[215] =
        std::bind(&InstructionDecoder::OPCodeCB0xD7, this);
    opcode_cb_functions[216] =
        std::bind(&InstructionDecoder::OPCodeCB0xD8, this);
    opcode_cb_functions[217] =
        std::bind(&InstructionDecoder::OPCodeCB0xD9, this);
    opcode_cb_functions[218] =
        std::bind(&InstructionDecoder::OPCodeCB0xDA, this);
    opcode_cb_functions[219] =
        std::bind(&InstructionDecoder::OPCodeCB0xDB, this);
    opcode_cb_functions[220] =
        std::bind(&InstructionDecoder::OPCodeCB0xDC, this);
    opcode_cb_functions[221] =
        std::bind(&InstructionDecoder::OPCodeCB0xDD, this);
    opcode_cb_functions[222] =
        std::bind(&InstructionDecoder::OPCodeCB0xDE, this);
    opcode_cb_functions[223] =
        std::bind(&InstructionDecoder::OPCodeCB0xDF, this);
    opcode_cb_functions[224] =
        std::bind(&InstructionDecoder::OPCodeCB0xE0, this);
    opcode_cb_functions[225] =
        std::bind(&InstructionDecoder::OPCodeCB0xE1, this);
    opcode_cb_functions[226] =
        std::bind(&InstructionDecoder::OPCodeCB0xE2, this);
    opcode_cb_functions[227] =
        std::bind(&InstructionDecoder::OPCodeCB0xE3, this);
    opcode_cb_functions[228] =
        std::bind(&InstructionDecoder::OPCodeCB0xE4, this);
    opcode_cb_functions[229] =
        std::bind(&InstructionDecoder::OPCodeCB0xE5, this);
    opcode_cb_functions[230] =
        std::bind(&InstructionDecoder::OPCodeCB0xE6, this);
    opcode_cb_functions[231] =
        std::bind(&InstructionDecoder::OPCodeCB0xE7, this);
    opcode_cb_functions[232] =
        std::bind(&InstructionDecoder::OPCodeCB0xE8, this);
    opcode_cb_functions[233] =
        std::bind(&InstructionDecoder::OPCodeCB0xE9, this);
    opcode_cb_functions[234] =
        std::bind(&InstructionDecoder::OPCodeCB0xEA, this);
    opcode_cb_functions[235] =
        std::bind(&InstructionDecoder::OPCodeCB0xEB, this);
    opcode_cb_functions[236] =
        std::bind(&InstructionDecoder::OPCodeCB0xEC, this);
    opcode_cb_functions[237] =
        std::bind(&InstructionDecoder::OPCodeCB0xED, this);
    opcode_cb_functions[238] =
        std::bind(&InstructionDecoder::OPCodeCB0xEE, this);
    opcode_cb_functions[239] =
        std::bind(&InstructionDecoder::OPCodeCB0xEF, this);
    opcode_cb_functions[240] =
        std::bind(&InstructionDecoder::OPCodeCB0xF0, this);
    opcode_cb_functions[241] =
        std::bind(&InstructionDecoder::OPCodeCB0xF1, this);
    opcode_cb_functions[242] =
        std::bind(&InstructionDecoder::OPCodeCB0xF2, this);
    opcode_cb_functions[243] =
        std::bind(&InstructionDecoder::OPCodeCB0xF3, this);
    opcode_cb_functions[244] =
        std::bind(&InstructionDecoder::OPCodeCB0xF4, this);
    opcode_cb_functions[245] =
        std::bind(&InstructionDecoder::OPCodeCB0xF5, this);
    opcode_cb_functions[246] =
        std::bind(&InstructionDecoder::OPCodeCB0xF6, this);
    opcode_cb_functions[247] =
        std::bind(&InstructionDecoder::OPCodeCB0xF7, this);
    opcode_cb_functions[248] =
        std::bind(&InstructionDecoder::OPCodeCB0xF8, this);
    opcode_cb_functions[249] =
        std::bind(&InstructionDecoder::OPCodeCB0xF9, this);
    opcode_cb_functions[250] =
        std::bind(&InstructionDecoder::OPCodeCB0xFA, this);
    opcode_cb_functions[251] =
        std::bind(&InstructionDecoder::OPCodeCB0xFB, this);
    opcode_cb_functions[252] =
        std::bind(&InstructionDecoder::OPCodeCB0xFC, this);
    opcode_cb_functions[253] =
        std::bind(&InstructionDecoder::OPCodeCB0xFD, this);
    opcode_cb_functions[254] =
        std::bind(&InstructionDecoder::OPCodeCB0xFE, this);
    opcode_cb_functions[255] =
        std::bind(&InstructionDecoder::OPCodeCB0xFF, this);
}
