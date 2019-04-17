#pragma once
#include "Register16bit.hh"
#include "Register8bit.hh"
#include "TestUtils.hh"

namespace TestRegister16bit {
void runAllTests();

bool testInitialValue();
bool testSetGet();
bool testIncrement();
bool testDecrement();

}  // namespace TestRegister16bit