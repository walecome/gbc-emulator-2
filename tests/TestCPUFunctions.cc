#include "TestCPUFunctions.hh"

void TestCPU::runAllTests(Processor &p)
{
    TestUtils::runCPUTest(TestCPU::testLoadReg, p, "testLoadReg");
    TestUtils::runCPUTest(TestCPU::testLoadIntoMem, p, "testLoadIntoMem");
    TestUtils::runCPUTest(TestCPU::testLoadIntoMemIm, p, "testLoadIntoMemIm");
    TestUtils::runCPUTest(TestCPU::testLoadFromMem, p, "testLoadFromMem");
    TestUtils::runCPUTest(TestCPU::testLoadFromMemIm, p, "testLoadFromMemIm");
    TestUtils::runCPUTest(TestCPU::testRegisterIncrement, p, "testRegisterIncrement");
    TestUtils::runCPUTest(TestCPU::testRegisterDecrement, p, "testRegisterDecrement");
    TestUtils::runCPUTest(TestCPU::testRegiserCopy, p, "testRegiserCopy");
    TestUtils::runCPUTest(TestCPU::testRegisterSubtraction, p, "testRegisterSubtraction");
    TestUtils::runCPUTest(TestCPU::testRegisterAddCarry, p, "testRegisterAddCarry");
    TestUtils::runCPUTest(TestCPU::testRegisterSubCarry, p, "testRegisterSubCarry");
    TestUtils::runCPUTest(TestCPU::testRegisterAND, p, "testRegisterAND");
    TestUtils::runCPUTest(TestCPU::testRegisterXOR, p, "testRegisterXOR");
    TestUtils::runCPUTest(TestCPU::testRegisterOR, p, "testRegisterOR");
    TestUtils::runCPUTest(TestCPU::testRegisterCMP, p, "testRegisterCMP");
    TestUtils::runCPUTest(TestCPU::testStackPush, p, "testStackPush");
    TestUtils::runCPUTest(TestCPU::testStackPop, p, "testStackPop");
    TestUtils::runCPUTest(TestCPU::testJump, p, "testJump");
    TestUtils::runCPUTest(TestCPU::testJumpIm, p, "testJumpIm");
    TestUtils::runCPUTest(TestCPU::testRegisterRLC, p, "testRegisterRLC");
    TestUtils::runCPUTest(TestCPU::testRegisterRRC, p, "testRegisterRRC");
    TestUtils::runCPUTest(TestCPU::testRegisterRL, p, "testRegisterRL");
    TestUtils::runCPUTest(TestCPU::testRegisterRR, p, "testRegisterRR");
    TestUtils::runCPUTest(TestCPU::testRegisterSLA, p, "testRegisterSLA");
    TestUtils::runCPUTest(TestCPU::testRegisterSRA, p, "testRegisterSRA");
    TestUtils::runCPUTest(TestCPU::testSwapNibble, p, "testSwapNibble");
    TestUtils::runCPUTest(TestCPU::testRegisterSRL, p, "testRegisterSRL");
    TestUtils::runCPUTest(TestCPU::testBitTest, p, "testBitTest");
    TestUtils::runCPUTest(TestCPU::testBitReset, p, "testBitReset");
    TestUtils::runCPUTest(TestCPU::testBitSet, p, "testBitSet");
}

bool TestCPU::testLoadReg(Processor &p)
{
    return false;
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
