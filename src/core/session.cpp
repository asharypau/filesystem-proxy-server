#include "session.h"
#include "../network/operationexception.h"
#include "../network/serialization.h"

Session::Session(Network::Tcp::Socket socket, ClientActivator& client_activator, Dispatcher& dispatcher)
    : _socket(std::move(socket))
    , _client_activator(client_activator)
    , _dispatcher(dispatcher)
    , _client_id()
{
}

Session::~Session()
{
    if (_socket.is_open())
    {
        stop();
    }
}

boost::asio::awaitable<Network::Protocol::Headers> Session::read_headers()
{
    Network::Protocol::data_t data = co_await read(Network::Protocol::Headers::SIZE);

    co_return Network::Serializer::deserialize<Network::Protocol::Headers>(data);
}

boost::asio::awaitable<Network::Protocol::data_t> Session::read(Network::Protocol::data_size_t data_size)
{
    co_return co_await _socket.read(data_size);
}

boost::asio::awaitable<void> Session::write_headers(const Network::Protocol::Headers& headers)
{
    Network::Protocol::data_t data = Network::Serializer::serialize(headers);
    co_await write(data);
}

boost::asio::awaitable<void> Session::write(const Network::Protocol::data_t& data)
{
    co_await _socket.write(data);
}

void Session::stop()
{
    try
    {
        _client_activator.deactivate(_client_id);
        _socket.close();
        Logger::info("Session stopped for the client {}", _client_id);
    }
    catch (const std::exception& ex)
    {
        Logger::error("An error occurred during session stop: {} for the client {}", ex.what(), _client_id);
    }
}

boost::asio::awaitable<void> Session::internal_run()
{
    while (_socket.is_open())
    {
        try
        {
            Network::Protocol::Headers headers = co_await read_headers();
            co_await _dispatcher.dispatch(_client_id, headers);
        }
        catch (const Network::OperationException& ex)
        {
            Logger::error("An error occurred during the session run: {} for the client {}", ex.what(), _client_id);

            if (ex.error_code() == boost::asio::error::eof || ex.error_code() == boost::asio::error::connection_reset)
            {
                stop();
            }
        }
    }
}
