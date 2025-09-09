#ifndef SESSION_H
#define SESSION_H

#include "../models/client.h"
#include "../network/iclientconnection.h"
#include "../network/protocol.h"
#include "../network/protocoltypes.h"
#include "../network/tcp/socket.h"
#include "../services/clientactivator.h"
#include "../utils/logger.h"
#include "dispatcher.h"
#include <boost/asio.hpp>

class Session : public Network::IClientConnection
{
public:
    Session(Network::Tcp::Socket socket, ClientActivator& client_activator, Dispatcher& dispatcher);
    ~Session();

    // Disallow movement  and assignment
    Session(Session&& other) = delete;
    Session& operator=(Session&& other) = delete;

    // Disallow copying and assignment
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    boost::asio::awaitable<Network::Protocol::Headers> read_headers() override;
    boost::asio::awaitable<Network::Protocol::data_t> read(Network::Protocol::data_size_t data_size) override;
    boost::asio::awaitable<void> write_headers(const Network::Protocol::Headers& headers) override;
    boost::asio::awaitable<void> write(const Network::Protocol::data_t& data) override;
    void stop();

    template<class TCompletionHandler>
    boost::asio::awaitable<void> run(TCompletionHandler completion_handler)
    {
        try
        {
            _client_id = co_await _client_activator.activate(this);
            co_await internal_run();
        }
        catch (const ClientActivationException& ex)
        {
            Logger::error("An error occurred during the session run: {}", ex.what());
            stop();
        }
        catch (const std::exception& ex)
        {
            Logger::error("An error occurred during the session run: {} for the client {}", ex.what(), _client_id);
            stop();
        }

        completion_handler(this);
    }

private:
    boost::asio::awaitable<void> internal_run();

private:
    Network::Tcp::Socket _socket;
    ClientActivator& _client_activator;
    Dispatcher& _dispatcher;
    Client::id_t _client_id;
};

#endif // SESSION_H
