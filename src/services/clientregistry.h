#ifndef CLIENTREGISTRY_H
#define CLIENTREGISTRY_H

#include "../models/client.h"
#include "../models/clientcontext.h"
#include "iclientregisterobserver.h"
#include <mutex>
#include <unordered_map>
#include <vector>

// forward declaration
class Session;

class ClientRegistry
{
public:
    ClientRegistry()
        : _mtx()
        , _clients_map()
        , _observers()
    {
    }

    // Disallow movement  and assignment
    ClientRegistry(ClientRegistry&& other) = delete;
    ClientRegistry& operator=(ClientRegistry&& other) = delete;

    // Disallow copying and assignment
    ClientRegistry(const ClientRegistry&) = delete;
    ClientRegistry& operator=(const ClientRegistry&) = delete;

    bool exists(const Client::id_t& id)
    {
        std::lock_guard<std::mutex> lock(_mtx);

        return _clients_map.contains(id);
    }

    ClientContext get(const Client::id_t& id)
    {
        std::lock_guard<std::mutex> lock(_mtx);

        return _clients_map.at(id);
    }

    std::vector<ClientContext> get_all()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        std::vector<ClientContext> clients;
        clients.reserve(_clients_map.size());

        for (auto& pair : _clients_map)
        {
            clients.push_back(pair.second);
        }

        return clients;
    }

    void add(const ClientContext& client)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _clients_map.emplace(client.id, client);
        }

        for (IClientRegistryObserver* observer : _observers)
        {
            observer->added(client.id);
        }
    }

    void remove(const Client::id_t& id)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _clients_map.erase(id);
        }

        for (IClientRegistryObserver* observer : _observers)
        {
            observer->removed(id);
        }
    }

    void activate(const Client::id_t& id, Session* session)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _clients_map.at(id).session = session;
        }

        for (IClientRegistryObserver* observer : _observers)
        {
            observer->activated(id);
        }
    }

    void deactivate(const Client::id_t& id)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _clients_map.at(id).session = nullptr;
        }

        for (IClientRegistryObserver* observer : _observers)
        {
            observer->deactivated(id);
        }
    }

    void add_observer(IClientRegistryObserver* observer) { _observers.push_back(observer); }

private:
    std::mutex _mtx;
    std::unordered_map<Client::id_t, ClientContext> _clients_map;
    std::vector<IClientRegistryObserver*> _observers;
};

#endif // CLIENTREGISTRY_H
