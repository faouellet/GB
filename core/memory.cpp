#include "memory.h"

#include <algorithm>

void Memory::LoadROMBank(const std::vector<uint8_t>& data)
{
    std::copy_n(data.begin(), 0x800, m_data.begin());
}

uint8_t Memory::Read(uint32_t offset) const
{
    return m_data[offset];
}

void Memory::Write(uint32_t offset, uint8_t value)
{
    m_data[offset] = value;
}