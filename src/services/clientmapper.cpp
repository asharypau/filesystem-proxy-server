#include "clientmapper.h"

ClientContext ClientMapper::map(ClientInput client_input, Session* session)
{
    ClientContext client_context;
    client_context.id = client_input.id;
    client_context.root = client_input.root;
    client_context.session = session;

    return client_context;
}

ClientPayload ClientMapper::map(ClientContext client_context)
{
    ClientPayload client_payload;
    client_payload.id = client_context.id;
    client_payload.is_active = client_context.is_active();

    return client_payload;
}
