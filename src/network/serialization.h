#ifndef NETWORK_SERIALIZATION_H
#define NETWORK_SERIALIZATION_H

#include "constants.h"
#include <string>
#include <vector>

namespace Network
{
template<class TModel>
class ISerializable
{
public:
    Network::data_size_t size() const { return get_impl()->size_impl(); }
    auto as_tuple() { return get_impl()->as_tuple_impl(); }
    auto as_tuple() const { return get_impl()->as_tuple_impl(); }

private:
    TModel* get_impl() { return static_cast<TModel*>(this); }
    const TModel* get_impl() const { return static_cast<const TModel*>(this); }
};

template<class TModel>
concept Serializable = std::is_base_of_v<Network::ISerializable<TModel>, TModel>;

template<class TModel>
concept SerializableVector = std::is_same_v<TModel, std::vector<typename TModel::value_type>>
                             && Network::Serializable<typename TModel::value_type>;

class Serializer
{
public:
    template<class TModel>
    static Network::data_t serialize(const TModel& model)
    {
        Network::data_size_t offset = 0;
        Network::data_t data;

        if constexpr (Network::Serializable<TModel>)
        {
            data.resize(model.size());
            serialize_tuple(data, model.as_tuple(), &offset);
        }
        else if constexpr (Network::SerializableVector<TModel>)
        {
            serialize_vector(data, model, &offset);
        }

        return data;
    }

    template<class TModel>
    static TModel deserialize(const Network::data_t& data)
    {
        Network::data_size_t offset = 0;
        TModel model{};

        if constexpr (Network::Serializable<TModel>)
        {
            auto tuple = model.as_tuple();
            deserialize_tuple(data, tuple, &offset);
        }
        else if constexpr (Network::SerializableVector<TModel>)
        {
            deserialize_vector(data, model, &offset);
        }

        return model;
    }

private:
    template<class... Args>
    static void serialize_tuple(Network::data_t& data,
                                const std::tuple<const Args&...>& tuple,
                                Network::data_size_t* offset)
    {
        std::apply([&](const auto&... args) { (internal_serialize(data, args, offset), ...); }, tuple);
    }

    template<class... Args>
    static void deserialize_tuple(const Network::data_t& data, std::tuple<Args&...>& tuple, Network::data_size_t* offset)
    {
        std::apply([&](auto&... args) { (internal_deserialize(data, args, offset), ...); }, tuple);
    }

    template<class TModel>
    static void serialize_vector(Network::data_t& data, const std::vector<TModel>& model, Network::data_size_t* offset)
    {
        Network::data_size_t data_size = Network::DATA_SIZE;
        for (const TModel& item : model)
        {
            data_size += Network::DATA_SIZE;
            data_size += item.size();
        }

        data.resize(data_size);

        // vector size
        Network::data_size_t vector_size = static_cast<Network::data_size_t>(model.size());
        std::memcpy(data.data() + *offset, &vector_size, Network::DATA_SIZE);
        *offset += Network::DATA_SIZE;

        for (const TModel& item : model)
        {
            // item size
            Network::data_size_t item_size = static_cast<Network::data_size_t>(item.size());
            std::memcpy(data.data() + *offset, &item_size, Network::DATA_SIZE);
            *offset += Network::DATA_SIZE;

            serialize_tuple(data, item.as_tuple(), offset);
        }
    }

    template<class TModel>
    static void deserialize_vector(const Network::data_t& data, std::vector<TModel>& model, Network::data_size_t* offset)
    {
        // vector size
        Network::data_size_t vector_size = 0;
        std::memcpy(&vector_size, data.data() + *offset, Network::DATA_SIZE);
        *offset += Network::DATA_SIZE;

        model.reserve(vector_size);

        while (vector_size-- > 0)
        {
            // item size
            Network::data_size_t item_size = 0;
            std::memcpy(&item_size, data.data() + *offset, Network::DATA_SIZE);
            *offset += Network::DATA_SIZE;

            model.emplace_back();
            auto tuple = model.back().as_tuple();
            deserialize_tuple(data, tuple, offset);
        }
    }

    template<class TModel>
    static void internal_serialize(Network::data_t& data, const TModel& model, Network::data_size_t* offset)
    {
        if constexpr (std::is_trivially_copyable_v<TModel>)
        {
            serialize_trivially_copyable_type(data, model, offset);
        }
        else if constexpr (std::is_same_v<TModel, std::string>)
        {
            serialize_string(data, model, offset);
        }
    }

    template<class TModel>
    static void internal_deserialize(const Network::data_t& data, TModel& model, Network::data_size_t* offset)
    {
        if constexpr (std::is_trivially_copyable_v<TModel>)
        {
            deserialize_trivially_copyable_type(data, model, offset);
        }
        else if constexpr (std::is_same_v<TModel, std::string>)
        {
            deserialize_string(data, model, offset);
        }
    }

    template<class TModel>
    static void serialize_trivially_copyable_type(Network::data_t& data,
                                                  const TModel& model,
                                                  Network::data_size_t* offset)
    {
        std::memcpy(data.data() + *offset, &model, sizeof(TModel));
        *offset += sizeof(TModel);
    }

    template<class TModel>
    static void deserialize_trivially_copyable_type(const Network::data_t& data,
                                                    TModel& model,
                                                    Network::data_size_t* offset)
    {
        std::memcpy(&model, data.data() + *offset, sizeof(TModel));
        *offset += sizeof(TModel);
    }

    static void serialize_string(Network::data_t& data, const std::string& model, Network::data_size_t* offset)
    {
        // string size
        const Network::data_size_t string_size = static_cast<const Network::data_size_t>(model.size());
        std::memcpy(data.data() + *offset, &string_size, Network::DATA_SIZE);
        *offset += Network::DATA_SIZE;

        // string
        std::memcpy(data.data() + *offset, model.data(), string_size);
        *offset += string_size;
    }

    static void deserialize_string(const Network::data_t& data, std::string& model, Network::data_size_t* offset)
    {
        // string size
        Network::data_size_t string_size = 0;
        std::memcpy(&string_size, data.data() + *offset, Network::DATA_SIZE);
        *offset += Network::DATA_SIZE;

        // string
        model.reserve(string_size);
        model.append(reinterpret_cast<const char*>(data.data() + *offset), string_size);
        *offset += string_size;
    }
};
} // namespace Network

#endif // NETWORK_SERIALIZATION_H
