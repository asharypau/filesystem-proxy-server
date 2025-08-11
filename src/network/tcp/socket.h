#ifndef NETWORK_TCP_SOCKET_H
#define NETWORK_TCP_SOCKET_H

#include "../constants.h"
#include "../headerdraft.h"
#include "../headerpackage.h"
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

    template<class TModel>
    boost::asio::awaitable<TModel> read()
    {
        Network::data_t header_package_data = co_await read_data(Network::HeaderPackage::SIZE);
        Network::HeaderPackage header_package = Serializer::deserialize<Network::HeaderPackage>(header_package_data);

        Network::data_t data = co_await read_data(header_package.data_size);

        co_return Network::Serializer::deserialize<TModel>(data);
    }

    template<class TModel>
    boost::asio::awaitable<void> write(const Network::HeaderDraft& header_draft, const TModel& model)
    {
        Network::data_t data = Network::Serializer::serialize(model);

        Network::HeaderPackage header_package = Network::HeaderPackage::from_draft(header_draft, data.size());
        Network::data_t header_package_data = Network::Serializer::serialize(header_package);

        co_await write_data(header_package_data);
        co_await write_data(data);
    }

private:
    boost::asio::awaitable<Network::data_t> read_data(Network::data_size_t data_size);
    boost::asio::awaitable<void> write_data(const Network::data_t& data);

private:
    boost::asio::ip::tcp::socket _socket;
    boost::beast::flat_buffer _read_buffer;
};
} // namespace Tcp
} // namespace Network

#endif // NETWORK_TCP_SOCKET_H
