#ifndef CLIENT_H
#define CLIENT_H

#include "../network/iclientconnection.h"
#include "../network/protocoltypes.h"
#include "../network/serialization.h"
#include <string>
#include <tuple>

class Client
{
public:
    using id_t = std::string;

    Client::id_t id;
};

class ClientActivation : public Client, public Network::ISerializable<ClientActivation>
{
public:
    std::string root;

private:
    friend class Network::ISerializable<ClientActivation>;

    Network::Protocol::data_size_t size_impl() const
    {
        return sizeof(Network::Protocol::data_size_t) + static_cast<Network::Protocol::data_size_t>(id.size())
               + sizeof(Network::Protocol::data_size_t) + static_cast<Network::Protocol::data_size_t>(root.size());
    }

    auto as_tuple_impl() { return std::tie(id, root); }
    auto as_tuple_impl() const { return std::tie(id, root); }
};

class ClientContext : public Client
{
public:
    std::string root;
    Network::IClientConnection* connection = nullptr;

    bool is_active() const { return connection != nullptr; }
};

class ClientPayload : public Client, public Network::ISerializable<ClientPayload>
{
public:
    bool is_active = false;

private:
    friend class Network::ISerializable<ClientPayload>;

    Network::Protocol::data_size_t size_impl() const
    {
        return sizeof(Network::Protocol::data_size_t) + static_cast<Network::Protocol::data_size_t>(id.size())
               + sizeof(is_active);
    }

    auto as_tuple_impl() { return std::tie(id, is_active); }
    auto as_tuple_impl() const { return std::tie(id, is_active); }
};

class ClientActivationException : public std::runtime_error
{
public:
    explicit ClientActivationException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

#endif // CLIENT_H
