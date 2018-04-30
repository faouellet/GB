#pragma once

#include "memory.h"

#include <functional>

class CPU
{
public:
    CPU();

    void ExecuteNextInstruction();

private:
    template <uint8_t TReg>
    void LD();

private:
    enum Register
    {
        A, F, B, C, D, E, H, L
    };

    enum class FlagMask
    {
        Z = 0x0001,
        N = 0x0010,
        H = 0x0100,
        C = 0x1000,
    };

    std::array<uint8_t, 8> m_GPRegs;

    uint16_t m_SP;
    uint16_t m_PC;

    Memory m_mem;
    std::array<std::function<void()>, 256> m_commands;
};


template <uint8_t TReg>
void CPU::LD()
{
    const uint8_t val = m_mem.Read(++m_PC);
    m_GPRegs[TReg] = val;
}