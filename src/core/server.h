#ifndef SERVER_H
#define SERVER_H

#include "../network/tcp/acceptor.h"
#include "../services/clientregistry.h"
#include "sessionmanager.h"
#include <boost/asio.hpp>
#include <string>

class Server
{
public:
    Server(unsigned short port, std::string host);

    int run();

private:
    boost::asio::awaitable<void> accept();

private:
    boost::asio::io_context _context;
    Network::Tcp::Acceptor _acceptor;
    ClientRegistry _client_registry;
    ClientSubscriptionsManager _client_subscriptions_manager;
    SessionManager _session_manager;
};

#endif // SERVER_H
