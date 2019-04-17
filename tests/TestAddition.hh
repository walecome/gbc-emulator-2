#pragma once
#include <assert.h>
#include <iostream>
#include "Constants.hh"
#include "Processor.hh"
#include "TestUtils.hh"

namespace Test {
void testAddition(Processor &p);
bool test8bit(Processor &p);
bool test16bit(Processor &p);
}  // namespace Test