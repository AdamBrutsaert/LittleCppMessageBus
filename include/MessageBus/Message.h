#pragma once

#include <typeinfo>

using MessageID = std::size_t;

template <typename M>
inline constexpr MessageID GetMessageID() noexcept
{
    return typeid(M).hash_code();
}