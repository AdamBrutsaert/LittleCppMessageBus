#include "MessageBus/MessageBus.h"

#include <cstdint>
#include <iostream>

struct Key
{
    uint32_t code;
};

void onKey(Key const &key)
{
    std::cout << "[Function] Key: " << key.code << std::endl;
}

class OnKeyCallable
{
public:
    void operator()(Key const &key) const
    {
        std::cout << "[Callable] Key: " << key.code << std::endl;
    }
};

int main()
{
    // Using lambda
    MessageBus::Subscribe<Key>(1, [](Key const &key) {
        std::cout << "[Lambda] Key: " << key.code << std::endl;
    });

    // Using function with a lower priority
    MessageBus::Subscribe<Key>(2, onKey);

    // Using a callable object with a higher priority
    MessageBus::Subscribe<Key>(0, OnKeyCallable{});

    // Sending event that are queued
    MessageBus::Send(Key{32});
    MessageBus::Send(Key{64});

    // Dispatching them
    MessageBus::Dispatch();

    return 0;
}