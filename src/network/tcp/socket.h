#ifndef NETWORK_TCP_SOCKET_H
#define NETWORK_TCP_SOCKET_H

#include "../constants.h"
#include "../operationexception.h"
#include "../serialization.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace Network
{
namespace Tcp
{
class Socket
{
public:
    explicit Socket(boost::asio::ip::tcp::socket socket);
    explicit Socket(boost::asio::io_context& context);
    ~Socket();

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    // Disallow copying and assignment
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    void close();
    boost::asio::awaitable<Network::data_t> read_data_async();
    boost::asio::awaitable<void> write_data_async(const Network::data_t& data);

    template<class TModel>
    boost::asio::awaitable<TModel> read_async()
    {
        Network::data_t data = co_await read_data_async();
        TModel model = Network::Serialization::deserialize<TModel>(data);

        co_return std::move(model);
    }

    template<class TModel>
    boost::asio::awaitable<void> write_async(const TModel& model)
    {
        Network::data_t data = Network::Serialization::serialize<TModel>(model);
        co_await write_data_async(data);
    }

private:
    boost::asio::awaitable<Network::data_size_t> read_data_size_async();
    Network::data_t read_data(Network::data_size_t data_size);
    Network::data_t extract_from_read_buffer(Network::data_size_t data_size);
    Network::data_t get_write_buffer(const Network::data_t& data) const;

private:
    boost::asio::ip::tcp::socket _socket;
    boost::beast::flat_buffer _read_buffer;
};
} // namespace Tcp
} // namespace Network

#endif // NETWORK_TCP_SOCKET_H
