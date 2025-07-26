#ifndef CLIENTSUBSCRIPTIONSMANAGER_H
#define CLIENTSUBSCRIPTIONSMANAGER_H

#include "../models/client.h"
#include "clientregistry.h"
#include <mutex>
#include <unordered_map>
#include <unordered_set>

class ClientSubscriptionsManager
{
public:
    explicit ClientSubscriptionsManager(ClientRegistry& client_registry);

    // Disallow movement  and assignment
    ClientSubscriptionsManager(ClientSubscriptionsManager&& other) = delete;
    ClientSubscriptionsManager& operator=(ClientSubscriptionsManager&& other) = delete;

    // Disallow copying and assignment
    ClientSubscriptionsManager(const ClientSubscriptionsManager&) = delete;
    ClientSubscriptionsManager& operator=(const ClientSubscriptionsManager&) = delete;

    void subscribe(Client::id_t subject, Client::id_t subscriber);
    void unsubscribe(Client::id_t subject, Client::id_t subscriber);
    std::unordered_set<Client::id_t> get_subscribers(Client::id_t subject);
    void notify(Client::id_t subject);

private:
    ClientRegistry& _client_registry;
    std::mutex _mtx;
    std::unordered_map<Client::id_t, std::unordered_set<Client::id_t>> _subscribers_map;
};

#endif // CLIENTSUBSCRIPTIONSMANAGER_H
