#include "Processor.hh"
#include "TestAddition.hh"

int main()
{
    Processor p{};

    Test::testAddition(p);

    TestUtils::printResults();
    return 0;
}