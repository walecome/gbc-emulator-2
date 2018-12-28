#include "TestCPUFunctions.hh"

bool TestCPU::testLoadReg(Processor &p)
{
    register16_t current_pc = 0xABCD;
    register16_t next_pc = current_pc++;
    p.setValuePC(current_pc);

    byte_t value = 0xEB;

    register16_t index = p.getValuePC();

    p.getProgramMem()[index] = value;

    throw 1;

    Register8bit reg("Temp");

    p.loadRegister(&reg);

    if (reg.getValue() != value)
        return false;

    if (p.getValuePC() != next_pc)
        return false;

    return true;
}

bool TestCPU::testLoadIntoMem(Processor &p)
{
    return false;
}

bool TestCPU::testLoadIntoMemIm(Processor &p)
{
    return false;
}

bool TestCPU::testLoadFromMem(Processor &p)
{
    return false;
}

bool TestCPU::testLoadFromMemIm(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterIncrement(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterDecrement(Processor &p)
{
    return false;
}

bool TestCPU::testRegiserCopy(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterSubtraction(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterAddCarry(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterSubCarry(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterAND(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterXOR(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterOR(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterCMP(Processor &p)
{
    return false;
}

bool TestCPU::testStackPush(Processor &p)
{
    return false;
}

bool TestCPU::testStackPop(Processor &p)
{
    return false;
}

bool TestCPU::testJump(Processor &p)
{
    return false;
}

bool TestCPU::testJumpIm(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterRLC(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterRRC(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterRL(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterRR(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterSLA(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterSRA(Processor &p)
{
    return false;
}

bool TestCPU::testSwapNibble(Processor &p)
{
    return false;
}

bool TestCPU::testRegisterSRL(Processor &p)
{
    return false;
}

bool TestCPU::testBitTest(Processor &p)
{
    return false;
}

bool TestCPU::testBitReset(Processor &p)
{
    return false;
}

bool TestCPU::testBitSet(Processor &p)
{
    return false;
}

void TestCPU::runAllTests(Processor &p)
{
    TestUtils::runCPUTest(TestCPU::testLoadReg, p,
                          "TestCPU::testLoadReg");

    TestUtils::runCPUTest(TestCPU::testLoadIntoMem, p,
                          "TestCPU::testLoadIntoMem");

    TestUtils::runCPUTest(TestCPU::testLoadIntoMemIm, p,
                          "TestCPU::testLoadIntoMemIm");

    TestUtils::runCPUTest(TestCPU::testLoadFromMem, p,
                          "TestCPU::testLoadFromMem");

    TestUtils::runCPUTest(TestCPU::testLoadFromMemIm, p,
                          "TestCPU::testLoadFromMemIm");

    TestUtils::runCPUTest(TestCPU::testRegisterIncrement, p,
                          "TestCPU::testRegisterIncrement");

    TestUtils::runCPUTest(TestCPU::testRegisterDecrement, p,
                          "TestCPU::testRegisterDecrement");

    TestUtils::runCPUTest(TestCPU::testRegiserCopy, p,
                          "TestCPU::testRegiserCopy");

    TestUtils::runCPUTest(TestCPU::testRegisterSubtraction, p,
                          "TestCPU::testRegisterSubtraction");

    TestUtils::runCPUTest(TestCPU::testRegisterAddCarry, p,
                          "TestCPU::testRegisterAddCarry");

    TestUtils::runCPUTest(TestCPU::testRegisterSubCarry, p,
                          "TestCPU::testRegisterSubCarry");

    TestUtils::runCPUTest(TestCPU::testRegisterAND, p,
                          "TestCPU::testRegisterAND");

    TestUtils::runCPUTest(TestCPU::testRegisterXOR, p,
                          "TestCPU::testRegisterXOR");

    TestUtils::runCPUTest(TestCPU::testRegisterOR, p,
                          "TestCPU::testRegisterOR");

    TestUtils::runCPUTest(TestCPU::testRegisterCMP, p,
                          "TestCPU::testRegisterCMP");

    TestUtils::runCPUTest(TestCPU::testStackPush, p,
                          "TestCPU::testStackPush");

    TestUtils::runCPUTest(TestCPU::testStackPop, p,
                          "TestCPU::testStackPop");

    TestUtils::runCPUTest(TestCPU::testJump, p,
                          "TestCPU::testJump");

    TestUtils::runCPUTest(TestCPU::testJumpIm, p,
                          "TestCPU::testJumpIm");

    TestUtils::runCPUTest(TestCPU::testRegisterRLC, p,
                          "TestCPU::testRegisterRLC");

    TestUtils::runCPUTest(TestCPU::testRegisterRRC, p,
                          "TestCPU::testRegisterRRC");

    TestUtils::runCPUTest(TestCPU::testRegisterRL, p,
                          "TestCPU::testRegisterRL");

    TestUtils::runCPUTest(TestCPU::testRegisterRR, p,
                          "TestCPU::testRegisterRR");

    TestUtils::runCPUTest(TestCPU::testRegisterSLA, p,
                          "TestCPU::testRegisterSLA");

    TestUtils::runCPUTest(TestCPU::testRegisterSRA, p,
                          "TestCPU::testRegisterSRA");

    TestUtils::runCPUTest(TestCPU::testSwapNibble, p,
                          "TestCPU::testSwapNibble");

    TestUtils::runCPUTest(TestCPU::testRegisterSRL, p,
                          "TestCPU::testRegisterSRL");

    TestUtils::runCPUTest(TestCPU::testBitTest, p,
                          "TestCPU::testBitTest");

    TestUtils::runCPUTest(TestCPU::testBitReset, p,
                          "TestCPU::testBitReset");

    TestUtils::runCPUTest(TestCPU::testBitSet, p,
                          "TestCPU::testBitSet");
}