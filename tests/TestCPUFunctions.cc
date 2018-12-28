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
    Register16bit address_reg("address_reg");
    Register8bit data_reg("data_reg");

    register16_t reg_address = 0x0012;
    register16_t actual_address = 0xFF00 + reg_address;

    byte_t data = 0xEB;

    address_reg.setValue(reg_address);
    data_reg.setValue(data);

    p.loadIntoMemory(&address_reg, &data_reg);
    if (p.getRAM()->_get_mem_vector()[actual_address] != data)
    {
        return false;
    }

    return true;
}

bool TestCPU::testLoadIntoMemIm()
{
    Processor p{};
    Register16bit address_reg("address_reg");

    register16_t reg_address = 0x0012;
    register16_t actual_address = 0xFF00 + reg_address;

    byte_t data = 0xBE;

    address_reg.setValue(reg_address);

    p.loadIntoMemory(&address_reg, data);
    if (p.getRAM()->_get_mem_vector()[actual_address] != data)
    {
        return false;
    }

    return true;
}

bool TestCPU::testLoadFromMem()
{
    return false;
}

bool TestCPU::testLoadFromMemIm()
{
    return false;
}

bool TestCPU::testRegisterIncrement()
{
    return false;
}

bool TestCPU::testRegisterDecrement()
{
    return false;
}

bool TestCPU::testRegiserCopy()
{
    return false;
}

bool TestCPU::testRegisterSubtraction()
{
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
    return false;
}

bool TestCPU::testStackPop()
{
    return false;
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