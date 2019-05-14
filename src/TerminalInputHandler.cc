#include "TerminalInputHandler.hh"

TIH::TIH(ptr<InstructionDecoder> instructionDecoder, ptr<Processor> cpu)
    : instructionDecoder { instructionDecoder },
      cpu { cpu },
      mainwin { initscr() } {
    if (!initCurses()) {
        throw std::runtime_error("Error initializing curses");
    }

    // No breakpoints
    breakpoints.fill(false);
    // std::cout << "Type 'help' to show commands" << std::endl;
}

TIH::~TIH() {
    // Clean up curses

    delwin(mainwin);
    endwin();
    refresh();
}

bool TIH::initCurses() {
    if (mainwin == NULL) {
        return false;
    }

    noecho();              /*  Turn off key echoing                 */
    keypad(mainwin, TRUE); /*  Enable the keypad for non-char keys  */

    refresh();
    wborder(mainwin, 0, 0, 0, 0, 0, 0, 0, 0);

    renderCPUInfo();

    return true;
}

void TIH::renderCPUInfo() {
    CPU_info info = cpu->getCPUInfo();

    render8bitRegister(info);
    render16bitRegister(info);
    renderInfo(info);
    renderPM(info);

    refresh();
}

void renderPair(int y, int x, const char *name, const char *value) {
    mvprintw(y, x, "%s: %s", name, value);
}

void TIH::render8bitRegister(const CPU_info &info) {
    for (unsigned int i = 0; i < info.bit8_regs.size(); ++i) {
        std::string name = info.bit8_regs[i]->getName();
        std::string value = Util::hexString(info.bit8_regs[i]->getValue(), 2);
        int y = 8 + i % 4;
        int x = 20 + (i / 4) * 10;
        renderPair(y, x, name.c_str(), value.c_str());
    }
}

void TIH::render16bitRegister(const CPU_info &info) {
    for (unsigned int i = 0; i < info.bit16_regs.size(); ++i) {
        std::string name = info.bit16_regs[i]->getName();
        std::string value = Util::hexString(info.bit16_regs[i]->getValue(), 4);
        renderPair(8 + i, 40, name.c_str(), value.c_str());
    }
}

void TIH::renderInfo(const CPU_info &info) {
    std::string name = info.SP->getName();
    std::string value = Util::hexString(info.SP->getValue(), 4).c_str();
    renderPair(8, 50, name.c_str(), value.c_str());

    name = info.PC->getName().c_str();
    value = Util::hexString(info.PC->getValue(), 4).c_str();
    renderPair(9, 50, name.c_str(), value.c_str());
}

void TIH::renderPM(const CPU_info &info) {
    int arrowOffset;
    int offsetY = 8;

    for (unsigned int i = 0; i < info.PM.size(); ++i) {
        auto addressInfo = info.PM[i];
        if (cpu->PC->getValue() == addressInfo.address) arrowOffset = i;
        mvprintw(i + offsetY, 65, "%s", addressInfo.str().c_str());
    }

    mvprintw(offsetY + arrowOffset, 80, "<----");
}

void TIH::clear() {
    wclear(mainwin);
    wborder(mainwin, 0, 0, 0, 0, 0, 0, 0, 0);
    refresh();
}

void TIH::cursesLoop() {
    /*  Loop until user presses 'q'  */

    int ch;

    while ((ch = getch()) != 'q') {
        /*  Delete the old response line, and print a new one  */
        if (ch == 'r') this->clear();
        if (ch == 's') instructionDecoder->step();

        renderCPUInfo();

        refresh();
    }
}

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
        try {
            while (!breakpoints[p->PC->getValue()]) {
                p->printStack();
                p->printProgramMemory();
                p->dump();
                id.step();
            }

            std::cout << "Stopping at breakpoint "
                      << Util::hexString(p->PC->getValue(), 4) << std::endl;

        } catch (const std::runtime_error &e) {
            std::cout << "Executing of opcode " << e.what() << " failed"
                      << std::endl;
        }
    } else if (match(input, { "mem" })) {
        register16_t addr;
        std::cout << "Address: ";
        std::cin >> std::hex >> addr;
        p->printPMAddressData(addr);

    } else if (match(input, { "b", "breakpoint" })) {
        register16_t addr;
        std::cout << "Address: ";
        std::cin >> std::hex >> addr;

        if (breakpoints[addr]) {
            std::cout << "Clearing breakpoint at address "
                      << Util::hexString(addr, 4) << std::endl;
            breakpoints[addr] = false;
        } else {
            std::cout << "Setting breakpoint att address "
                      << Util::hexString(addr, 4) << std::endl;
            breakpoints[addr] = true;
        }

    } else if (match(input, { "clearall" })) {
        std::cout << "Clearing all breakpoints" << std::endl;
        breakpoints.fill(false);
    } else {
        std::cout << "Invalid input" << std::endl;
    }
}

void TIH::print_help() {
    std::cout << "Avaliable commands:" << std::endl;
    std::cout << "\tbreakpoint" << std::endl;
    std::cout << "\tclearall" << std::endl;
    std::cout << "\tdump" << std::endl;
    std::cout << "\tmem" << std::endl;
    std::cout << "\tpm" << std::endl;
    std::cout << "\tquit" << std::endl;
    std::cout << "\trun" << std::endl;
    std::cout << "\tstack" << std::endl;
    std::cout << "\tstep" << std::endl;
}