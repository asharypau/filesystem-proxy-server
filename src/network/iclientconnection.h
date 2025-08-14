#ifndef NETWORK_ICLIENTCONNECTION_H
#define NETWORK_ICLIENTCONNECTION_H

#include "protocol.h"
#include <boost/asio.hpp>

namespace Network
{
class IClientConnection
{
public:
    virtual ~IClientConnection() = default;

    virtual boost::asio::awaitable<Network::Protocol::Headers> read_headers() = 0;
    virtual boost::asio::awaitable<Network::Protocol::data_t> read(Network::Protocol::data_size_t data_size) = 0;
    virtual boost::asio::awaitable<void> write_headers(const Network::Protocol::Headers& headers) = 0;
    virtual boost::asio::awaitable<void> write(const Network::Protocol::data_t& data) = 0;
};
} // namespace Network

#endif // NETWORK_ICLIENTCONNECTION_H
