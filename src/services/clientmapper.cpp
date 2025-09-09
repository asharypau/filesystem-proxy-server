#include "clientmapper.h"

ClientContext ClientMapper::map(ClientActivation client_activation, Network::IClientConnection* connection)
{
    ClientContext client_context;
    client_context.id = client_activation.id;
    client_context.root = client_activation.root;
    client_context.connection = connection;

    return client_context;
}

ClientPayload ClientMapper::map(ClientContext client_context)
{
    ClientPayload client_payload;
    client_payload.id = client_context.id;
    client_payload.is_active = client_context.is_active();

    return client_payload;
}
