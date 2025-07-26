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
    Network::data_t serialize() const { return std::move(get_impl()->serialize_impl()); }
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
        return std::move(model.serialize());
    }
    else if constexpr (Network::Serialization::SerializableVector<TModel>)
    {
        Network::data_size_t offset = 0;
        Network::data_t data(Network::DATA_SIZE);

        // model size
        Network::data_size_t model_size = static_cast<Network::data_size_t>(model.size());
        std::memcpy(data.data() + offset, &model_size, Network::DATA_SIZE);
        offset += Network::DATA_SIZE;

        for (const typename TModel::value_type& item : model)
        {
            Network::data_t serialized_item = item.serialize();
            Network::data_size_t serialized_item_size = static_cast<Network::data_size_t>(serialized_item.size());

            Network::data_t batch(Network::DATA_SIZE + serialized_item_size);
            Network::data_size_t batch_size = static_cast<Network::data_size_t>(batch.size());

            // batch size
            std::memcpy(batch.data(), &serialized_item_size, Network::DATA_SIZE);
            // batch
            std::memcpy(batch.data() + Network::DATA_SIZE, serialized_item.data(), serialized_item_size);

            // write batch to data
            data.resize(offset + batch_size);
            std::memcpy(data.data() + offset, batch.data(), batch_size);
            offset += batch_size;
        }

        return std::move(data);
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

        return std::move(model);
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

        return std::move(models);
    }

    return {};
}

} // namespace Serialization
} // namespace Network

#endif // NETWORK_SERIALIZATION_H
