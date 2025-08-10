#ifndef CLIENTMAPPER_H
#define CLIENTMAPPER_H

#include "../models/clientcontext.h"
#include "../models/clientinput.h"
#include "../models/clientpayload.h"

// forward declaration
class Session;

class ClientMapper
{
public:
    static ClientContext map(ClientInput client_input, Session* session);
    static ClientPayload map(ClientContext client_context);
};

#endif // CLIENTMAPPER_H
