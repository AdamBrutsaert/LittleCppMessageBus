#include "MessageBus/MessageBus.h"

std::vector<std::map<MessageID, std::vector<std::shared_ptr<MessageCallable>>>> MessageBus::s_Subscribers = {
    std::map<MessageID, std::vector<std::shared_ptr<MessageCallable>>>{}
};
MessageQueue MessageBus::s_Queue{1000};

static bool s_PreventPropagation = false;

void MessageBus::PreventPropagation()
{
    s_PreventPropagation = true;
}

void MessageBus::Dispatch() 
{
    while (!s_Queue.empty()) {
        auto element = s_Queue.front();

        for (auto it = s_Subscribers.begin(); it != s_Subscribers.end(); it++)
        {
            for (auto const& subscriber : (*it)[element.id])
            {
                (*subscriber)(element.msg);
            }
            if (s_PreventPropagation)
            {
                s_PreventPropagation = false;
                break;
            }
        }

        s_Queue.pop();
    }
}
