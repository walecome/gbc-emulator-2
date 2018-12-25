#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>

// User headers
#include "Instruction.hh"
#include "Opcode.hh"
#include "Constants.hh"
#include "Processor.hh"

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

void tests(Processor &p)
{
    std::cout << "Testing register addition..." << std::endl;

    p.setA(0x03);
    p.setB(0x14);

    std::cout << "A: " << std::hex << p.getA() << std::endl;
    std::cout << "B: " << std::hex << p.getB() << std::endl;

    std::cout << "Doing addition of A and B..." << std::endl;

    p.addRegisters(A, B);
}

int main()
{
    std::string filename = "roms/pokemon_silver.gbc";

    std::cout << "Creating processor..." << std::endl;
    Processor p{};
    std::cout << "Creating reading instructions from binary file..."
              << std::endl;
    p.readInstructions(filename.c_str());

    std::cout << "AF value: " << std::hex << p.getAF() << std::endl;
    std::cout << "Setting A to 0xEB" << std::endl;
    p.setA(0xEB);
    std::cout << "AF value: " << std::hex << (int)p.getAF() << std::endl;

    std::cout << "Setting AF to 0xBEEF" << std::endl;
    p.setAF(0xBEEF);
    std::cout << "AF value: " << std::hex << p.getAF() << std::endl;
    std::cout << "A value: " << std::hex << (int)p.getA() << std::endl;
    std::cout << "F value: " << std::hex << (int)p.getF() << std::endl;

    std::cout << "Done" << std::endl;

    return 0;
}