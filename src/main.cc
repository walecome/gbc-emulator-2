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

    ptr<Processor> processor { std::make_shared<Processor>() };

    InstructionDecoder instructionDecoder { processor };

    std::cout << "Reading instructions from binary file..." << std::endl;
    processor->readInstructions(filename.c_str());
    processor->dump();
    processor->printStack(10);

    instructionDecoder.executeInstruction(0x00);

    return 0;
}