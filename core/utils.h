#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

constexpr unsigned int GetNbSetBits(uint64_t val)
{
    unsigned int count{};
    while(val != 0)
    {
        count += val & 1;
        val >>= 1;
    }

    return count;
}

constexpr unsigned int GetSetBitPosition(uint64_t val)
{
    assert(val != 0 && "No bit set in 0");
    assert((val & (val - 1)) == 0 && "More than one bit set");

    unsigned int pos{};
    while(!(val & 1))
    {
        ++pos;
        val >>= 1;
    }

    return pos;
}

std::vector<unsigned int> GetSetBitPositions(uint64_t val);
