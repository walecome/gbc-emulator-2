// System headers
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
#include "Metadata.hh"
#include "Processor.hh"
#include "TerminalInputHandler.hh"

int main(int argc, char **argv) {
    ptr<Processor> processor { std::make_shared<Processor>() };
    InstructionDecoder instructionDecoder { processor };

    bool rom_provided { argc == 2 };

    if (!rom_provided) {
        std::cerr << "Please provide a ROM file as argument" << std::endl;
        return 1;
    }

    std::string filename { argv[1] };
    processor->readInstructions(filename, true);

    Util::ROM_Metadata metadata { processor->rom_data };
    metadata.dump();

    TIH inputHandler {};

    while (inputHandler.getInput()) {
        inputHandler.handle_input(instructionDecoder, processor);
    }

    return 0;
}