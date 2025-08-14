#include "protocol.h"

using namespace Network::Protocol;

std::string Network::Protocol::to_string(Network::Protocol::Type type)
{
    {
        switch (type)
        {
        case Network::Protocol::Type::Activation:
            return "Activation";
        case Network::Protocol::Type::Client:
            return "Client";

        case Network::Protocol::Type::None:
        default:
            return "None";
        }
    }
}

std::string Network::Protocol::to_string(Network::Protocol::Method method)
{
    {
        switch (method)
        {
        case Network::Protocol::Method::Get:
            return "Get";
        case Network::Protocol::Method::GetAll:
            return "GetAll";

        case Network::Protocol::Method::None:
        default:
            return "None";
        }
    }
}
