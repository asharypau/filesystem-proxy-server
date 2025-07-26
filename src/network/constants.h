#ifndef NETWORK_CONSTANTS_H
#define NETWORK_CONSTANTS_H

#include <vector>

namespace Network
{
using data_size_t = std::size_t;
using data_t = std::vector<unsigned char>;

static constexpr std::size_t DATA_SIZE = sizeof(data_size_t);
} // namespace Network

#endif // NETWORK_CONSTANTS_H
