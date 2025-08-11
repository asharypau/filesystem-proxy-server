#ifndef NETWORK_REQUESTTYPE_H
#define NETWORK_REQUESTTYPE_H

#include <cstdint>

namespace Network
{
enum class RequestType : uint16_t
{
    None,
    ClientList,
};
} // namespace Network

#endif // NETWORK_REQUESTTYPE_H
