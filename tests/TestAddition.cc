#include "TestAddition.hh"

bool Test::test8bit(Processor &p) {
    for (unsigned int i = 0; i < 0xFF; ++i) {
        for (unsigned int j; j < 0xFF; ++j) {
            register8_t reg_a_val = (register8_t)i;
            register8_t reg_b_val = (register8_t)j;
            register8_t expected_val = reg_a_val + reg_b_val;

            p.setValueA(reg_a_val);
            p.setValueB(reg_b_val);

            p.addRegisters(p.getA(), p.getB());

            bool ok = p.getValueA() == expected_val;

            if (!ok) {
                return false;
            }
        }
    }

    return true;
}

// What is edge case testing hmmmmm
bool Test::test16bit(Processor &p) {
    for (unsigned int i = 0; i < 0xFFFF; ++i) {
        for (unsigned int j; j < 0xFFFF; ++j) {
            register16_t reg_af_val = (register16_t)i;
            register16_t reg_bc_val = (register16_t)j;
            register16_t expected_val = reg_af_val + reg_bc_val;

            p.setValueAF(reg_af_val);
            p.setValueBC(reg_bc_val);

            p.addRegisters(p.getAF(), p.getBC());

            bool ok = p.getValueAF() == expected_val;

            if (!ok) {
                return false;
            }
        }
    }

    return true;
}

void Test::testAddition(Processor &p) {
    TestUtils::runCPUTest(Test::test8bit, p, "Test::test8bit");
    TestUtils::runCPUTest(Test::test16bit, p, "Test::test16bit");
}