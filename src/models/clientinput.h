#ifndef CLIENTINPUT_H
#define CLIENTINPUT_H

#include "../network/constants.h"
#include "../network/serialization.h"
#include "client.h"
#include <string>

class ClientInput : public Client, public Network::ISerializable<ClientInput>
{
public:
    std::string root;

private:
    friend class Network::ISerializable<ClientInput>;

    Network::data_size_t size_impl() const
    {
        return Network::DATA_SIZE + static_cast<Network::data_size_t>(id.size()) + Network::DATA_SIZE
               + static_cast<Network::data_size_t>(root.size());
    }

    auto as_tuple_impl() { return std::tie(id, root); }
    auto as_tuple_impl() const { return std::tie(id, root); }
};

#endif // CLIENTINPUT_H
