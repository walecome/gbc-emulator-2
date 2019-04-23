#include <iomanip>
#include "Processor.hh"

void hexPrint(unsigned value, unsigned length, bool flush = false) {
    std::cout << "0x" << std::setfill('0') << std::setw(length) << std::hex
              << value;

    if (flush) std::cout << std::endl;
}

// TODO move to own file
void readMetaData(const std::vector<opcode_t> &data) {
    std::ostringstream is {};

    for (int i = 0; i < 11; ++i) {
        char c = data[0x0134 + i];
        if (c == 0) break;
        is << c;
    }

    std::string name { is.str() };
    std::cout << "name: " << name << std::endl;

    // ROM size
    register8_t rom_size = data[0x148];

    std::cout << "rom size: ";
    hexPrint(rom_size, 2, true);
}

void Processor::readInstructions(const char *filename) {
    // open the file as binary
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error reading file " << filename << std::endl;
        return;
    }

    // Stop eating new lines in binary mode
    file.unsetf(std::ios::skipws);

    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<opcode_t> tmp {};

    // read the data:
    tmp.insert(tmp.begin(), std::istream_iterator<opcode_t>(file),
               std::istream_iterator<opcode_t>());

    readMetaData(tmp);

    return;

    std::cout << "Starting with PC: ";
    hexPrint(PC->getValue(), 4);
    std::cout << std::endl;

    for (opcode_t val : tmp) {
        program_memory->setData(PC->getValue(), val);
        PC->increment();
        hexPrint(PC->getValue(), 4);
        std::cout << std::endl;
    }

    std::cout << PC->getValue() << std::endl;

    std::cout << "Inserted " << tmp.size() << " elements into program memory"
              << std::endl;

    std::cout << "Ending with PC: ";
    hexPrint(PC->getValue(), 4);
    std::cout << std::endl;

    // PC->setValue(PC_START);
}

void Processor::printStack(int radius) {
    register16_t start = std::max<int>(0, (int)(SP->getValue()) - radius);
    register16_t end = std::min<int>(SP_START, (int)(SP->getValue()) + radius);

    std::cout << "Stack: " << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    byte_t value;
    while (start != end) {
        ++start;
        value = stack->getData(start);

        std::cout << "\t";
        hexPrint(start, 4);
        std::cout << ": ";
        hexPrint(value, 2);

        if (start == SP->getValue()) {
            std::cout << " <---";
        }

        std::cout << std::endl;
    }
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    std::cout << "Done printing stack" << std::endl;
}

void Processor::dump() {
    std::cout << "Printing processor" << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    std::cout << "\t" << *A << "\t\t" << *B << std::endl
              << "\t" << *D << "\t\t" << *C << std::endl
              << "\t" << *E << "\t\t" << *F << std::endl
              << "\t" << *H << "\t\t" << *L << std::endl;

    std::cout << "\t" << *AF << "\t" << *BC << std::endl;
    std::cout << "\t" << *DE << "\t" << *HL << std::endl;
    std::cout << "\t" << *SP << "\t";
    std::cout << *PC << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
}