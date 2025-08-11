#ifndef NETWORK_HEADERDRAFT_H
#define NETWORK_HEADERDRAFT_H

#include "requesttype.h"

namespace Network
{
class HeaderDraft
{
public:
    Network::RequestType type = Network::RequestType::None;

    static Network::HeaderDraft of_type(Network::RequestType type) { return {type}; }
};
} // namespace Network

#endif // NETWORK_HEADERDRAFT_H
