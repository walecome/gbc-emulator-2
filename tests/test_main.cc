#include "Processor.hh"
#include "TestAddition.hh"
#include "TestCPUFunctions.hh"

int main()
{
    Processor p{};

    Test::testAddition(p);

    TestCPU::runAllTests(p);

    TestUtils::printResults();
    return 0;
}