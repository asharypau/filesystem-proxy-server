#include "clientsubscriptionsmanager.h"

ClientSubscriptionsManager::ClientSubscriptionsManager(ClientRegistry& client_registry)
    : _client_registry(client_registry)
    , _mtx()
    , _subscribers_map()
{
    _client_registry.add_observer(this);
}

void ClientSubscriptionsManager::added(const Client::id_t& id)
{
    notify("client_registry_update");
}

void ClientSubscriptionsManager::removed(const Client::id_t& id)
{
    notify("client_registry_update");
}

void ClientSubscriptionsManager::activated(const Client::id_t& id)
{
    notify("client_registry_update");
}

void ClientSubscriptionsManager::deactivated(const Client::id_t& id)
{
    notify("client_registry_update");
}

std::unordered_set<Client::id_t> ClientSubscriptionsManager::get_subscribers(const Client::id_t& subject)
{
    static const std::unordered_set<Client::id_t> empty;

    std::lock_guard<std::mutex> lock(_mtx);
    auto it = _subscribers_map.find(subject);

    return it != _subscribers_map.end() ? it->second : empty;
}

void ClientSubscriptionsManager::subscribe(const Client::id_t& subject, const Client::id_t& subscriber)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _subscribers_map[subject].insert(subscriber);
}

void ClientSubscriptionsManager::unsubscribe(const Client::id_t& subject, const Client::id_t& subscriber)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _subscribers_map[subject].erase(subscriber);
}

void ClientSubscriptionsManager::notify(const Client::id_t& subject)
{
    for (const Client::id_t& subscriber : get_subscribers(subject))
    {
        ClientContext client = _client_registry.get(subscriber);
        if (client.is_active())
        {
            //client.session->request_update();
        }
    }
}
