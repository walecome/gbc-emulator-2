#include "Processor.hh"
#include "TestAddition.hh"
#include "TestRegister8bit.hh"
#include "TestCPUFunctions.hh"

int main()
{
    Processor p{};

    TestRegister8bit::runAllTests();

    Test::testAddition(p);

    TestCPU::runAllTests(p);

    TestUtils::printResults();
    return 0;
}