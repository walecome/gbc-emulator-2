#pragma once
#include "TestUtils.hh"
#include "Register8bit.hh"

namespace TestRegister8bit
{
void runAllTests();

bool testInitialValue();
bool testSetGet();
bool testIncrement();
bool testDecrement();

} // namespace TestRegister8bit