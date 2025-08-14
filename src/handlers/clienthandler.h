#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include "../models/client.h"
#include "../services/clientregistry.h"
#include "../services/clientsubscriptionsmanager.h"
#include "ihandler.h"
#include <boost/asio.hpp>

class ClientHandler : public IHandler
{
public:
    ClientHandler(ClientRegistry& client_registry, ClientSubscriptionsManager& client_subscriptions_manager);

    boost::asio::awaitable<void> handle(Client::id_t client_id, const Network::Protocol::Headers& headers) override;

private:
    boost::asio::awaitable<void> handle_get_all(Client::id_t client_id);

    ClientRegistry& _client_registry;
    ClientSubscriptionsManager& _client_subscriptions_manager;
};

#endif // CLIENTHANDLER_H
