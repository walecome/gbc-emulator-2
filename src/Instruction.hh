#include <string>

class Instruction
{
public:
  Instruction(unsigned char opcode, int instruction_length, std::string name) : _opcode(opcode), _instruction_length(instruction_length), _name(name) {}

private:
  unsigned char _opcode;
  int _instruction_length;
  std::string _name;
};