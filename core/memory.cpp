#include "memory.h"

uint8_t Memory::Read(uint8_t offset) const
{
    return m_data[offset];
}

void Memory::Write(uint8_t offset, uint8_t value)
{
    m_data[offset] = value;
}