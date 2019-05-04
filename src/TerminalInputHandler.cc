#include "TerminalInputHandler.hh"

TIH::TIH() { std::cout << "Type 'help' to show commands" << std::endl; }

bool TIH::getInput() {
    std::cout << "Input: ";
    std::cin >> input;
    if (input == "q" || input == "quit") return false;

    return true;
}

bool match(const std::string &s, const std::vector<std::string> &inputs) {
    for (auto &x : inputs) {
        if (x == s) return true;
    }

    return false;
}

void TIH::handle_input(InstructionDecoder &id, ptr<Processor> &p) {
    if (match(input, { "step", "s" })) {
        id.step();
    } else if (match(input, { "stack" })) {
        p->printStack();
    } else if (match(input, { "pm", "p" })) {
        p->printProgramMemory();
    } else if (match(input, { "dump", "d" })) {
        p->dump();
    } else if (match(input, { "help", "h" })) {
        print_help();

    } else if (match(input, { "run", "r" })) {
        while (1) {
            p->printStack();
            p->printProgramMemory();
            p->dump();
            id.step();
        }
    } else if (match(input, { "mem" })) {
        register16_t addr;
        std::cout << "Address: ";
        std::cin >> std::hex >> addr;
        p->printPMAddressData(addr);

    } else {
        std::cout << "Invalid input" << std::endl;
    }
}

void TIH::print_help() {
    std::cout << "Avaliable commands:" << std::endl;
    std::cout << "\tdump" << std::endl;
    std::cout << "\tmem" << std::endl;
    std::cout << "\tpm" << std::endl;
    std::cout << "\tquit" << std::endl;
    std::cout << "\trun" << std::endl;
    std::cout << "\tstack" << std::endl;
    std::cout << "\tstep" << std::endl;
}