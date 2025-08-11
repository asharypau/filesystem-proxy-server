#ifndef NETWORK_HEADERPACKAGE_H
#define NETWORK_HEADERPACKAGE_H

#include "constants.h"
#include "headerdraft.h"
#include "requesttype.h"
#include "serialization.h"
#include <cstdint>

namespace Network
{
class HeaderPackage : public Network::ISerializable<HeaderPackage>
{
public:
    static constexpr std::size_t SIZE = sizeof(Network::RequestType) + sizeof(Network::data_size_t);

    Network::data_size_t data_size = 0;
    Network::RequestType type = Network::RequestType::None;

    static Network::HeaderPackage from_draft(const Network::HeaderDraft& header_draft, Network::data_size_t size)
    {
        Network::HeaderPackage header_package;
        header_package.type = header_draft.type;
        header_package.data_size = size;

        return header_package;
    }

private:
    friend class Network::ISerializable<HeaderPackage>;

    Network::data_size_t size_impl() const { return SIZE; }
    auto as_tuple_impl() { return std::tie(data_size, type); }
    auto as_tuple_impl() const { return std::tie(data_size, type); }
};
} // namespace Network

#endif // NETWORK_HEADERPACKAGE_H