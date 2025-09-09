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
    Activated,
    ClientRequest,
};

enum class Action : uint16_t
{
    None,
    GetClients,
    GetClientFiles,
};

class Headers : public Network::ISerializable<Headers>
{
public:
    static constexpr std::size_t SIZE = sizeof(Network::Protocol::data_size_t) + sizeof(Network::Protocol::Type)
                                        + sizeof(Network::Protocol::Action);

    Network::Protocol::data_size_t data_size = 0;
    Network::Protocol::Type type = Network::Protocol::Type::None;
    Network::Protocol::Action method = Network::Protocol::Action::None;

    static Network::Protocol::Headers make(Network::Protocol::data_size_t size = 0,
                                           Network::Protocol::Type request_type = Network::Protocol::Type::None,
                                           Network::Protocol::Action request_method = Network::Protocol::Action::None)
    {
        Network::Protocol::Headers headers;
        headers.data_size = size;
        headers.type = request_type;
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
std::string to_string(Network::Protocol::Action method);
} // namespace Protocol
} // namespace Network

#endif // NETWORK_PROTOCOL_H
