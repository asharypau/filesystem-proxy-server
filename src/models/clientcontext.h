#ifndef CLIENTCONTEXT_H
#define CLIENTCONTEXT_H

#include "client.h"
#include <string>

// forward declaration
class Session;

class ClientContext : public Client
{
public:
    std::string root;
    Session* session = nullptr;

    bool is_active() const { return session != nullptr; }
};

#endif // CLIENTCONTEXT_H
