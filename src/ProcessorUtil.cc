#include "Processor.hh"
#include <iomanip>

void hexPrint(unsigned value, unsigned length)
{
    std::cout << "0x" << std::setfill('0')
              << std::setw(length) << std::hex << value;
}

/**
    Reads instructions (and data) into the program memory vector given
    binary filename
*/
void Processor::readInstructions(const char *filename)
{
    // open the file as binary
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode
    file.unsetf(std::ios::skipws);

    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    program_memory.reserve(fileSize);

    // read the data:
    program_memory.insert(program_memory.begin(),
                          std::istream_iterator<opcode_t>(file),
                          std::istream_iterator<opcode_t>());
}

/**
   Prints the stack content from stack to end.
*/
void Processor::printStack(register16_t start, register16_t end)
{
    std::cout << "Stack: " << std::endl;
    byte_t value;
    while (start != end)
    {
        value = stack->getData(start);

        std::cout << "\t";
        hexPrint(start, 4);
        std::cout << ": ";
        hexPrint(value, 2);
        std::cout << std::endl;
        ++start;
    }

    std::cout << "Done printing stack" << std::endl;
}

void Processor::print()
{
    std::cout << "Printing processor" << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    std::cout << "\t" << *A << "\t\t"
              << *B << std::endl
              << "\t" << *D << "\t\t"
              << *C << std::endl
              << "\t" << *E << "\t\t"
              << *F << std::endl
              << "\t" << *H << "\t\t"
              << *L << std::endl
              << std::endl;

    std::cout << "\t" << *AF << "\t" << *BC << std::endl;
    std::cout << "\t" << *DE << "\t" << *HL << std::endl
              << std::endl;
    std::cout << "\t" << *stack_pointer << "\t";
    std::cout << *program_counter << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
}