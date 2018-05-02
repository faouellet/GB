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

template <uint64_t TVal>
constexpr unsigned int GetSetBitPosition()
{
    static_assert(TVal != 0, "No bit set in 0");
    static_assert((TVal & (TVal - 1)) == 0, "More than one bit set");

    unsigned int pos{};
    uint64_t val = TVal;
    while(!(val & 1))
    {
        ++pos;
        val >>= 1;
    }

    return pos;
}
