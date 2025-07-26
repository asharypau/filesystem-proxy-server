#include "clientsubscriptionsmanager.h"

ClientSubscriptionsManager::ClientSubscriptionsManager(ClientRegistry& client_registry)
    : _client_registry(client_registry)
    , _mtx()
    , _subscribers_map()
{
}

void ClientSubscriptionsManager::subscribe(Client::id_t subject, Client::id_t subscriber)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _subscribers_map[subject].insert(subscriber);
}

void ClientSubscriptionsManager::unsubscribe(Client::id_t subject, Client::id_t subscriber)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _subscribers_map[subject].erase(subscriber);
}

std::unordered_set<Client::id_t> ClientSubscriptionsManager::get_subscribers(Client::id_t subject)
{
    static const std::unordered_set<Client::id_t> empty;
    std::lock_guard<std::mutex> lock(_mtx);
    auto it = _subscribers_map.find(subject);

    return it != _subscribers_map.end() ? it->second : empty;
}

void ClientSubscriptionsManager::notify(Client::id_t subject)
{
    for (Client::id_t subscriber : get_subscribers(subject))
    {
        Client client = _client_registry.get(subscriber);
        if (client.is_active())
        {
            //client.session->update();
        }
    }
}
