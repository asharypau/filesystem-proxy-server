#ifndef CLIENT_H
#define CLIENT_H

#include "clientmetadata.h"
#include <string>
#include <vector>

class Session;

class Client : public ClientMetadata
{
public:
    Session* session = nullptr;

    bool is_active() const { return session != nullptr; }
};

#endif // CLIENT_H
