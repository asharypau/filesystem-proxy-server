#ifndef CLIENTPAYLOAD_H
#define CLIENTPAYLOAD_H

#include "../network/serialization.h"
#include "client.h"

class ClientPayload : public Client, public Network::ISerializable<ClientPayload>
{
public:
    bool is_active = false;

private:
    friend class Network::ISerializable<ClientPayload>;

    Network::data_size_t size_impl() const
    {
        return Network::DATA_SIZE + static_cast<Network::data_size_t>(id.size()) + sizeof(is_active);
    }

    auto as_tuple_impl() { return std::tie(id, is_active); }
    auto as_tuple_impl() const { return std::tie(id, is_active); }
};

#endif // CLIENTPAYLOAD_H
