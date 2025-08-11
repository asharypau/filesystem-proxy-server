#include "socket.h"
#include "../operationexception.h"

using namespace Network::Tcp;

Socket::Socket(boost::asio::ip::tcp::socket socket)
    : _socket(std::move(socket))
    , _read_buffer()
{
}

Socket::Socket(boost::asio::io_context& context)
    : _socket(context)
    , _read_buffer()
{
}

Socket::~Socket()
{
    close();
}

Socket::Socket(Socket&& other) noexcept
    : _socket(std::move(other._socket))
    , _read_buffer(std::move(other._read_buffer))
{
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this != &other)
    {
        _socket = std::move(other._socket);
        _read_buffer = std::move(other._read_buffer);
    }

    return *this;
}

void Socket::close()
{
    if (_socket.is_open())
    {
        _socket.close();
    }
}

boost::asio::awaitable<Network::data_t> Network::Tcp::Socket::read_data(Network::data_size_t data_size)
{
    auto [error, _] = co_await boost::asio::async_read(_socket,
                                                       _read_buffer.prepare(data_size),
                                                       boost::asio::transfer_exactly(data_size),
                                                       boost::asio::as_tuple(boost::asio::use_awaitable));

    if (error)
    {
        throw Network::OperationException(error);
    }

    _read_buffer.commit(data_size);

    Network::data_t extracted_data(data_size);
    unsigned char* begin = static_cast<unsigned char*>(_read_buffer.data().data());

    // copy bytes from read_buffer to extracted_data
    std::memcpy(extracted_data.data(), begin, data_size);

    _read_buffer.consume(data_size);

    co_return extracted_data;
}

boost::asio::awaitable<void> Network::Tcp::Socket::write_data(const Network::data_t& data)
{
    auto [error, _] = co_await boost::asio::async_write(_socket,
                                                        boost::asio::buffer(data),
                                                        boost::asio::as_tuple(boost::asio::use_awaitable));

    if (error)
    {
        throw Network::OperationException(error);
    }
}
