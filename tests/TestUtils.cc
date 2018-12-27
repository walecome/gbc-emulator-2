#include "TestUtils.hh"

namespace TestUtils
{
int total_tests = 0;
int passed_tests = 0;
int failed_tests = 0;
} // namespace TestUtils

void TestUtils::log(std::string msg)
{
    std::cout << "TestUtils (log): " << msg << std::endl;
}

void TestUtils::err(std::string msg)
{
    std::cerr << "TestUtils (err): " << msg << std::endl;
}

std::string strRed(std::string msg)
{
    return "\033[1;31m" + msg + "\033[0m";
}

std::string strGreen(std::string msg)
{
    return "\033[1;32m" + msg + "\033[0m";
}

void TestUtils::runCPUTest(std::function<bool(Processor &)> testFunc,
                           Processor &p, std::string testName)
{
    TestUtils::total_tests++;

    // TestUtils::log("[" + testName + "] running test");
    bool test_ok = testFunc(p);

    TestUtils::logTestResult(test_ok, testName);
}

void TestUtils::runTestNoArg(std::function<bool()> testFunc, std::string testName)
{
    TestUtils::total_tests++;
    // TestUtils::log("[" + testName + "] running test");

    bool test_ok = testFunc();

    TestUtils::logTestResult(test_ok, testName);
}

void TestUtils::logTestResult(bool passed, std::string testName)
{
    if (passed)
    {
        TestUtils::passed_tests++;
        TestUtils::log(strGreen("passed") + " - [" + testName + "]");
    }
    else
    {
        TestUtils::failed_tests++;
        TestUtils::log(strRed("failed") + " - [" + testName + "]");
    }
}

void TestUtils::printResults()
{
    std::cout << "Total tests: " << TestUtils::total_tests << std::endl;
    std::cout << "Passed tests: " << TestUtils::passed_tests << std::endl;
    std::cout << "Failed tests: " << TestUtils::failed_tests << std::endl;
}