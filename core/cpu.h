#pragma once

#include "memory.h"
#include "utils.h"

#include <functional>
#include <sstream>

class CPU
{
public:
    CPU();

    void ExecuteNextInstruction();

private:
    template <uint32_t TReg>
    void LD();

    template <uint64_t TLHS, uint64_t TRHS>
    void LD();

private:
    enum RegisterMask
    {
        // 8 bits
        A = 0b00000001,
        F = 0b00000010,
        B = 0b00000100,
        C = 0b00001000,
        D = 0b00010000,
        E = 0b00100000,
        H = 0b01000000,
        L = 0b10000000,

        // 16 bits
        AF = A + F,
        BC = B + C,
        DE = D + E,
        HL = H + L
    };

    enum class FlagMask
    {
        Z = 0b0001,
        N = 0b0010,
        H = 0b0100,
        C = 0b1000,
    };

    class Immediate{};
    class Indirect{};

private:
    std::array<uint8_t, 8>  m_GPRegs;

    uint16_t m_SP;
    uint16_t m_PC;

    Memory m_mem;
    std::array<std::function<void()>, 256> m_commands;
};


template <uint32_t TReg>
void CPU::LD()
{
    m_GPRegs[TReg] = m_mem.Read(m_PC++);
}

template <uint64_t TLHS, uint64_t TRHS>
void CPU::LD()
{
    constexpr unsigned int nbBitsSetLHS = GetNbSetBits<TLHS>();
    constexpr unsigned int nbBitsSetRHS = GetNbSetBits<TRHS>();

    if constexpr(nbBitsSetLHS == 1)
    {
        if constexpr(nbBitsSetRHS == 1)
        {
            m_GPRegs[TLHS] = m_GPRegs[TRHS];
        }
        else if constexpr(nbBitsSetRHS == 2)
        {
            m_GPRegs[TLHS] = m_mem.Read(TRHS);
        }
    }
    else if constexpr(nbBitsSetLHS == 2)
    {
        if constexpr(nbBitsSetRHS == 1)
        {
            m_GPRegs[TLHS] = m_GPRegs[TRHS];
        }
    }
}
