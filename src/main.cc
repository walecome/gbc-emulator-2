#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>
#include "Instruction.hh"
#include "Opcode.hh"
#include "Constants.hh"
#include "Processor.hh"
#include <iostream>

std::string get_byte_string(byte_t byte)
{

    std::stringstream ss;

    ss << "0x" << std::setfill('0') << std::setw(2)
       << std::hex << std::uppercase << (int)byte;

    std::string ret;

    ss >> ret;

    return ret;
}

std::vector<unsigned char> readFile(const char *filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<unsigned char> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<unsigned char>(file),
               std::istream_iterator<unsigned char>());

    return vec;
}

int main()
{
    std::string filename = "roms/pokemon_silver.gbc";

    std::cout << "Creating processor..." << std::endl;
    Processor p{};
    std::cout << "Creating reading instructions from binary file..."
              << std::endl;
    p.readInstructions(filename.c_str());

    std::cout << "AF value: " << std::hex << p.getAFValue() << std::endl;
    p.setAValue(0xEB);
    std::cout << "AF value: " << std::hex << p.getAFValue() << std::endl;

    std::cout << "Done" << std::endl;

    return 0;
}