#ifndef CLIENTREGISTRY_H
#define CLIENTREGISTRY_H

#include "../models/client.h"
#include <mutex>
#include <unordered_map>
#include <vector>

class ClientRegistry
{
public:
    ClientRegistry()
        : _mtx()
        , _clients_map()
    {
    }

    // Disallow movement  and assignment
    ClientRegistry(ClientRegistry&& other) = delete;
    ClientRegistry& operator=(ClientRegistry&& other) = delete;

    // Disallow copying and assignment
    ClientRegistry(const ClientRegistry&) = delete;
    ClientRegistry& operator=(const ClientRegistry&) = delete;

    bool exists(Client::id_t id)
    {
        std::lock_guard<std::mutex> lock(_mtx);

        return _clients_map.contains(id);
    }

    void add(const Client& client)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _clients_map.emplace(client.id, client);
    }

    Client get(Client::id_t id)
    {
        std::lock_guard<std::mutex> lock(_mtx);

        return _clients_map.at(id);
    }

    std::vector<Client> get_all()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        std::vector<Client> clients;
        clients.reserve(_clients_map.size());

        for (auto& pair : _clients_map)
        {
            clients.push_back(pair.second);
        }

        return std::move(clients);
    }

    void activate(Client::id_t id, Session* session)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _clients_map.at(id).session = session;
    }

    void deactivate(Client::id_t id)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _clients_map.at(id).session = nullptr;
    }

private:
    std::mutex _mtx;
    std::unordered_map<Client::id_t, Client> _clients_map;
};

#endif // CLIENTREGISTRY_H
