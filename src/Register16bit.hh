#pragma once
#include <string>
#include "Register8bit.hh"
#include <iostream>
#include <iomanip>

#include "Constants.hh"

class Register16bit
{
public:
  Register16bit(const std::string &name)
      : name{name},
        high{new Register8bit(name + "_high")},
        low{new Register8bit(name + "_low")},
        free_self{true} {}

  Register16bit(const std::string &_name,
                Register8bit *_high, Register8bit *_low)
      : name{_name}, high{_high}, low{_low} {}

  ~Register16bit();

  // Weffc++
  Register16bit(const Register16bit &) = delete;
  void operator=(const Register16bit &) = delete;

  register16_t getValue() const;
  void setValue(register16_t value);
  std::string getName() const { return name; }
  void increment();
  void decrement();
  Register8bit *getHighRegister();
  Register8bit *getLowRegister();

  friend std::ostream &operator<<(std::ostream &, const Register16bit &);

private:
  std::string name;
  Register8bit *high;
  Register8bit *low;

  // Used for deleting resources that were allocated using the name only constructor
  bool free_self = false;
};