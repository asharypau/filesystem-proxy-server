#ifndef SESSION_H
#define SESSION_H

#include "../network/tcp/socket.h"
#include "../services/clientregistry.h"
#include "../services/clientsubscriptionsmanager.h"
#include <boost/asio.hpp>

class Session
{
public:
    Session(Network::Tcp::Socket socket,
            ClientRegistry& client_registry,
            ClientSubscriptionsManager& client_subscriptions_manager);
    ~Session();

    // Disallow movement  and assignment
    Session(Session&& other) = delete;
    Session& operator=(Session&& other) = delete;

    // Disallow copying and assignment
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    //boost::asio::awaitable<void> request_update();

    template<class TCompletionHandler>
    boost::asio::awaitable<void> run(TCompletionHandler completion_handler)
    {
        co_await internal_run();
        completion_handler(this);
    }

private:
    boost::asio::awaitable<void> internal_run();
    boost::asio::awaitable<void> activate_client();
    boost::asio::awaitable<void> write_clients();

private:
    Network::Tcp::Socket _socket;
    ClientRegistry& _client_registry;
    ClientSubscriptionsManager& _client_subscriptions_manager;
    Client::id_t _client_id;
};

#endif // SESSION_H
