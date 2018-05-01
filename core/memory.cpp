#include "memory.h"

uint8_t Memory::Read(uint32_t offset) const
{
    return m_data[offset];
}

void Memory::Write(uint32_t offset, uint8_t value)
{
    m_data[offset] = value;
}