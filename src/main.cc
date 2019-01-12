#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <assert.h>

// User headers
#include "Instruction.hh"
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
    std::cout << "Testing 8bit register addition..." << std::endl;

    p.setValueA(0x03);
    p.setValueB(0x14);

    std::cout << "A: " << std::hex << unsigned(p.getValueA()) << std::endl;
    std::cout << "B: " << std::hex << unsigned(p.getValueB()) << std::endl;

    std::cout << "Doing addition of A and B..." << std::endl;

    p.addRegisters(p.getA(), p.getB());

    std::cout << "A: " << std::hex << unsigned(p.getValueA()) << std::endl;

    assert(p.getValueA() == (register8_t)(0x03 + 0x14));

    std::cout << "Testing 16bit register addition..." << std::endl;

    register16_t af_val = 0xABCD;
    register16_t bc_val = 0xB00F;
    register16_t expected_value = af_val + bc_val;

    p.setValueAF(af_val);
    p.setValueBC(bc_val);

    std::cout << "AF: " << std::hex << p.getValueAF() << std::endl;
    std::cout << "BC: " << std::hex << p.getValueBC() << std::endl;

    std::cout << "Doing addition of AF and BC..." << std::endl;

    p.addRegisters(p.getAF(), p.getBC());

    std::cout << "AF: " << std::hex << p.getValueAF() << std::endl;

    assert(p.getValueAF() == expected_value);
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
    p.setValueA(0xEB);
    std::cout << "AF value: " << std::hex << (int)p.getValueAF() << std::endl;

    std::cout << "Setting AF to 0xBEEF" << std::endl;
    p.setValueAF(0xBEEF);
    std::cout << "AF value: " << std::hex << p.getAF() << std::endl;
    std::cout << "A value: " << std::hex << (int)p.getValueA() << std::endl;
    std::cout << "F value: " << std::hex << (int)p.getValueF() << std::endl;

    std::cout << "Running tests..." << std::endl;
    tests(p);

    std::cout << "Printing processor" << std::endl;
    p.print();

    std::cout
        << "Done" << std::endl;

    std::cout << *p.getA() << std::endl;
    std::cout << *p.getAF() << std::endl;

    return 0;
}