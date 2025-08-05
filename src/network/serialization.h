#ifndef NETWORK_SERIALIZATION_H
#define NETWORK_SERIALIZATION_H

#include "constants.h"
#include <vector>

namespace Network
{
namespace Serialization
{
template<class TModel>
class ISerializable
{
public:
    Network::data_t serialize() const { return get_impl()->serialize_impl(); }
    void deserialize(const Network::data_t& data) { get_impl()->deserialize_impl(data); }

private:
    TModel* get_impl() { return static_cast<TModel*>(this); }
    const TModel* get_impl() const { return static_cast<const TModel*>(this); }
};

template<class TModel>
concept Serializable = std::is_base_of<Network::Serialization::ISerializable<TModel>, TModel>::value;

template<class TModel>
concept SerializableVector = std::is_same_v<TModel, std::vector<typename TModel::value_type>>
                             && Network::Serialization::Serializable<typename TModel::value_type>;

template<class TModel>
Network::data_t serialize(const TModel& model)
{
    if constexpr (Network::Serialization::Serializable<TModel>)
    {
        return model.serialize();
    }
    else if constexpr (Network::Serialization::SerializableVector<TModel>)
    {
        Network::data_size_t offset = 0;
        Network::data_size_t data_size = Network::DATA_SIZE;

        std::vector<Network::data_t> serialized_models;
        serialized_models.reserve(model.size());

        for (const typename TModel::value_type& item : model)
        {
            Network::data_t serialized_item = item.serialize();

            data_size += Network::DATA_SIZE + static_cast<Network::data_size_t>(serialized_item.size());
            serialized_models.push_back(std::move(serialized_item));
        }

        Network::data_t data(data_size);

        // write serialized models size
        Network::data_size_t serialized_models_size = static_cast<Network::data_size_t>(serialized_models.size());
        std::memcpy(data.data() + offset, &serialized_models_size, Network::DATA_SIZE);
        offset += Network::DATA_SIZE;

        for (const Network::data_t& item : serialized_models)
        {
            Network::data_size_t item_size = static_cast<Network::data_size_t>(item.size());

            // write item size
            std::memcpy(data.data() + offset, &item_size, Network::DATA_SIZE);
            offset += Network::DATA_SIZE;

            // write item
            std::memcpy(data.data() + offset, item.data(), item_size);
            offset += item_size;
        }

        return data;
    }

    return {};
}

template<class TModel>
TModel deserialize(const Network::data_t& data)
{
    if constexpr (Network::Serialization::Serializable<TModel>)
    {
        TModel model;
        model.deserialize(data);

        return model;
    }
    else if constexpr (Network::Serialization::SerializableVector<TModel>)
    {
        // models size
        Network::data_size_t models_size;
        std::memcpy(&models_size, data.data(), Network::DATA_SIZE);

        // models
        TModel models;
        models.reserve(models_size);

        Network::data_t::const_iterator begin = data.begin() + Network::DATA_SIZE;

        for (Network::data_size_t i = 0; i != models_size; ++i)
        {
            // batch size
            Network::data_size_t batch_size;
            std::memcpy(&batch_size, &(*begin), Network::DATA_SIZE);
            begin += Network::DATA_SIZE;

            // batch
            Network::data_t batch(begin, begin + batch_size);
            begin += batch_size;

            typename TModel::value_type model;
            model.deserialize(batch);
            models.push_back(std::move(model));
        }

        return models;
    }

    return {};
}

} // namespace Serialization
} // namespace Network

#endif // NETWORK_SERIALIZATION_H
