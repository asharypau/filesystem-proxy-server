#include "acceptor.h"
#include "../operationexception.h"

using namespace Network::Tcp;

Acceptor::Acceptor(unsigned short port, std::string host, boost::asio::io_context& context)
    : _acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

boost::asio::awaitable<Network::Tcp::Socket> Acceptor::accept()
{
    auto [error_code, socket] = co_await _acceptor.async_accept(_acceptor.get_executor(),
                                                                boost::asio::as_tuple(boost::asio::use_awaitable));

    if (error_code)
    {
        throw Network::OperationException(error_code);
    }

    co_return Network::Tcp::Socket(std::move(socket));
}
