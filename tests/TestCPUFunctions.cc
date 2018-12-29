#include "TestCPUFunctions.hh"

bool TestCPU::testLoadReg()
{
    Processor p{};
    register16_t current_pc = 0xABCD;
    register16_t next_pc = current_pc + 1;
    p.setValuePC(current_pc);

    byte_t value = 0xEB;

    register16_t index = p.getValuePC();

    p.getProgramMem()[index] = value;

    Register8bit reg("Temp");

    p.loadRegister(&reg);

    if (reg.getValue() != value)
        return false;

    if (p.getValuePC() != next_pc)
        return false;

    return true;
}

bool TestCPU::testLoadIntoMem()
{
    Processor p{};
    Register16bit address_reg{"address_reg"};
    Register8bit data_reg{"data_reg"};

    register16_t reg_address = 0x0012;
    register16_t actual_address = 0xFF00 + reg_address;

    byte_t data = 0xEB;

    address_reg.setValue(reg_address);
    data_reg.setValue(data);

    p.loadIntoMemory(&address_reg, &data_reg);

    return (p.getRAM()->_get_mem_vector()[actual_address] == data);
}

bool TestCPU::testLoadIntoMemIm()
{
    Processor p{};
    Register16bit address_reg{"address_reg"};

    register16_t reg_address = 0x0012;
    register16_t actual_address = 0xFF00 + reg_address;

    byte_t data = 0xBE;

    address_reg.setValue(reg_address);

    p.loadIntoMemory(&address_reg, data);

    return (p.getRAM()->_get_mem_vector()[actual_address] == data);
}

bool TestCPU::testLoadFromMem()
{
    Processor p{};
    Register8bit data_reg{"tmp"};
    Register16bit address_reg{"tmp"};

    register16_t reg_address = 0x0012;
    register16_t actual_address = 0xFF00 + reg_address;

    byte_t expected_data = 0xBE;

    p.getRAM()->_get_mem_vector()[actual_address] = expected_data;

    address_reg.setValue(reg_address);

    p.loadFromMemory(&data_reg, &address_reg);

    return (data_reg.getValue() == expected_data);
}

bool TestCPU::testLoadFromMemIm()
{
    Processor p{};
    Register16bit address_reg{"tmp"};

    register16_t reg_address = 0x0012;
    register16_t actual_address = 0xFF00 + reg_address;

    byte_t expected_data = 0xBE;

    p.getRAM()->_get_mem_vector()[actual_address] = expected_data;

    address_reg.setValue(reg_address);

    byte_t value = p.loadFromMemory(&address_reg);

    return (value == expected_data);
}

bool TestCPU::testRegisterIncrement()
{
    // TODO check flag logic
    return false;
}

bool TestCPU::testRegisterDecrement()
{
    // TODO check flag logic
    return false;
}

bool TestCPU::testRegiserCopy()
{
    Processor p{};
    Register8bit reg1{"reg1"};
    Register8bit reg2{"reg2"};

    register8_t expected_value = 0xEB;

    reg1.setValue(expected_value);

    // Copy value in reg1 into reg2
    p.copyRegister(&reg2, &reg1);

    return (reg2.getValue() == expected_value);
}

bool TestCPU::testRegisterSubtraction()
{
    // TODO check flag logic
    return false;
}

bool TestCPU::testRegisterAddCarry()
{
    return false;
}

bool TestCPU::testRegisterSubCarry()
{
    return false;
}

bool TestCPU::testRegisterAND()
{
    return false;
}

bool TestCPU::testRegisterXOR()
{
    return false;
}

bool TestCPU::testRegisterOR()
{
    return false;
}

bool TestCPU::testRegisterCMP()
{
    return false;
}

bool TestCPU::testStackPush()
{
    Processor p{};
    Register16bit reg{"value_reg"};

    register16_t expected_value = 0xBEEF;
    register8_t expected_high = 0xBE;
    register8_t expected_low = 0xEF;

    reg.setValue(expected_value);

    register16_t current_sp = p.getValueSP();
    register16_t expected_sp = current_sp - 2;

    p.pushStack(&reg);

    if (p.getValueSP() != expected_sp)
    {
        return false;
    }

    if (p.getStack()->_get_mem_vector()[current_sp - 1] != expected_high)
    {
        return false;
    }

    if (p.getStack()->_get_mem_vector()[current_sp - 2] != expected_low)
    {
        return false;
    }

    return true;
}

bool TestCPU::testStackPop()
{
    Processor p{};
    Register16bit reg{"value_reg"};

    Register16bit reg_dest{"dest_reg"};

    register16_t expected_value = 0xBEEF;

    reg.setValue(expected_value);

    p.pushStack(&reg);

    register16_t current_sp = p.getValueSP();
    register16_t expected_sp = current_sp + 2;

    p.popStack(&reg_dest);

    if (p.getValueSP() != expected_sp)
    {
        return false;
    }

    if (reg_dest.getValue() != expected_value)
    {
        return false;
    }

    return true;
}

bool TestCPU::testStackPopAF()
{
    Processor p{};
    Register16bit reg{"value_reg"};

    register16_t expected_value = 0xBEFF;

    reg.setValue(expected_value);

    p.pushStack(&reg);

    register16_t current_sp = p.getValueSP();
    register16_t expected_sp = current_sp + 2;

    p.popStackAF();

    if (!p.getFlagZ())
        return false;

    if (!p.getFlagN())
        return false;

    if (!p.getFlagH())
        return false;

    if (!p.getFlagC())
        return false;

    // All flags 0
    expected_value = 0xBE0F;

    reg.setValue(expected_value);

    p.pushStack(&reg);

    p.popStackAF();

    if (p.getFlagZ())
        return false;

    if (p.getFlagN())
        return false;

    if (p.getFlagH())
        return false;

    if (p.getFlagC())
        return false;

    return true;
}

bool TestCPU::testJump()
{
    return false;
}

bool TestCPU::testJumpIm()
{
    return false;
}

bool TestCPU::testRegisterRLC()
{
    return false;
}

bool TestCPU::testRegisterRRC()
{
    return false;
}

bool TestCPU::testRegisterRL()
{
    return false;
}

bool TestCPU::testRegisterRR()
{
    return false;
}

bool TestCPU::testRegisterSLA()
{
    return false;
}

bool TestCPU::testRegisterSRA()
{
    return false;
}

bool TestCPU::testSwapNibble()
{
    return false;
}

bool TestCPU::testRegisterSRL()
{
    return false;
}

bool TestCPU::testBitTest()
{
    return false;
}

bool TestCPU::testBitReset()
{
    return false;
}

bool TestCPU::testBitSet()
{
    return false;
}

void TestCPU::runAllTests()
{
    TestUtils::runTestNoArg(TestCPU::testLoadReg,
                            "TestCPU::testLoadReg");

    TestUtils::runTestNoArg(TestCPU::testLoadIntoMem,
                            "TestCPU::testLoadIntoMem");

    TestUtils::runTestNoArg(TestCPU::testLoadIntoMemIm,
                            "TestCPU::testLoadIntoMemIm");

    TestUtils::runTestNoArg(TestCPU::testLoadFromMem,
                            "TestCPU::testLoadFromMem");

    TestUtils::runTestNoArg(TestCPU::testLoadFromMemIm,
                            "TestCPU::testLoadFromMemIm");

    TestUtils::runTestNoArg(TestCPU::testRegisterIncrement,
                            "TestCPU::testRegisterIncrement");

    TestUtils::runTestNoArg(TestCPU::testRegisterDecrement,
                            "TestCPU::testRegisterDecrement");

    TestUtils::runTestNoArg(TestCPU::testRegiserCopy,
                            "TestCPU::testRegiserCopy");

    TestUtils::runTestNoArg(TestCPU::testRegisterSubtraction,
                            "TestCPU::testRegisterSubtraction");

    TestUtils::runTestNoArg(TestCPU::testRegisterAddCarry,
                            "TestCPU::testRegisterAddCarry");

    TestUtils::runTestNoArg(TestCPU::testRegisterSubCarry,
                            "TestCPU::testRegisterSubCarry");

    TestUtils::runTestNoArg(TestCPU::testRegisterAND,
                            "TestCPU::testRegisterAND");

    TestUtils::runTestNoArg(TestCPU::testRegisterXOR,
                            "TestCPU::testRegisterXOR");

    TestUtils::runTestNoArg(TestCPU::testRegisterOR,
                            "TestCPU::testRegisterOR");

    TestUtils::runTestNoArg(TestCPU::testRegisterCMP,
                            "TestCPU::testRegisterCMP");

    TestUtils::runTestNoArg(TestCPU::testStackPush,
                            "TestCPU::testStackPush");

    TestUtils::runTestNoArg(TestCPU::testStackPop,
                            "TestCPU::testStackPop");

    TestUtils::runTestNoArg(TestCPU::testStackPopAF,
                            "TestCPU::testStackPopAF");

    TestUtils::runTestNoArg(TestCPU::testJump,
                            "TestCPU::testJump");

    TestUtils::runTestNoArg(TestCPU::testJumpIm,
                            "TestCPU::testJumpIm");

    TestUtils::runTestNoArg(TestCPU::testRegisterRLC,
                            "TestCPU::testRegisterRLC");

    TestUtils::runTestNoArg(TestCPU::testRegisterRRC,
                            "TestCPU::testRegisterRRC");

    TestUtils::runTestNoArg(TestCPU::testRegisterRL,
                            "TestCPU::testRegisterRL");

    TestUtils::runTestNoArg(TestCPU::testRegisterRR,
                            "TestCPU::testRegisterRR");

    TestUtils::runTestNoArg(TestCPU::testRegisterSLA,
                            "TestCPU::testRegisterSLA");

    TestUtils::runTestNoArg(TestCPU::testRegisterSRA,
                            "TestCPU::testRegisterSRA");

    TestUtils::runTestNoArg(TestCPU::testSwapNibble,
                            "TestCPU::testSwapNibble");

    TestUtils::runTestNoArg(TestCPU::testRegisterSRL,
                            "TestCPU::testRegisterSRL");

    TestUtils::runTestNoArg(TestCPU::testBitTest,
                            "TestCPU::testBitTest");

    TestUtils::runTestNoArg(TestCPU::testBitReset,
                            "TestCPU::testBitReset");

    TestUtils::runTestNoArg(TestCPU::testBitSet,
                            "TestCPU::testBitSet");
}