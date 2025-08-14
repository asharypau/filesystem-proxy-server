#ifndef NETWORK_TCP_SOCKET_H
#define NETWORK_TCP_SOCKET_H

#include "../protocoltypes.h"
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
    bool is_open() { return _socket.is_open(); }
    boost::asio::awaitable<Network::Protocol::data_t> read(Network::Protocol::data_size_t data_size);
    boost::asio::awaitable<void> write(const Network::Protocol::data_t& data);

private:
    boost::asio::ip::tcp::socket _socket;
    boost::beast::flat_buffer _read_buffer;
};
} // namespace Tcp
} // namespace Network

#endif // NETWORK_TCP_SOCKET_H
