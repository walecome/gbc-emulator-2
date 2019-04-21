#include <assert.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// User headers
#include "Constants.hh"
#include "InstructionDecoder.hh"
#include "Processor.hh"

int main() {
    std::string filename { "roms/pokemon_silver.gbc" };

    Processor processor {};
    // InstructionDecoder instructionDecoder { std::make_shared<Processor>(
    // processor) };

    std::cout << "Reading instructions from binary file..." << std::endl;
    processor.readInstructions(filename.c_str());
    processor.print();
    processor.printStack(10, 20);

    return 0;
}