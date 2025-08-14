#include "clientactivator.h"
#include "../network/protocol.h"
#include "../network/protocoltypes.h"
#include "../network/serialization.h"
#include "clientmapper.h"
#include "idgenerator.h"

ClientActivator::ClientActivator(ClientRegistry& client_registry)
    : _client_registry(client_registry)
{
}

boost::asio::awaitable<Client::id_t> ClientActivator::activate(Network::IClientConnection* connection)
{
    Network::Protocol::Headers headers = co_await connection->read_headers();
    if (headers.type != Network::Protocol::Type::Activation)
    {
        throw ClientActivationException("Attempt to start a session without activation");
    }

    Network::Protocol::data_t data = co_await connection->read(headers.data_size);
    ClientActivationData activation_data = Network::Serializer::deserialize<ClientActivationData>(data);

    if (_client_registry.exists(activation_data.id))
    {
        _client_registry.activate(activation_data.id, connection);
        activation_data.id = IdGenerator::generate(activation_data.root);
    }
    else
    {
        ClientContext client_context = ClientMapper::map(activation_data, connection);
        _client_registry.add(std::move(client_context));
    }

    co_return activation_data.id;
}

void ClientActivator::deactivate(Client::id_t client_id)
{
    if (_client_registry.exists(client_id))
    {
        _client_registry.deactivate(client_id);
    }
}
