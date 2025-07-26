#include "sessionmanager.h"

SessionManager::SessionManager(boost::asio::io_context& context,
                               ClientRegistry& client_registry,
                               ClientSubscriptionsManager& client_subscriptions_manager)
    : _context(context)
    , _client_registry(client_registry)
    , _client_subscriptions_manager(client_subscriptions_manager)
    , _sessions()
{
}

void SessionManager::run_new(Network::Tcp::Socket socket)
{
    _sessions.emplace_back(std::move(socket), _client_registry, _client_subscriptions_manager);

    boost::asio::co_spawn(_context, _sessions.back().run([this](Session* session) { stop(session); }), boost::asio::detached);
}

void SessionManager::stop(Session* session)
{
    _sessions.remove_if([session](const Session& current) { return std::addressof(current) == session; });
}
