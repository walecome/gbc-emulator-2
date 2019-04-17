#pragma once
#include "Register8bit.hh"
#include "TestUtils.hh"

namespace TestRegister8bit {
void runAllTests();

bool testInitialValue();
bool testSetGet();
bool testIncrement();
bool testDecrement();

}  // namespace TestRegister8bit