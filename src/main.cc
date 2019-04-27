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

void handle_input(const std::string &input, InstructionDecoder &id,
                  ptr<Processor> &p) {
    if (input == "step" || input == "s") {
        id.step();
    } else if (input == "stack") {
        p->printStack();
    } else if (input == "pm" || input == "p") {
        p->printProgramMemory();
    } else if (input == "dump" || input == "d") {
        p->dump();
    } else if (input == "help") {
        std::cout << "Avaliable commands:" << std::endl;
        std::cout << "\tstep" << std::endl;
        std::cout << "\tstack" << std::endl;
        std::cout << "\tpm" << std::endl;
        std::cout << "\tdump" << std::endl;
        std::cout << "\tquit" << std::endl;
    } else {
        std::cout << "Invalid input" << std::endl;
    }
}

int main(int argc, char **argv) {
    ptr<Processor> processor { std::make_shared<Processor>() };
    InstructionDecoder instructionDecoder { processor };

    bool rom_provided = argc == 2;

    if (!rom_provided) {
        std::cerr << "Please provide a ROM file as argument" << std::endl;
        return 1;
    }

    std::cout << "Reading instructions from binary file..." << std::endl;
    std::string filename { argv[1] };
    processor->readInstructions(filename);

    Util::ROM_Metadata metadata { processor->rom_data };
    metadata.dump();

    // Start instructions
    processor->PC->setValue(PC_START);

    std::string input;

    std::cout << "Type 'help' to show commands" << std::endl;

    while (1) {
        std::cout << "Input: ";
        std::cin >> input;
        if (input == "q" || input == "quit") return 0;

        handle_input(input, instructionDecoder, processor);
    }

    return 0;
}