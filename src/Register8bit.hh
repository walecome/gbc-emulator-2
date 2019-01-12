#pragma once
#include <string>
#include "Constants.hh"
#include <iostream>

class Register8bit
{
public:
  Register8bit(std::string _name) : name{_name} {};
  register8_t getValue() const;
  void setValue(register8_t value);
  std::string getName() const { return name; }
  void increment();
  void decrement();

  friend std::ostream &operator<<(std::ostream &, const Register8bit &);

private:
  std::string name;

  register8_t value{0xFF};
};