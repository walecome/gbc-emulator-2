#include "TestUtils.hh"

namespace TestUtils
{
int cpu_total_tests = 0;
int cpu_passed_tests = 0;
int cpu_failed_tests = 0;
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
    TestUtils::cpu_total_tests++;

    // TestUtils::log("[" + testName + "] running test");
    bool test_ok = testFunc(p);

    if (test_ok)
    {
        TestUtils::cpu_passed_tests++;
        TestUtils::log(strGreen("passed") + " - [" + testName + "]");
    }
    else
    {
        TestUtils::cpu_failed_tests++;
        TestUtils::log(strRed("failed") + " - [" + testName + "]");
    }
}

void TestUtils::runTestNoArg(std::function<bool()> testFunc, std::string testName)
{
    TestUtils::cpu_total_tests++;

    bool test_ok = testFunc();

    if (test_ok)
    {
        TestUtils::cpu_passed_tests++;
        TestUtils::log(strGreen("passed") + " - [" + testName + "]");
    }
    else
    {
        TestUtils::cpu_failed_tests++;
        TestUtils::log(strRed("failed") + " - [" + testName + "]");
    }
}

void TestUtils::printResults()
{
    std::cout << "Total cpu tests: " << TestUtils::cpu_total_tests << std::endl;
    std::cout << "Passed cpu tests: " << TestUtils::cpu_passed_tests << std::endl;
    std::cout << "Failed cpu tests: " << TestUtils::cpu_failed_tests << std::endl;
}