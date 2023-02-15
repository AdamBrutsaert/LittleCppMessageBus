#include "MessageBus/MessageQueue.h"

MessageQueue::MessageQueue(std::size_t capacity)
    : m_Capacity(capacity), m_Length(0), m_Cursor(0), m_Queue(nullptr)
{
    if (m_Capacity != 0) {
        m_Queue = static_cast<uint8_t*>(std::malloc(m_Capacity));

        if (m_Queue == nullptr) {
            throw std::bad_alloc();
        }
    }
}

MessageQueue::~MessageQueue() 
{
    std::free(m_Queue);
    m_Queue = nullptr;
}

void MessageQueue::push(MessageQueueElement const& element) 
{
    // Realloc the buffer it there's not enough space to store the element
    if (m_Capacity < m_Length + sizeof(element.id) + sizeof(element.size) + element.size)
    {
        std::size_t new_capacity = (m_Length + sizeof(element.id) + sizeof(element.size) + element.size) * 2;
        uint8_t* ptr = static_cast<uint8_t *>(std::realloc(m_Queue, new_capacity));

        if (ptr == nullptr)
        {
            std::free(m_Queue);
            m_Queue = nullptr;
            throw std::bad_alloc();
        }

        m_Queue = ptr;
        m_Capacity = new_capacity;
    }

    std::memcpy(m_Queue + m_Length, &element.id, sizeof(element.id));
    std::memcpy(m_Queue + m_Length + sizeof(element.id), &element.size,
                sizeof(element.size));
    std::memcpy(m_Queue + m_Length + sizeof(element.id) + sizeof(element.size),
                element.msg, element.size);

    m_Length += sizeof(element.id) + sizeof(element.size) + element.size;
}

void MessageQueue::pop() 
{
    if (!m_Length == 0) {
        std::size_t size = *reinterpret_cast<std::size_t*>(m_Queue + m_Cursor + sizeof(MessageID));

        m_Cursor += sizeof(MessageID) + sizeof(std::size_t) + size;

        if (m_Cursor == m_Length) {
            m_Cursor = 0;
            m_Length = 0;
        }
    }
}

MessageQueueElement MessageQueue::front() 
{
    if (m_Length == 0) {
        std::free(m_Queue);
        m_Queue = nullptr;
        throw std::runtime_error("Cannot retrieve front because queue is empty.");
    }

    return {
        (m_Queue + m_Cursor + sizeof(MessageID) + sizeof(std::size_t)),
        *reinterpret_cast<std::size_t*>(m_Queue + m_Cursor + sizeof(MessageID)),
        *reinterpret_cast<MessageID*>(m_Queue + m_Cursor)
    };
}

bool MessageQueue::empty() 
{ 
    return m_Length == 0; 
}
