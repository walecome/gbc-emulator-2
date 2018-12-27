#include "TestRegister8bit.hh"

void TestRegister8bit::runAllTests()
{
    TestUtils::runTestNoArg(testInitialValue, "testInitialValue");
    TestUtils::runTestNoArg(testSetGet, "testSetGet");
    TestUtils::runTestNoArg(testIncrement, "testIncrement");
    TestUtils::runTestNoArg(testDecrement, "testDecrement");
}

bool TestRegister8bit::testInitialValue()
{
    Register8bit reg("tmp");
    register8_t expected_value = 0xFF;

    return reg.getValue() == expected_value;
}

bool TestRegister8bit::testSetGet()
{
    Register8bit reg("tmp");

    for (register8_t i = 0x00; i < 0xFF; ++i)
    {
        reg.setValue(i);

        if (reg.getValue() != i)
        {
            return false;
        }
    }

    return true;
}

bool TestRegister8bit::testIncrement()
{
    Register8bit reg("tmp");

    // Start somewhere in the middle
    register8_t expected_value = 0xEB;
    reg.setValue(expected_value);

    for (register8_t i = 0x00; i < 0xFF; ++i)
    {
        reg.increment();

        if (reg.getValue() != ++expected_value)
            return false;
    }

    return true;
}

bool TestRegister8bit::testDecrement()
{
    Register8bit reg("tmp");

    // Start somewhere in the middle
    register8_t expected_value = 0x0EB;
    reg.setValue(expected_value);

    for (register8_t i = 0x00; i < 0xFF; ++i)
    {
        reg.decrement();

        if (reg.getValue() != --expected_value)
            return false;
    }

    return true;
}
