#ifndef CLIENTSUBSCRIPTIONSMANAGER_H
#define CLIENTSUBSCRIPTIONSMANAGER_H

#include "../models/client.h"
#include "../models/clientcontext.h"
#include "clientregistry.h"
#include "iclientregisterobserver.h"
#include <mutex>
#include <unordered_map>
#include <unordered_set>

class ClientSubscriptionsManager : public IClientRegistryObserver
{
public:
    explicit ClientSubscriptionsManager(ClientRegistry& client_registry);

    // Disallow movement  and assignment
    ClientSubscriptionsManager(ClientSubscriptionsManager&& other) = delete;
    ClientSubscriptionsManager& operator=(ClientSubscriptionsManager&& other) = delete;

    // Disallow copying and assignment
    ClientSubscriptionsManager(const ClientSubscriptionsManager&) = delete;
    ClientSubscriptionsManager& operator=(const ClientSubscriptionsManager&) = delete;

    void added(const Client::id_t& id) override;
    void removed(const Client::id_t& id) override;
    void activated(const Client::id_t& id) override;
    void deactivated(const Client::id_t& id) override;

    std::unordered_set<Client::id_t> get_subscribers(const Client::id_t& subject);
    void subscribe(const Client::id_t& subject, const Client::id_t& subscriber);
    void unsubscribe(const Client::id_t& subject, const Client::id_t& subscriber);
    void notify(const Client::id_t& subject);

private:
    ClientRegistry& _client_registry;
    std::mutex _mtx;
    std::unordered_map<Client::id_t, std::unordered_set<Client::id_t>> _subscribers_map;
};

#endif // CLIENTSUBSCRIPTIONSMANAGER_H
