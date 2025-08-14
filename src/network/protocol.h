#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "protocoltypes.h"
#include "serialization.h"
#include <string>
#include <tuple>

namespace Network
{
namespace Protocol
{
enum class Type : uint16_t
{
    None,
    Activation,
    Client,
};

enum class Method : uint16_t
{
    None,
    Get,
    GetAll,
};

class Headers : public Network::ISerializable<Headers>
{
public:
    static constexpr std::size_t SIZE = sizeof(Network::Protocol::data_size_t) + sizeof(Network::Protocol::Type)
                                        + sizeof(Network::Protocol::Method);

    Network::Protocol::data_size_t data_size = 0;
    Network::Protocol::Type type = Network::Protocol::Type::None;
    Network::Protocol::Method method = Network::Protocol::Method::None;

    static Network::Protocol::Headers make_client_type(Network::Protocol::data_size_t size,
                                                       Network::Protocol::Method request_method)
    {
        Network::Protocol::Headers headers;
        headers.data_size = size;
        headers.type = Network::Protocol::Type::Client;
        headers.method = request_method;

        return headers;
    }

private:
    friend class Network::ISerializable<Headers>;

    Network::Protocol::data_size_t size_impl() const { return SIZE; }
    auto as_tuple_impl() { return std::tie(data_size, type, method); }
    auto as_tuple_impl() const { return std::tie(data_size, type, method); }
};

std::string to_string(Network::Protocol::Type type);
std::string to_string(Network::Protocol::Method method);
} // namespace Protocol
} // namespace Network

#endif // NETWORK_PROTOCOL_H
