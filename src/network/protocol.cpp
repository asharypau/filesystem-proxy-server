#include "protocol.h"

using namespace Network::Protocol;

std::string Network::Protocol::to_string(Network::Protocol::Type type)
{
    {
        switch (type)
        {
        case Network::Protocol::Type::Activation:
            return "Activation";
        case Network::Protocol::Type::Activated:
            return "Activated";
        case Network::Protocol::Type::ClientRequest:
            return "ClientRequest";

        case Network::Protocol::Type::None:
        default:
            return "None";
        }
    }
}

std::string Network::Protocol::to_string(Network::Protocol::Action method)
{
    {
        switch (method)
        {
        case Network::Protocol::Action::GetClients:
            return "GetClients";
        case Network::Protocol::Action::GetClientFiles:
            return "GetClientFiles";

        case Network::Protocol::Action::None:
        default:
            return "None";
        }
    }
}
