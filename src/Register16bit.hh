#pragma once
#include <string>
#include "Constants.hh"
#include "Register8bit.hh"

class Register16bit
{
public:
  Register16bit(std::string _name, Register8bit *_high, Register8bit *_low) : name{_name}, high{_high}, low{_low} {}
  register16_t getValue();
  void setValue(register16_t value);
  void increment();
  void decrement();

private:
  std::string name;
  Register8bit *high;
  Register8bit *low;
};