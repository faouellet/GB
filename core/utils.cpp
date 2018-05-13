#include "utils.h"

std::vector<unsigned int> GetSetBitPositions(uint64_t val)
{
    std::vector<unsigned int> positions{};
    unsigned int curPos{};
    while(val != 0)
    {
        ++curPos;

        if(val & 1)
        {
            positions.push_back(curPos);
        }
    }

    return positions;
}