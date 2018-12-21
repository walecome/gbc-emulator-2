#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>
#define BYTE unsigned char

std::string get_byte_string(BYTE byte)
{

    std::stringstream ss;

    ss << "0x" << std::setfill('0') << std::setw(2)
       << std::hex << std::uppercase << (int)byte;

    std::string ret;

    ss >> ret;

    return ret;
}

std::string translate_byte(BYTE byte)
{
    std::string translated_byte;

    switch (byte)
    {
    case 0x00:
        translated_byte = "NOP";
        break;

    default:
        // std::cerr << "Unrecognized op: " << get_byte_string(byte) << std::endl;
        // throw 1;
        translated_byte = "UNSUPPORTED";
    }

    return translated_byte;
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

    std::vector<unsigned char> bytes = readFile(filename.c_str());

    unsigned int nop_count = 0;
    unsigned int total_count = 0;

    for (int i = 0; i < bytes.size(); ++i)
    {
        // std::cout << "Got byte: " << get_byte_string(bytes[i]) << std::endl;

        ++total_count;
        if (translate_byte(bytes[i]) == "NOP")
            ++nop_count;
    }

    std::cout << "NOP count: " << nop_count << std::endl;
    std::cout << "Total count: " << total_count << std::endl;

    return 0;
}