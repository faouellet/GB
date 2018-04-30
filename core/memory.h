#pragma once

#include <array>
#include <cstdint>

class Memory
{
public:
    uint8_t Read(uint8_t offset) const;
    void Write(uint8_t offset, uint8_t value);

private:
    std::array<uint8_t, 0xFFFF> m_data;
};