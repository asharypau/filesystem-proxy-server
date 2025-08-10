#include "session.h"
#include "../models/clientcontext.h"
#include "../models/clientinput.h"
#include "../models/clientpayload.h"
#include "../services/clientmapper.h"

Session::Session(Network::Tcp::Socket socket,
                 ClientRegistry& client_registry,
                 ClientSubscriptionsManager& client_subscriptions_manager)
    : _socket(std::move(socket))
    , _client_registry(client_registry)
    , _client_subscriptions_manager(client_subscriptions_manager)
    , _client_id()
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
    ClientInput client_input = co_await _socket.read_async<ClientInput>();
    if (_client_registry.exists(client_input.id))
    {
        _client_registry.activate(client_input.id, this);
    }
    else
    {
        ClientContext client_context = ClientMapper::map(client_input, this);
        _client_registry.add(std::move(client_context));
    }

    _client_id = client_input.id;
}

boost::asio::awaitable<void> Session::write_clients()
{
    std::vector<ClientContext> client_contexts = _client_registry.get_all();
    std::vector<ClientPayload> client_payloads;
    client_payloads.reserve(client_contexts.size());

    for (const ClientContext& client_context : client_contexts)
    {
        client_payloads.emplace_back(ClientMapper::map(client_context));
    }

    co_await _socket.write_async(client_payloads);
    _client_subscriptions_manager.subscribe("client_registry_update", _client_id);
}
