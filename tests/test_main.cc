#include "Processor.hh"
#include "TestAddition.hh"
#include "TestCPUFunctions.hh"
#include "TestRegister16bit.hh"
#include "TestRegister8bit.hh"

int main() {
    Processor p {};

    TestRegister8bit::runAllTests();
    TestRegister16bit::runAllTests();

    Test::testAddition(p);

    TestCPU::runAllTests();

    TestUtils::printResults();
    return 0;
}