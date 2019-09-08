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
#include "Window.hh"

// Lib headers
#include "argparse.h"

ArgumentParser parseArgs(int argc, char **argv) {
    ArgumentParser parser("CLI argument parser");
    parser.add_argument("--rom", "The filename of the ROM", true);
    try {
        parser.parse(argc, argv);
    } catch (const ArgumentParser::ArgumentNotFound &ex) {
        std::cerr << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (parser.is_help()) exit(EXIT_SUCCESS);

    return parser;
}

int main(int argc, char **argv) {
    ArgumentParser parser = parseArgs(argc, argv);

    ptr<Processor> processor { std::make_shared<Processor>() };
    ptr<InstructionDecoder> instructionDecoder {
        std::make_shared<InstructionDecoder>(processor)
    };

    std::string filename { parser.get<std::string>("rom") };
    processor->readInstructions(filename, true);

    Util::ROM_Metadata metadata { processor->rom_data };
    metadata.dump();

    TIH inputHandler { instructionDecoder, processor };
    inputHandler.cursesLoop();

    // Window::createMainWindow(300, 300, "Gameboy Color emulator");
    // while (Window::shouldRemainOpen()) {
    //     Window::update();
    // }

    // while (inputHandler.getInput()) {
    // inputHandler.handle_input(instructionDecoder, processor);
    // }

    return 0;
}