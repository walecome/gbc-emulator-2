#include "Utility.hh"

namespace Util {

std::ostream &operator<<(std::ostream &os,
                         const std::pair<register8_t, std::string> &p) {
    os << "(" << (unsigned)p.first;
    os << ", " << p.second << ")";

    return os;
}

void hexPrint(unsigned value, unsigned length, bool flush) {
    std::cout << "0x" << std::setfill('0') << std::setw(length)
              << std::uppercase << std::hex << value;

    // Reset std::cout to decimal after std::hex
    std::cout << std::dec;

    if (flush) std::cout << std::endl;
}

std::string hexString(unsigned value, unsigned length) {
    std::stringstream ss {};

    ss << "0x" << std::setfill('0') << std::setw(length) << std::uppercase
       << std::hex << value;

    return ss.str();
}
}  // namespace Util