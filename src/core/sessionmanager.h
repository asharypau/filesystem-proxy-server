#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "../network/tcp/socket.h"
#include "../services/clientactivator.h"
#include "../services/clientregistry.h"
#include "../services/clientsubscriptionsmanager.h"
#include "session.h"
#include <boost/asio.hpp>
#include <list>

class SessionManager
{
public:
    SessionManager(boost::asio::io_context& context,
                   ClientRegistry& client_registry,
                   ClientSubscriptionsManager& client_subscriptions_manager,
                   ClientActivator& client_activator,
                   Dispatcher& dispatcher);

    // Disallow movement  and assignment
    SessionManager(SessionManager&& other) = delete;
    SessionManager& operator=(SessionManager&& other) = delete;

    // Disallow copying and assignment
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

    void run_new(Network::Tcp::Socket socket);

private:
    void stop(Session* session);

private:
    boost::asio::io_context& _context;
    ClientRegistry& _client_registry;
    ClientSubscriptionsManager& _client_subscriptions_manager;
    ClientActivator& _client_activator;
    Dispatcher& _dispatcher;
    std::list<Session> _sessions;
};

#endif // SESSIONMANAGER_H
