#pragma once
#include <assert.h>
#include <iostream>
#include "Processor.hh"
#include "TestUtils.hh"
#include "Constants.hh"

namespace Test
{
void testAddition(Processor &p);
bool test8bit(Processor &p);
bool test16bit(Processor &p);
}