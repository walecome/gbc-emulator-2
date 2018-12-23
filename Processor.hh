#include <iterator>
#include <fstream>
#include <vector>
#include "Constants.hh"

class Processor
{
  public:
    Processor();

    /**
        Reads instructions (and data) into the instruction vector given
        binary filename
    */
    void readInstructions(const char *filename);

  private:
    unsigned int stack_pointer{0};
    std::vector<opcode_t> instructions;
};