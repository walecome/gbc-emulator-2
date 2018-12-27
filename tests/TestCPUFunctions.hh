#pragma once
#include "Processor.hh"
#include "TestUtils.hh"
#include "Register8bit.hh"
#include "Register16bit.hh"

namespace TestCPU
{
void runAllTests(Processor &p);

bool testLoadReg(Processor &p);
bool testLoadIntoMem(Processor &p);
bool testLoadIntoMemIm(Processor &p);
bool testLoadFromMem(Processor &p);
bool testLoadFromMemIm(Processor &p);
bool testRegisterIncrement(Processor &p);
bool testRegisterDecrement(Processor &p);
bool testRegiserCopy(Processor &p);
bool testRegisterSubtraction(Processor &p);
bool testRegisterAddCarry(Processor &p);
bool testRegisterSubCarry(Processor &p);
bool testRegisterAND(Processor &p);
bool testRegisterXOR(Processor &p);
bool testRegisterOR(Processor &p);
bool testRegisterCMP(Processor &p);
bool testStackPush(Processor &p);
bool testStackPop(Processor &p);
bool testJump(Processor &p);
bool testJumpIm(Processor &p);
bool testRegisterRLC(Processor &p);
bool testRegisterRRC(Processor &p);
bool testRegisterRL(Processor &p);
bool testRegisterRR(Processor &p);
bool testRegisterSLA(Processor &p);
bool testRegisterSRA(Processor &p);
bool testSwapNibble(Processor &p);
bool testRegisterSRL(Processor &p);
bool testBitTest(Processor &p);
bool testBitReset(Processor &p);
bool testBitSet(Processor &p);
} // namespace TestCPU