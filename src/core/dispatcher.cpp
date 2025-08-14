#include "dispatcher.h"
#include "../Handlers/clienthandler.h"
#include "../utils/logger.h"

Dispatcher::Dispatcher(ClientRegistry& client_registry, ClientSubscriptionsManager& client_subscriptions_manager)
    : _handlers()
{
    _handlers.emplace(static_cast<uint16_t>(Network::Protocol::Type::Client),
                      std::make_unique<ClientHandler>(client_registry, client_subscriptions_manager));
}

boost::asio::awaitable<void> Dispatcher::dispatch(Client::id_t client_id, const Network::Protocol::Headers& headers)
{
    auto it = _handlers.find(static_cast<uint16_t>(headers.type));
    if (it != _handlers.end())
    {
        co_await it->second->handle(client_id, headers);
    }

    Logger::error("Invalid protocol type: {}", Network::Protocol::to_string(headers.type));
}
