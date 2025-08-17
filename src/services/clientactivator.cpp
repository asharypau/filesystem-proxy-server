#include "clientactivator.h"
#include "../network/protocol.h"
#include "../network/protocoltypes.h"
#include "../network/serialization.h"
#include "clientmapper.h"
#include "idgenerator.h"
#include <format>

ClientActivator::ClientActivator(ClientRegistry& client_registry)
    : _client_registry(client_registry)
{
}

boost::asio::awaitable<Client::id_t> ClientActivator::activate(Network::IClientConnection* connection)
{
    Network::Protocol::Headers client_activation_headers = co_await connection->read_headers();
    if (client_activation_headers.type != Network::Protocol::Type::Activation)
    {
        throw ClientActivationException(std::format("Invalid request type {} during activation",
                                                    Network::Protocol::to_string(client_activation_headers.type)));
    }

    Network::Protocol::data_t client_activation_data = co_await connection->read(client_activation_headers.data_size);
    ClientActivation client_activation = Network::Serializer::deserialize<ClientActivation>(client_activation_data);

    if (_client_registry.exists(client_activation.id))
    {
        _client_registry.activate(client_activation.id, connection);
    }
    else
    {
        client_activation.id = IdGenerator::generate(client_activation.root);
        _client_registry.add(ClientMapper::map(client_activation, connection));
    }

    Network::Protocol::data_t updated_client_activation_data = Network::Serializer::serialize(client_activation);
    Network::Protocol::Headers client_activated_headers
        = Network::Protocol::Headers::make(updated_client_activation_data.size(), Network::Protocol::Type::Activated);

    co_await connection->write_headers(client_activated_headers);
    co_await connection->write(updated_client_activation_data);

    co_return client_activation.id;
}

void ClientActivator::deactivate(Client::id_t client_id)
{
    if (_client_registry.exists(client_id))
    {
        _client_registry.deactivate(client_id);
    }
}
