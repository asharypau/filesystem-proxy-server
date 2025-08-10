#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
public:
    using id_t = std::string;

    Client::id_t id;
};

#endif // CLIENT_H
