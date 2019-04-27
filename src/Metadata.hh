#pragma once

// System headers
#include <iomanip>
#include <vector>

// User headers
#include "Constants.hh"
#include "Processor.hh"
#include "Utility.hh"

namespace Util {

struct ROM_Metadata {
    ROM_Metadata(const std::vector<opcode_t> &rom_data);
    std::string title;
    std::pair<register8_t, std::string> gameboy_type;
    std::pair<register8_t, std::string> cartridge_type;
    std::pair<register8_t, std::string> rom_size;
    std::pair<register8_t, std::string> ram_size;
    register8_t checksum;

    std::array<register8_t, 4> start_instructions;

    void dump();
};

std::string readROMTitle(const std::vector<opcode_t> &rom_data);

std::pair<register8_t, std::string> readGameboyType(
    const std::vector<opcode_t> &rom_data);

std::pair<register8_t, std::string> readCartridgeType(
    const std::vector<opcode_t> &rom_data);

std::pair<register8_t, std::string> readROMSizeType(
    const std::vector<opcode_t> &rom_data);

std::pair<register_t, std::string> readROMRAMSize(
    const std::vector<opcode_t> &rom_data);

register8_t verifyROMChecksum(const std::vector<opcode_t> &rom_data);

std::array<register8_t, 4> readStartInstructions(
    const std::vector<opcode_t> &rom_data);

ROM_Metadata readMetaData(const std::vector<opcode_t> &rom_data);

}  // namespace Util