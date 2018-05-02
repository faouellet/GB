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
    template <uint8_t TReg>
    void LD();

    template <uint8_t TLHS, uint8_t TRHS>
    void LD();

    template <uint8_t TReg>
    constexpr uint16_t GetMemAddr();

private:
    enum RegisterMask : uint8_t
    {
        // 8-bits registers mask
        A = 0b00000001,
        F = 0b00000010,
        B = 0b00000100,
        C = 0b00001000,
        D = 0b00010000,
        E = 0b00100000,
        H = 0b01000000,
        L = 0b10000000,

        // 16-bits registers mask
        AF = A + F,
        BC = B + C,
        DE = D + E,
        HL = H + L
    };

    enum class FlagMask : uint8_t
    {
        Z = 0b0001,
        N = 0b0010,
        H = 0b0100,
        C = 0b1000,
    };

private:
    std::array<uint8_t, 8>  m_GPRegs;

    uint16_t m_SP;
    uint16_t m_PC;

    Memory m_mem;
    std::array<std::function<void()>, 256> m_commands;
};


template <uint8_t TReg>
void CPU::LD()
{
    static_assert(GetNbSetBits<TReg>() == 1, "Immediate load only works with 8-bit registers");
    m_GPRegs[TReg] = m_mem.Read(m_PC++);
}

template <uint8_t TLHS, uint8_t TRHS>
void CPU::LD()
{
    constexpr unsigned int nbBitsSetLHS = GetNbSetBits<TLHS>();
    constexpr unsigned int nbBitsSetRHS = GetNbSetBits<TRHS>();

    if constexpr(nbBitsSetLHS == 1)
    {
        constexpr unsigned int idx = GetSetBitPosition<TLHS>();

        if constexpr(nbBitsSetRHS == 1)
        {
            m_GPRegs[idx] = m_GPRegs[GetSetBitPosition<TRHS>()];
        }
        else if constexpr(nbBitsSetRHS == 2)
        {

            m_GPRegs[idx] = m_mem.Read(GetMemAddr<TRHS>());
        }
    }
    else if constexpr(nbBitsSetLHS == 2)
    {
        if constexpr(nbBitsSetRHS == 1)
        {
            m_mem.Write(GetMemAddr<TLHS>(), m_GPRegs[GetSetBitPosition<TRHS>()]);
        }
    }
}

template <uint8_t TReg>
constexpr uint16_t CPU::GetMemAddr()
{
    static_assert(GetNbSetBits<TReg>() <= 2);

    uint16_t addr{};
    uint8_t pos{};
    uint8_t val = TReg;

    while(val != 0)
    {
        if(val & 1)
        {
            addr <<= 8;
            addr |= m_GPRegs[pos];
        }

        ++pos;
        val >>= 1;
    }

    return addr;
}
