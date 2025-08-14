#ifndef CLIENTACTIVATOR_H
#define CLIENTACTIVATOR_H

#include "../models/client.h"
#include "../network/iclientconnection.h"
#include "../services/clientregistry.h"
#include <boost/asio.hpp>

class ClientActivator
{
public:
    explicit ClientActivator(ClientRegistry& client_registry);

    boost::asio::awaitable<Client::id_t> activate(Network::IClientConnection* connection);
    void deactivate(Client::id_t client_id);

private:
    ClientRegistry& _client_registry;
};

#endif // CLIENTACTIVATOR_H
