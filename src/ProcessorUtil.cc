#include <iomanip>
#include "Processor.hh"

void hexPrint(unsigned value, unsigned length, bool flush = false) {
    std::cout << "0x" << std::setfill('0') << std::setw(length) << std::hex
              << value;

    if (flush) std::cout << std::endl;
}

void readROMTitle(const std::vector<opcode_t> &rom_data) {
    std::ostringstream is {};

    for (int i = 0; i < 11; ++i) {
        char c = rom_data[0x0134 + i];
        if (c == 0) break;
        is << c;
    }

    std::string title { is.str() };
    std::cout << "ROM title: " << title << std::endl;
}

void readROMSizeType(const std::vector<opcode_t> &rom_data) {
    // ROM size type
    register8_t rom_type = rom_data[0x148];

    std::cout << "ROM size type: ";

    switch (rom_type) {
        case 0x00:
            std::cout << "32KByte (no ROM banking)" << std::endl;
            break;

        case 0x01:
            std::cout << "64KByte (4 banks)" << std::endl;
            break;

        case 0x02:
            std::cout << "128KByte (8 banks)" << std::endl;
            break;

        case 0x03:
            std::cout << "256KByte (16 banks)" << std::endl;
            break;

        case 0x04:
            std::cout << "512KByte (32 banks)" << std::endl;
            break;

        case 0x05:
            std::cout << "1MByte (64 banks)  - only 63 banks used by MBC1"
                      << std::endl;
            break;

        case 0x06:
            std::cout << "2MByte (128 banks) - only 125 banks used by MBC1"
                      << std::endl;
            break;

        case 0x07:
            std::cout << "4MByte (256 banks)" << std::endl;
            break;

        case 0x52:
            std::cout << "1.1MByte (72 banks)" << std::endl;
            break;

        case 0x53:
            std::cout << "1.2MByte (80 banks)" << std::endl;
            break;

        case 0x54:
            std::cout << "1.5MByte (96 banks)" << std::endl;
            break;

        default:
            std::cerr << "Invalid ROM size found" << std::endl;
    }
}

void readROMRAMSize(const std::vector<opcode_t> &rom_data) {
    register8_t rom_ram_type = rom_data[0x149];

    std::cout << "ROM RAM size: ";

    switch (rom_ram_type) {
        case 0x00:
            std::cout << "0 bytes" << std::endl;
            break;

        case 0x01:
            std::cout << "2 KBytes" << std::endl;
            break;

        case 0x02:
            std::cout << "8 KBytes" << std::endl;
            break;

        case 0x03:
            std::cout << "32 KBytes" << std::endl;
            break;

        default:
            std::cerr << "Invalid ROM RAM type" << std::endl;
    }
}

void verifyROMChecksum(const std::vector<opcode_t> &rom_data) {
    register8_t header_checksum = rom_data[0x14D];

    register8_t calculated_checksum = 0;

    for (register16_t i = 0x0134; i <= 0x014C; ++i) {
        calculated_checksum = calculated_checksum - rom_data[i] - 1;
    }

    // std::cout << "Header checksum: ";
    // hexPrint(header_checksum, 2, true);
    // std::cout << "Calculated checksum: ";
    // hexPrint(calculated_checksum, 2, true);

    bool match = header_checksum == calculated_checksum;

    if (match) {
        std::cout << "Checksum verified" << std::endl;
    } else {
        std::cerr << "Checksum doesn't match, GAME WILL NOT WORK!" << std::endl;
    }
}

// TODO move to own file
void readMetaData(const std::vector<opcode_t> &rom_data) {
    readROMTitle(rom_data);
    readROMSizeType(rom_data);
    readROMRAMSize(rom_data);
    verifyROMChecksum(rom_data);
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