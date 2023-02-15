#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "Message.h"

struct MessageQueueElement
{
    void const* msg;
    std::size_t size;
    MessageID id;
};

class MessageQueue
{
public:
    MessageQueue(std::size_t capacity);
    ~MessageQueue();

    void push(MessageQueueElement const& element);
    void pop();

    MessageQueueElement front();

    bool empty();

private:
    std::size_t m_Capacity;
    std::size_t m_Length;
    std::size_t m_Cursor;
    uint8_t* m_Queue;
};
