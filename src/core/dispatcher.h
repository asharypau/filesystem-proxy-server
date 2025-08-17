#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "../handlers/ihandler.h"
#include "../models/client.h"
#include "../network/protocol.h"
#include "../services/clientregistry.h"
#include "../services/clientsubscriptionsmanager.h"
#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>

class Dispatcher
{
public:
    Dispatcher(ClientRegistry& client_registry, ClientSubscriptionsManager& client_subscriptions_manager);

    boost::asio::awaitable<void> dispatch(const Client::id_t& client_id, const Network::Protocol::Headers& headers);

private:
    std::unordered_map<uint16_t, std::unique_ptr<IHandler>> _handlers;
};

#endif // DISPATCHER_H
