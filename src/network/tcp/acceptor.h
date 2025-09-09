#ifndef NETWORK_TCP_ACCEPTOR_H
#define NETWORK_TCP_ACCEPTOR_H

#include "socket.h"
#include <boost/asio.hpp>
#include <string>

namespace Network
{
namespace Tcp
{
class Acceptor
{
public:
    Acceptor(unsigned short port, std::string host, boost::asio::io_context& context);

    boost::asio::awaitable<Network::Tcp::Socket> accept();

private:
    boost::asio::ip::tcp::acceptor _acceptor;
};
} // namespace Tcp
} // namespace Network

#endif // NETWORK_TCP_ACCEPTOR_H
