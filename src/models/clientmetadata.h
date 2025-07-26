#ifndef CLIENTMETADATA_H
#define CLIENTMETADATA_H

#include "../network/constants.h"
#include "../network/serialization.h"
#include <string>
#include <vector>

class ClientMetadata : public Network::Serialization::ISerializable<ClientMetadata>
{
public:
    using id_t = std::size_t;

    id_t id = 0;
    std::string root;

private:
    friend class Network::Serialization::ISerializable<ClientMetadata>;

    Network::data_t serialize_impl() const
    {
        Network::data_size_t offset = 0;
        Network::data_t data(Network::DATA_SIZE + Network::DATA_SIZE + static_cast<Network::data_size_t>(root.size()));

        // id
        std::memcpy(data.data() + offset, &id, Network::DATA_SIZE);
        offset += Network::DATA_SIZE;

        // root size
        Network::data_size_t root_size = static_cast<Network::data_size_t>(root.size());
        std::memcpy(data.data() + offset, &root_size, Network::DATA_SIZE);
        offset += Network::DATA_SIZE;

        // root
        std::memcpy(data.data() + offset, root.data(), root_size);
        offset += root_size;

        return std::move(data);
    }

    void deserialize_impl(const Network::data_t& data)
    {
        Network::data_size_t offset = 0;
        const unsigned char* begin = data.data();

        // id
        std::memcpy(&id, begin + offset, Network::DATA_SIZE);
        offset += Network::DATA_SIZE;

        // root size
        Network::data_size_t root_size = 0;
        std::memcpy(&root_size, begin + offset, Network::DATA_SIZE);
        offset += Network::DATA_SIZE;

        // root
        root = std::string(begin + offset, begin + offset + root_size);
        offset += root_size;
    }
};

#endif // CLIENTMETADATA_H
