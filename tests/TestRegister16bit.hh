#pragma once
#include "TestUtils.hh"
#include "Register8bit.hh"
#include "Register16bit.hh"

namespace TestRegister16bit
{
void runAllTests();

bool testInitialValue();
bool testSetGet();
bool testIncrement();
bool testDecrement();

} // namespace TestRegister16bit