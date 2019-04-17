#include <assert.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

// User headers
#include "Constants.hh"
#include "Processor.hh"

int main() {
    std::string filename { "roms/pokemon_silver.gbc" };

    Processor p {};
    std::cout << "Reading instructions from binary file..." << std::endl;

    p.readInstructions(filename.c_str());
    p.print();
    p.printStack(10, 20);

    return 0;
}