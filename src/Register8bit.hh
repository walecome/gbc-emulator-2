#pragma once
#include <string>
#include "Constants.hh"

class Register8bit
{
public:
  Register8bit(std::string _name) : name{_name} {};
  register8_t getValue();
  void setValue(register8_t value);
  void increment();
  void decrement();

private:
  std::string name;

  register8_t value{0xFF};
};