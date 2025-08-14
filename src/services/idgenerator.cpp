#include "idgenerator.h"
#include <chrono>
#include <functional>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

std::string IdGenerator::generate(const std::string& base)
{
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    std::hash<std::string> hasher;
    auto hash = static_cast<uint64_t>(hasher(base));

    uint64_t part1 = now;
    uint64_t part2 = hash;

    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(part1 >> 32) << "-" << std::setw(4)
        << static_cast<uint16_t>(part1 >> 16) << "-" << std::setw(4) << static_cast<uint16_t>(part1) << "-"
        << std::setw(4) << static_cast<uint16_t>(part2 >> 48) << "-" << std::setw(12)
        << (part2 & 0x0000FFFFFFFFFFFFULL);

    return oss.str();
}
