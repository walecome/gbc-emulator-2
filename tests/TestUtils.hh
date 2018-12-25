#pragma once
#include <iostream>
#include <functional>
#include <string>
#include "Processor.hh"

namespace TestUtils
{
void log(std::string msg);
void err(std::string msg);
void runCPUTest(std::function<bool(Processor &)> testFunc, Processor &p,
                std::string testName);
void printResults();

extern int cpu_total_tests;
extern int cpu_passed_tests;
extern int cpu_failed_tests;
} // namespace TestUtils
