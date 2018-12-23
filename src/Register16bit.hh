#pragma once
#include <string>
#include "Constants.hh"
#include "Register8bit.hh"

class Register16bit
{
public:
  Register16bit(std::string name);
  Register16bit(std::string _name, Register8bit *_high, Register8bit *_low) : name{_name}, high{_high}, low{_low} {}
  ~Register16bit();
  register16_t getValue();
  void setValue(register16_t value);
  void increment();
  void decrement();
  Register8bit *getHighRegister();
  Register8bit *getLowRegister();

private:
  std::string name;
  Register8bit *high;
  Register8bit *low;

  // Used for deleting resources that were allocated using the name only constructor
  bool free_self = false;
};