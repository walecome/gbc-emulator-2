#pragma once

#include <iomanip>
#include <iostream>
#include <utility>

#include "Constants.hh"

namespace Util {
std::ostream &operator<<(std::ostream &os,
                         const std::pair<register8_t, std::string> &p);
void hexPrint(unsigned value, unsigned length, bool flush = false);

}  // namespace Util