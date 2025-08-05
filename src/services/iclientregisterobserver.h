#ifndef ICLIENTREGISTRYOBSERVER
#define ICLIENTREGISTRYOBSERVER

#include "../models/client.h"

class IClientRegistryObserver
{
public:
    virtual void added(const Client::id_t& id) = 0;
    virtual void removed(const Client::id_t& id) = 0;
    virtual void activated(const Client::id_t& id) = 0;
    virtual void deactivated(const Client::id_t& id) = 0;
};

#endif // ICLIENTREGISTRYOBSERVER
