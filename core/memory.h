#pragma once

#include <array>
#include <cstdint>

class Memory
{
public:
    uint8_t Read(uint32_t offset) const;
    void Write(uint32_t offset, uint8_t value);

private:
    std::array<uint8_t, 0xFFFF> m_data;
};