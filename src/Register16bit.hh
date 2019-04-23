#pragma once

// System headers
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

// User headers
#include "Constants.hh"
#include "Register8bit.hh"

class Register16bit {
   public:
    Register16bit(const std::string &name)
        : name { name },
          high { new Register8bit(name + "_high") },
          low { new Register8bit(name + "_low") } {}

    Register16bit(const std::string &_name, ptr<Register8bit> high,
                  ptr<Register8bit> low)
        : name { _name }, high { high }, low { low } {}

    // Weffc++
    Register16bit(const Register16bit &) = delete;
    void operator=(const Register16bit &) = delete;

    register16_t getValue() const;
    void setValue(register16_t value);
    std::string getName() const { return name; }
    /**
     *   Increment register value by one.
     */
    void increment();

    /**
     *   Decrement register value by one.
     */
    void decrement();
    ptr<Register8bit> getHighRegister() const;
    ptr<Register8bit> getLowRegister() const;

    friend std::ostream &operator<<(std::ostream &, const Register16bit &);

   private:
    const std::string name;
    ptr<Register8bit> high, low;
};