#pragma once

#include <array>
#include <cstdint>
#include <vector>

class Memory
{
public:
    void LoadROMBank(const std::vector<uint8_t>& data);
    uint8_t Read(uint32_t offset) const;
    void Write(uint32_t offset, uint8_t value);

private:
    std::array<uint8_t, 0xFFFF> m_data;
};