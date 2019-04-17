#pragma once
#include <functional>
#include <iostream>
#include <string>
#include "Processor.hh"

namespace TestUtils {
void log(std::string msg);
void err(std::string msg);
void runCPUTest(std::function<bool(Processor &)> testFunc, Processor &p,
                std::string testName);

void runTestNoArg(std::function<bool()> testFunc, std::string testName);
void logTestResult(bool passed, std::string testName);

void printResults();

extern int total_tests;
extern int passed_tests;
extern int failed_tests;
}  // namespace TestUtils
