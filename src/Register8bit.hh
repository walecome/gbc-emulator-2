#pragma once
#include <string>
#include <iostream>
#include <iomanip>

#include "Constants.hh"

class Register8bit
{
public:
  Register8bit(const std::string &name) : name{name} {};

  // Weffc++
  Register8bit(const Register8bit &) = delete;
  void operator=(const Register8bit &) = delete;

  register8_t getValue() const;
  void setValue(register8_t value);
  std::string getName() const { return name; }
  void increment();
  void decrement();

  friend std::ostream &operator<<(std::ostream &, const Register8bit &);

private:
  const std::string name;

  register8_t value{0xFF};
};