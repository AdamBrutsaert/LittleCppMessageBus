#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include <type_traits>

#include "Message.h"
#include "MessageQueue.h"
#include "MessageSubscriber.h"

class MessageBus {
public:
    template <typename M> 
    static void Register(uint32_t priority, MessageSubscriber<M> const& subscriber) {
        if (s_Subscribers.size() <= priority) {
            s_Subscribers.resize(priority + 1);
        }
        s_Subscribers[priority][GetMessageID<M>()].push_back(subscriber);
    }

    template <typename M, typename F> 
    static MessageSubscriber<M> Subscribe(uint32_t priority, F&& func) {
        auto subscriber = MakeMessageSubscriber<M, F>(std::forward<F>(func));
        Register(priority, subscriber);
        return subscriber;
    }

    template <typename M> 
    static void Remove(uint32_t priority, MessageSubscriber<M> const& subscriber) {
        if (s_Subscribers.size() <= priority) return;

        auto &v = s_Subscribers[priority][GetMessageID<M>()];

        auto it = std::find(v.begin(), v.end(), subscriber);
        if (it != v.end()) {
            std::iter_swap(it, v.end() - 1);
            v.pop_back();
        }
    }

    template <typename M> 
    static void Send(M const& msg) {
        s_Queue.push({&msg, sizeof(msg), GetMessageID<M>()});
    }

    template <typename M, typename... Args>
    static void Send(Args&&... args) {
        Send<M>({ std::forward<Args>(args)... });
    }

    static void PreventPropagation();
    static void Dispatch();

private:

    static std::vector<std::map<MessageID, std::vector<std::shared_ptr<MessageCallable>>>> s_Subscribers;
    static MessageQueue s_Queue;
};
