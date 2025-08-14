#include "clientmapper.h"

ClientContext ClientMapper::map(ClientActivationData client_activation_data, Network::IClientConnection* connection)
{
    ClientContext client_context;
    client_context.id = client_activation_data.id;
    client_context.root = client_activation_data.root;
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
