#include "TerminalInputHandler.hh"

TIH::TIH() { std::cout << "Type 'help' to show commands" << std::endl; }

bool TIH::getInput() {
    std::cout << "Input: ";
    std::cin >> input;
    if (input == "q" || input == "quit") return false;

    return true;
}

void TIH::handle_input(InstructionDecoder &id, ptr<Processor> &p) {
    if (input == "step" || input == "s") {
        id.step();
    } else if (input == "stack") {
        p->printStack();
    } else if (input == "pm" || input == "p") {
        p->printProgramMemory();
    } else if (input == "dump" || input == "d") {
        p->dump();
    } else if (input == "help") {
        print_help();
    } else {
        std::cout << "Invalid input" << std::endl;
    }
}

void TIH::print_help() {
    std::cout << "Avaliable commands:" << std::endl;
    std::cout << "\tstep" << std::endl;
    std::cout << "\tstack" << std::endl;
    std::cout << "\tpm" << std::endl;
    std::cout << "\tdump" << std::endl;
    std::cout << "\tquit" << std::endl;
}