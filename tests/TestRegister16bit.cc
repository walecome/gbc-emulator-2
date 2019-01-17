#include "TestRegister16bit.hh"
#include <cassert>
bool TestRegister16bit::testInitialValue()
{
    Register16bit reg("tmp");
    register16_t expected_value = 0xFFFF;

    return reg.getValue() == expected_value;
}

bool TestRegister16bit::testSetGet()
{
    Register16bit reg("tmp");

    for (register16_t i = 0x0000; i < 0xFFFF; ++i)
    {
        reg.setValue(i);

        if (reg.getValue() != i)
        {
            return false;
        }
    }

    // Test one explicit value

    register16_t expected_value = 0xEBEB;

    reg.setValue(expected_value);

    return (reg.getValue() == expected_value);
}

bool TestRegister16bit::testIncrement()
{
    Register16bit reg("tmp");

    // Start somewhere in the middle
    register16_t expected_value = 0xEB00;
    reg.setValue(expected_value);

    for (register16_t i = 0x0000; i < 0xFFFF; ++i)
    {
        reg.increment();

        ++expected_value;

        if (reg.getValue() != expected_value)
        {
            return false;
        }
    }

    return true;
}

bool TestRegister16bit::testDecrement()
{
    Register16bit reg("tmp");

    // Start somewhere in the middle
    register16_t expected_value = 0xEB00;
    reg.setValue(expected_value);

    for (register16_t i = 0x0000; i < 0xFFFF; ++i)
    {
        reg.decrement();

        if (reg.getValue() != --expected_value)
            return false;
    }

    return true;
}

void TestRegister16bit::runAllTests()
{
    TestUtils::runTestNoArg(TestRegister16bit::testInitialValue,
                            "TestRegister16bit::testInitialValue");
    TestUtils::runTestNoArg(TestRegister16bit::testSetGet,
                            "TestRegister16bit::testSetGet");
    TestUtils::runTestNoArg(TestRegister16bit::testIncrement,
                            "TestRegister16bit::testIncrement");
    TestUtils::runTestNoArg(TestRegister16bit::testDecrement,
                            "TestRegister16bit::testDecrement");
}