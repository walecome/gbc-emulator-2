#pragma once
#include <vector>
#include "Constants.hh"

class Memory
{
public:
  Memory(const register16_t size);
  void setData(const register16_t address, byte_t data);
  byte_t getData(const register16_t address);

  // Use only for testing!!
  std::vector<byte_t> &_get_mem_vector()
  {
    return memory;
  }

private:
  std::vector<byte_t> memory{};
};
