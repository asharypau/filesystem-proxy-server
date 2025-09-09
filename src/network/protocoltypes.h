#ifndef NETWORK_PROTOCOLTYPES_H
#define NETWORK_PROTOCOLTYPES_H

#include <cstddef>
#include <vector>

namespace Network
{
namespace Protocol
{
using data_size_t = std::size_t;
using data_t = std::vector<unsigned char>;
} // namespace Protocol
} // namespace Network

#endif // NETWORK_PROTOCOLTYPES_H
