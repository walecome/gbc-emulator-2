#include "Utility.hh"

namespace Util {

void hexPrint(unsigned value, unsigned length, bool flush) {
    std::cout << "0x" << std::setfill('0') << std::setw(length) << std::hex
              << value;

    if (flush) std::cout << std::endl;
}

std::ostream &operator<<(std::ostream &os,
                         const std::pair<register8_t, std::string> &p) {
    os << "(" << (unsigned)p.first;
    os << ", " << p.second << ")";

    return os;
}
}  // namespace Util