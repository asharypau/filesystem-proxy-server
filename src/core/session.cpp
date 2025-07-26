#include "session.h"
#include "../models/client.h"
#include "../models/clientmetadata.h"

Session::Session(Network::Tcp::Socket socket,
                 ClientRegistry& client_registry,
                 ClientSubscriptionsManager& client_subscriptions_manager)
    : _socket(std::move(socket))
    , _client_registry(client_registry)
    , _client_subscriptions_manager(client_subscriptions_manager)
    , _client_id(0)
{
}

Session::~Session()
{
    _client_registry.deactivate(_client_id);
    _socket.close();
}

boost::asio::awaitable<void> Session::internal_run()
{
    co_await activate_client();
    co_await write_clients();
}

boost::asio::awaitable<void> Session::activate_client()
{
    ClientMetadata client_metadata = co_await _socket.read_async<ClientMetadata>();
    if (_client_registry.exists(client_metadata.id))
    {
        _client_registry.activate(client_metadata.id, this);
    }
    else
    {
        Client client{client_metadata, this};
        _client_registry.add(std::move(client));
    }

    _client_id = client_metadata.id;

    co_return;
}

boost::asio::awaitable<void> Session::write_clients()
{
    std::vector<Client> clients = _client_registry.get_all();
    std::vector<ClientMetadata> clients_metadata;
    clients_metadata.reserve(clients.size());

    for (const Client& client : clients)
    {
        clients_metadata.push_back(static_cast<ClientMetadata>(client));
    }

    co_await _socket.write_async(clients_metadata);
}
