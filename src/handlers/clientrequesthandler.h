#ifndef CLIENTREQUESTHANDLER_H
#define CLIENTREQUESTHANDLER_H

#include "../models/client.h"
#include "../services/clientregistry.h"
#include "../services/clientsubscriptionsmanager.h"
#include "ihandler.h"
#include <boost/asio.hpp>

class ClientRequestHandler : public IHandler
{
public:
    ClientRequestHandler(ClientRegistry& client_registry, ClientSubscriptionsManager& client_subscriptions_manager);

    boost::asio::awaitable<void> handle(const Client::id_t& client_id,
                                        const Network::Protocol::Headers& headers) override;

private:
    boost::asio::awaitable<void> handle_get_clients_action(const Client::id_t& client_id);

    ClientRegistry& _client_registry;
    ClientSubscriptionsManager& _client_subscriptions_manager;
};

#endif // CLIENTREQUESTHANDLER_H
