#pragma once

#include <cstdint>
#include <type_traits>

template <uint64_t TVal>
constexpr unsigned int GetNbSetBits()
{
    unsigned int count{};
    uint64_t val = TVal;
    while(val != 0)
    {
        count += val & 1;
        val >>= 1;
    }

    return count;
}