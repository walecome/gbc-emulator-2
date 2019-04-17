#include "TestRegister8bit.hh"

bool TestRegister8bit::testInitialValue() {
    Register8bit reg("tmp");
    register8_t expected_value = 0xFF;

    return reg.getValue() == expected_value;
}

bool TestRegister8bit::testSetGet() {
    Register8bit reg("tmp");

    for (register8_t i = 0x00; i < 0xFF; ++i) {
        reg.setValue(i);

        if (reg.getValue() != i) {
            return false;
        }
    }

    // Test one explicit value

    register8_t expected_value = 0xEB;
    reg.setValue(expected_value);

    return reg.getValue() == expected_value;
}

bool TestRegister8bit::testIncrement() {
    Register8bit reg("tmp");

    // Start somewhere in the middle
    register8_t expected_value = 0xEB;
    reg.setValue(expected_value);

    for (register8_t i = 0x00; i < 0xFF; ++i) {
        reg.increment();

        if (reg.getValue() != ++expected_value) return false;
    }

    return true;
}

bool TestRegister8bit::testDecrement() {
    Register8bit reg("tmp");

    // Start somewhere in the middle
    register8_t expected_value = 0xEB;
    reg.setValue(expected_value);

    for (register8_t i = 0x00; i < 0xFF; ++i) {
        reg.decrement();

        if (reg.getValue() != --expected_value) return false;
    }

    return true;
}

void TestRegister8bit::runAllTests() {
    TestUtils::runTestNoArg(TestRegister8bit::testInitialValue,
                            "TestRegister8bit::testInitialValue");
    TestUtils::runTestNoArg(TestRegister8bit::testSetGet,
                            "TestRegister8bit::testSetGet");
    TestUtils::runTestNoArg(TestRegister8bit::testIncrement,
                            "TestRegister8bit::testIncrement");
    TestUtils::runTestNoArg(TestRegister8bit::testDecrement,
                            "TestRegister8bit::testDecrement");
}