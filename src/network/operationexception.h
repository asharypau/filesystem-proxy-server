#ifndef NETWORK_OPERATION_EXCEPTION_H
#define NETWORK_OPERATION_EXCEPTION_H

#include <boost/asio.hpp>
#include <string>

namespace Network
{
class OperationException : public std::runtime_error
{
public:
    explicit OperationException(boost::system::error_code error_code)
        : std::runtime_error(std::move(error_code.message()))
        , _error_code(error_code.value())
    {
    }

    OperationException(int error_code, const std::string& message)
        : std::runtime_error(message)
        , _error_code(error_code)
    {
    }

    int error_code() const noexcept { return _error_code; }

private:
    int _error_code;
};
} // namespace Network

#endif // NETWORK_OPERATION_EXCEPTION_H
