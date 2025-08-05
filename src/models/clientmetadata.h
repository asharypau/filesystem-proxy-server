#ifndef CLIENTMETADATA_H
#define CLIENTMETADATA_H

#include "../network/constants.h"
#include "../network/serialization.h"
#include <string>
#include <vector>

class ClientMetadata : public Network::Serialization::ISerializable<ClientMetadata>
{
public:
    using id_t = std::string;

    ClientMetadata::id_t id;
    std::string root;

private:
    friend class Network::Serialization::ISerializable<ClientMetadata>;

    Network::data_t serialize_impl() const
    {
        Network::data_t data(Network::DATA_SIZE + static_cast<Network::data_size_t>(id.size()) + Network::DATA_SIZE
                             + static_cast<Network::data_size_t>(root.size()));
        Network::data_t::iterator begin = data.begin();

        // id size
        Network::data_size_t id_size = static_cast<Network::data_size_t>(id.size());
        std::memcpy(&(*begin), &id_size, Network::DATA_SIZE);
        begin += Network::DATA_SIZE;

        // id
        std::memcpy(&(*begin), id.data(), id_size);
        begin += id_size;

        // root size
        Network::data_size_t root_size = static_cast<Network::data_size_t>(root.size());
        std::memcpy(&(*begin), &root_size, Network::DATA_SIZE);
        begin += Network::DATA_SIZE;

        // root
        std::memcpy(&(*begin), root.data(), root_size);
        begin += root_size;

        return data;
    }

    void deserialize_impl(const Network::data_t& data)
    {
        Network::data_t::const_iterator begin = data.cbegin();

        // id size
        Network::data_size_t id_size = 0;
        std::memcpy(&id_size, &(*begin), Network::DATA_SIZE);
        begin += Network::DATA_SIZE;

        // id
        id = std::string(begin, begin + id_size);
        begin += id_size;

        // root size
        Network::data_size_t root_size = 0;
        std::memcpy(&root_size, &(*begin), Network::DATA_SIZE);
        begin += Network::DATA_SIZE;

        // root
        root = std::string(begin, begin + root_size);
        begin += root_size;
    }
};

#endif // CLIENTMETADATA_H
