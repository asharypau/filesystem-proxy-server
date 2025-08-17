#ifndef CLIENTMAPPER_H
#define CLIENTMAPPER_H

#include "../models/client.h"
#include "../network/iclientconnection.h"

class ClientMapper
{
public:
    static ClientContext map(ClientActivation client_activation, Network::IClientConnection* connection);
    static ClientPayload map(ClientContext client_context);
};

#endif // CLIENTMAPPER_H
