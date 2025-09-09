#ifndef IHANDLER_H
#define IHANDLER_H

#include "../models/client.h"
#include "../network/protocol.h"
#include <boost/asio.hpp>

class IHandler
{
public:
    virtual ~IHandler() = default;

    virtual boost::asio::awaitable<void> handle(const Client::id_t& client_id, const Network::Protocol::Headers& headers)
        = 0;
};

#endif // IHANDLER_H
