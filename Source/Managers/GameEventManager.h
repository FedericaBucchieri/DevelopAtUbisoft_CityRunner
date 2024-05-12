#pragma once
#include "stdafx.h"
#include "GameEvent.h"

//
// CGameEventManager is responsible for managing event subscriptions and publishing.
// It provides a simple mechanism for subscribing to and publishing events of various types.
//

class CGameEventManager {
public:
    using EventHandler = std::function<void(FGameEvent*)>;

    template <typename EventType>
    void Subscribe(const int uniqueId, const std::function<void(EventType*)>& handler) {
        auto typeIndex = std::type_index(typeid(EventType));

        auto it = m_subscribers.find(uniqueId);
        if (it != m_subscribers.end()) {
            auto& handlersOfType = it->second[typeIndex];
            handlersOfType.emplace_back([handler](FGameEvent* event) {
                    handler(static_cast<EventType*>(event));
                });
        }
        else {
            m_subscribers[uniqueId][typeIndex].emplace_back([handler](FGameEvent* event) {
                    handler(static_cast<EventType*>(event));
                });
        }
    }

    template <typename EventType>
    void Publish(const int uniqueId, FGameEvent* eventData) {
        auto typeIndex = std::type_index(typeid(EventType));

        auto it = m_subscribers.find(uniqueId);
        if (it != m_subscribers.end()) {
            auto itTypeIndex = it->second.find(typeIndex);
            if (itTypeIndex != it->second.end()) {
                for (const auto& handler : itTypeIndex->second) {
                    handler(eventData);
                }
            }
        }
    }

    void RemoveAllSubscriptions(const int uniqueId) {
        m_subscribers.erase(uniqueId);
    }

    template <typename EventType>
    void RemoveSubscription(const int uniqueId, const std::function<void(EventType*)>& handler) {
        auto typeIndex = std::type_index(typeid(EventType));

        auto it = m_subscribers.find(uniqueId);
        if (it != m_subscribers.end()) {
            auto itTypeIndex = it->second.find(typeIndex);
            if (itTypeIndex != it->second.end()) {
                for (auto itHandler = itTypeIndex->second.begin(); itHandler != itTypeIndex->second.end(); ++itHandler) {
                    if (itHandler->target<std::function<void(EventType*)>>() == handler.target<std::function<void(EventType*)>>()) {
                        itTypeIndex->second.erase(itHandler);
                        break;
                    }
                }
            }
        }
    }



private:
    std::unordered_map<int, std::map<std::type_index, std::vector<EventHandler>>> m_subscribers;
};