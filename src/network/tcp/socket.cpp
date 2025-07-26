#include "socket.h"

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

boost::asio::awaitable<Network::data_t> Network::Tcp::Socket::read_data_async()
{
    Network::data_size_t data_size = co_await read_data_size_async();

    co_return std::move(read_data(data_size));
}

boost::asio::awaitable<void> Network::Tcp::Socket::write_data_async(const Network::data_t& data)
{
    const Network::data_t write_buffer = get_write_buffer(data);
    auto [error, _] = co_await boost::asio::async_write(_socket,
                                                        boost::asio::buffer(write_buffer),
                                                        boost::asio::as_tuple(boost::asio::use_awaitable));

    if (error)
    {
        throw Network::OperationException(error);
    }
}

boost::asio::awaitable<Network::data_size_t> Network::Tcp::Socket::read_data_size_async()
{
    auto [error, _] = co_await boost::asio::async_read(_socket,
                                                       _read_buffer.prepare(Network::DATA_SIZE),
                                                       boost::asio::transfer_exactly(Network::DATA_SIZE),
                                                       boost::asio::as_tuple(boost::asio::use_awaitable));

    if (error)
    {
        throw Network::OperationException(error);
    }

    _read_buffer.commit(Network::DATA_SIZE);

    Network::data_t extracted_data = extract_from_read_buffer(Network::DATA_SIZE);

    Network::data_size_t data_size;
    std::memcpy(&data_size, extracted_data.data(), Network::DATA_SIZE);

    co_return data_size;
}

Network::data_t Network::Tcp::Socket::read_data(Network::data_size_t data_size)
{
    boost::system::error_code error;
    boost::asio::read(_socket, _read_buffer.prepare(data_size), boost::asio::transfer_exactly(data_size), error);

    if (error)
    {
        throw Network::OperationException(error);
    }

    _read_buffer.commit(data_size);

    return std::move(extract_from_read_buffer(data_size));
}

Network::data_t Network::Tcp::Socket::extract_from_read_buffer(Network::data_size_t data_size)
{
    Network::data_t extracted_data(data_size);
    unsigned char* begin = static_cast<unsigned char*>(_read_buffer.data().data());

    // copy bytes from read_buffer to extracted_data
    std::memcpy(extracted_data.data(), begin, data_size);

    _read_buffer.consume(data_size);

    return std::move(extracted_data);
}

Network::data_t Network::Tcp::Socket::get_write_buffer(const Network::data_t& data) const
{
    Network::data_size_t data_size = data.size();
    Network::data_t write_buffer = Network::data_t(Network::DATA_SIZE + data_size);

    // copy data size
    std::memcpy(write_buffer.data(), &data_size, Network::DATA_SIZE);
    // copy data
    std::memcpy(write_buffer.data() + Network::DATA_SIZE, data.data(), data_size);

    return std::move(write_buffer);
}
