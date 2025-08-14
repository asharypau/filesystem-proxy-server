#include "server.h"
#include "../network/tcp/socket.h"
#include "../utils/logger.h"

Server::Server(unsigned short port, std::string host)
    : _context()
    , _acceptor(port, host, _context)
    , _client_registry()
    , _client_subscriptions_manager(_client_registry)
    , _client_activator(_client_registry)
    , _dispatcher(_client_registry, _client_subscriptions_manager)
    , _session_manager(_context, _client_registry, _client_subscriptions_manager, _client_activator, _dispatcher)
{
}

int Server::run()
{
    try
    {
        Logger::info("Server started");
        boost::asio::co_spawn(_context,
                              accept(),
                              [this](std::exception_ptr eptr)
                              {
                                  if (eptr)
                                  {
                                      std::rethrow_exception(eptr);
                                  }
                              });
        int result = _context.run();
        Logger::info("Server stopped");

        return result;
    }
    catch (const std::exception& ex)
    {
        Logger::error("Server stopped. Reason: {}", ex.what());

        return -1;
    }
}

boost::asio::awaitable<void> Server::accept()
{
    while (true)
    {
        Network::Tcp::Socket socket = co_await _acceptor.accept();
        _session_manager.run_new(std::move(socket));
    }
}
