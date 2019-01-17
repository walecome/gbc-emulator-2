#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <assert.h>

// User headers
#include "Instruction.hh"
#include "Constants.hh"
#include "Processor.hh"

int main()
{
    std::string filename = "roms/pokemon_silver.gbc";

    Processor p{};
    std::cout << "Reading instructions from binary file..."
              << std::endl;

    p.readInstructions(filename.c_str());
    p.print();
    p.printStack(10, 20);

    return 0;
}