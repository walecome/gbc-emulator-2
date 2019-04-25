// System headers
#include <iomanip>

// User headers
#include "Processor.hh"

// Forward declarations
void hexPrint(unsigned value, unsigned length, bool flush = false);

std::ostream &operator<<(std::ostream &os,
                         const std::pair<register8_t, std::string> &p) {
    os << "(" << (unsigned)p.first;
    os << ", " << p.second << ")";

    return os;
}

struct ROM_Metadata {
    std::string title;
    std::pair<register8_t, std::string> gameboy_type;
    std::pair<register8_t, std::string> cartridge_type;
    std::pair<register8_t, std::string> rom_size;
    std::pair<register8_t, std::string> ram_size;
    register8_t checksum;

    std::array<register8_t, 4> start_instructions {};

    void dump() {
        std::cout << "ROM metadata:" << std::endl;
        std::cout << "\tTitle: " << title << std::endl;
        std::cout << "\tGameboy type: " << gameboy_type << std::endl;
        std::cout << "\tCartridge type: " << cartridge_type << std::endl;
        std::cout << "\tROM size: " << rom_size << std::endl;
        std::cout << "\tRAM size: " << ram_size << std::endl;
        std::cout << "\tChecksum: " << (unsigned)checksum << std::endl;

        std::cout << "Start instructions: ";
        hexPrint(start_instructions[0], 2);
        std::cout << " ";
        hexPrint(start_instructions[1], 2);
        std::cout << " ";
        hexPrint(start_instructions[2], 2);
        std::cout << " ";
        hexPrint(start_instructions[3], 2, true);
    }
};

void hexPrint(unsigned value, unsigned length, bool flush) {
    std::cout << "0x" << std::setfill('0') << std::setw(length) << std::hex
              << value;

    if (flush) std::cout << std::endl;
}

std::string readROMTitle(const std::vector<opcode_t> &rom_data) {
    std::ostringstream is {};

    for (int i = 0; i < 11; ++i) {
        char c = rom_data[0x0134 + i];
        if (c == 0) break;
        is << c;
    }

    std::string title { is.str() };
    return title;
}

std::pair<register8_t, std::string> readGameboyType(
    const std::vector<opcode_t> &rom_data) {
    register8_t gameboy_type = rom_data[0x0143];

    std::string type { "Not Gameboy Color" };

    if (gameboy_type == 0x80) type = "Gameboy Color";

    return std::make_pair(gameboy_type, type);
}

std::pair<register8_t, std::string> readCartridgeType(
    const std::vector<opcode_t> &rom_data) {
    register_t cartridge_type = rom_data[0x0147];
    std::string type { "INVALID" };

    switch (cartridge_type) {
        case 0x00:
            type = "ROM ONLY";
            break;

        case 0x01:
            type = "ROM+MBC1";
            break;

        case 0x02:
            type = "ROM+MBC1+RAM";
            break;

        case 0x03:
            type = "ROM+MBC1+RAM+BATT";
            break;

        case 0x05:
            type = "ROM+MBC2";
            break;

        case 0x06:
            type = "ROM+MBC2+BATTERY";
            break;

        case 0x08:
            type = "ROM+RAM";
            break;

        case 0x09:
            type = "ROM+RAM+BATTERY";
            break;

        case 0x0B:
            type = "ROM+MMM01";
            break;

        case 0x0C:
            type = "ROM+MMM01+SRAM";
            break;

        case 0x0D:
            type = "ROM+MMM01+SRAM+BATT";
            break;

        case 0x0F:
            type = "ROM+MBC3+TIMER+BATT";
            break;

        case 0x10:
            type = "ROM+MBC3+TIMER+RAM+BATT";
            break;

        case 0x11:
            type = "ROM+MBC3";
            break;

        case 0x12:
            type = "ROM+MBC3+RAM";
            break;

        case 0x13:
            type = "ROM+MBC3+RAM+BATT";
            break;

        case 0x19:
            type = "ROM+MBC5";
            break;

        case 0x1A:
            type = "ROM+MBC5+RAM";
            break;

        case 0x1B:
            type = "ROM+MBC5+RAM+BATT";
            break;

        case 0x1C:
            type = "ROM+MBC5+RUMBLE";
            break;

        case 0x1D:
            type = "ROM+MBC5+RUMBLE+SRAM";
            break;

        case 0x1E:
            type = "ROM+MBC5+RUMBLE+SRAM+BATT";
            break;

        case 0x1F:
            type = "Pocket Camera";
            break;

        case 0xFD:
            type = "Bandai TAMA5";
            break;

        case 0xFE:
            type = "Hudson HuC-3";
            break;

        default:
            std::cerr << "INVALID CARTRIDGE TYPE" << std::endl;
    }

    return std::make_pair(cartridge_type, type);
}

std::pair<register8_t, std::string> readROMSizeType(
    const std::vector<opcode_t> &rom_data) {
    // ROM size type
    register8_t rom_type = rom_data[0x148];
    std::string rom_size { "INVALID" };

    switch (rom_type) {
        case 0x00:
            rom_size = "32KByte (2 bank)";
            break;

        case 0x01:
            rom_size = "64KByte (4 banks)";
            break;

        case 0x02:
            rom_size = "128KByte (8 banks)";
            break;

        case 0x03:
            rom_size = "256KByte (16 banks)";
            break;

        case 0x04:
            rom_size = "512KByte (32 banks)";
            break;

        case 0x05:
            rom_size = "1MByte (64 banks)  - only 63 banks used by MBC1";
            break;

        case 0x06:
            rom_size = "2MByte (128 banks) - only 125 banks used by MBC1";
            break;

        case 0x07:
            rom_size = "4MByte (256 banks)";
            break;

        case 0x52:
            rom_size = "1.1MByte (72 banks)";
            break;

        case 0x53:
            rom_size = "1.2MByte (80 banks)";
            break;

        case 0x54:
            rom_size = "1.5MByte (96 banks)";
            break;

        default:
            std::cerr << "INVALID ROM SIZE" << std::endl;
    }

    return std::make_pair(rom_type, rom_size);
}

std::pair<register_t, std::string> readROMRAMSize(
    const std::vector<opcode_t> &rom_data) {
    register8_t rom_ram_type = rom_data[0x149];
    std::string ram_size { "INVALID" };

    switch (rom_ram_type) {
        case 0x00:
            ram_size = "0 bytes";
            break;

        case 0x01:
            ram_size = "2 KBytes";
            break;

        case 0x02:
            ram_size = "8 KBytes";
            break;

        case 0x03:
            ram_size = "32 KBytes";
            break;

        default:
            std::cerr << "Invalid ROM RAM type" << std::endl;
    }

    return std::make_pair(rom_ram_type, ram_size);
}

register8_t verifyROMChecksum(const std::vector<opcode_t> &rom_data) {
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

    return header_checksum;
}

std::array<register8_t, 4> readStartInstructions(
    const std::vector<opcode_t> &rom_data) {
    std::array<register8_t, 4> instructions {};

    instructions[0] = rom_data[0x100];
    instructions[1] = rom_data[0x101];
    instructions[2] = rom_data[0x102];
    instructions[3] = rom_data[0x103];

    return instructions;
}

// TODO move to own file
ROM_Metadata readMetaData(const std::vector<opcode_t> &rom_data) {
    ROM_Metadata metadata;

    metadata.title = readROMTitle(rom_data);
    metadata.rom_size = readROMSizeType(rom_data);
    metadata.ram_size = readROMRAMSize(rom_data);
    metadata.checksum = verifyROMChecksum(rom_data);
    metadata.cartridge_type = readCartridgeType(rom_data);
    metadata.gameboy_type = readGameboyType(rom_data);
    metadata.start_instructions = readStartInstructions(rom_data);

    return metadata;
}

void Processor::readInstructions(const std::string &filename) {
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

    std::cout << "Loaded " << tmp.size() << " bytes from ROM file" << std::endl;

    readMetaData(tmp).dump();

    std::cout << "Starting with PC: ";
    hexPrint(PC->getValue(), 4);
    std::cout << std::endl;

    PC->setValue(0x0000);
    for (opcode_t val : tmp) {
        program_memory->setData(PC->getValue(), val);
        PC->increment();
    }

    std::cout << PC->getValue() << std::endl;

    std::cout << "Ending with PC: ";
    hexPrint(PC->getValue(), 4);
    std::cout << std::endl;

    // PC->setValue(PC_START);
}

void Processor::printStack(int radius) {
    register16_t start = std::max<int>(0, (int)(SP->getValue()) - radius);
    register16_t end =
        std::min<int>(SP_START + 1, (int)(SP->getValue()) + radius + 1);

    std::cout << "Stack: " << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    byte_t value;
    while (start != end) {
        value = stack->getData(start);

        std::cout << "\t";
        hexPrint(start, 4);
        std::cout << ": ";
        hexPrint(value, 2);

        if (start == SP->getValue()) {
            std::cout << " <---";
        }

        std::cout << std::endl;
        ++start;
    }
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    std::cout << "Done printing stack" << std::endl;
}

void Processor::printProgramMemory(int radius) {
    register16_t start = std::max<int>(0, (int)(PC->getValue()) - radius);
    register16_t end =
        std::min<int>(PC_MAX + 1, (int)(PC->getValue()) + radius + 1);

    std::cout << "Program memory: " << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    byte_t value;
    while (start != end) {
        value = program_memory->getData(start);

        std::cout << "\t";
        hexPrint(start, 4);
        std::cout << ": ";
        hexPrint(value, 2);

        if (start == PC->getValue()) {
            std::cout << " <---";
        }

        std::cout << std::endl;
        ++start;
    }
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    std::cout << "Done printing program memory" << std::endl;
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