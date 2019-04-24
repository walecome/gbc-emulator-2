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

int main(int argc, char **argv) {
    ptr<Processor> processor { std::make_shared<Processor>() };
    InstructionDecoder instructionDecoder { processor };

    bool rom_provided = argc == 2;

    if (rom_provided) {
        std::cout << "Reading instructions from binary file..." << std::endl;
        std::string filename { argv[1] };
        processor->readInstructions(filename.c_str());
    }

    // Start instructions
    processor->setValuePC(0x100);
    instructionDecoder.step(true);
    instructionDecoder.step(true);
    instructionDecoder.step(true);

    return 0;
}