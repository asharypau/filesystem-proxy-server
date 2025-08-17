#include "clientrequesthandler.h"
#include "../network/protocol.h"
#include "../network/serialization.h"
#include "../services/clientmapper.h"
#include "../utils/logger.h"

ClientRequestHandler::ClientRequestHandler(ClientRegistry& client_registry,
                                           ClientSubscriptionsManager& client_subscriptions_manager)
    : _client_registry(client_registry)
    , _client_subscriptions_manager(client_subscriptions_manager)
{
}

boost::asio::awaitable<void> ClientRequestHandler::handle(const Client::id_t& client_id,
                                                          const Network::Protocol::Headers& headers)
{
    switch (headers.method)
    {
    case Network::Protocol::Action::GetClients:
        co_await handle_get_clients_action(client_id);
        break;

    case Network::Protocol::Action::None:
    default:
        Logger::error("Invalid action type: {}", Network::Protocol::to_string(headers.method));
        break;
    }
}

boost::asio::awaitable<void> ClientRequestHandler::handle_get_clients_action(const Client::id_t& client_id)
{
    std::vector<ClientContext> client_contexts = _client_registry.get_all();
    std::vector<ClientPayload> client_payloads;
    client_payloads.reserve(client_contexts.size());

    for (const ClientContext& client_context : client_contexts)
    {
        client_payloads.emplace_back(ClientMapper::map(client_context));
    }

    Network::Protocol::data_t data = Network::Serializer::serialize(client_payloads);
    ClientContext current_client = _client_registry.get(client_id);

    Network::Protocol::Headers headers = Network::Protocol::Headers::make(data.size(),
                                                                          Network::Protocol::Type::ClientRequest,
                                                                          Network::Protocol::Action::GetClients);
    co_await current_client.connection->write_headers(headers);
    co_await current_client.connection->write(data);

    _client_subscriptions_manager.subscribe("client_registry_update", client_id);
}
