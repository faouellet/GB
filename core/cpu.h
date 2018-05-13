#pragma once

#include "memory.h"
#include "utils.h"

#include <functional>
#include <type_traits>

class CPU
{
public:
    CPU();

    void ExecuteNextInstruction();
    void Reset();

private:
    enum class RegisterMask : uint8_t
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
        Z = 0b10000000,
        N = 0b01000000,
        H = 0b00100000,
        C = 0b00010000,
    };

private:
    // Loads
    template <uint8_t Reg>
    void LD();

    template <uint8_t LHS, uint8_t RHS>
    void LD();

    // ALU operations
    template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
    void ExecuteBinaryALU(uint8_t val, TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers);

    template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
    void ExecuteBinaryALU(RegisterMask reg, TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers);

    template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
    void ExecuteBinaryALU(TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers);

    template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
    void ExecuteUnaryALU(RegisterMask reg, TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers);

    // Stack pointer manipulation
    template <uint16_t Reg>
    void PUSH();

    template <uint16_t Reg>
    void POP();

    // Utility
    constexpr uint16_t GetMemAddr(uint8_t reg);

private:
    static constexpr int m_NB_REGISTERS = 8;
    static constexpr int m_ACC_REGISTER_IDX = 0;
    static constexpr int m_FLAG_REGISTER_IDX = 1;
    std::array<uint8_t, m_NB_REGISTERS>  m_GPRegs;

    uint16_t m_SP;
    uint16_t m_PC;

    Memory m_mem;
};

template <uint8_t Reg>
void CPU::LD()
{
    static_assert(GetNbSetBits(Reg) == 1, "Immediate load only works with 8-bit registers");
    
    constexpr unsigned int regIdx = GetSetBitPosition(Reg);
    static_assert(regIdx != m_FLAG_REGISTER_IDX, "Loading into flag register is forbidden");
    
    m_GPRegs[regIdx] = m_mem.Read(m_PC++);
}

template <uint8_t LHS, uint8_t RHS>
void CPU::LD()
{
    constexpr unsigned int nbBitsSetLHS = GetNbSetBits(LHS);
    constexpr unsigned int nbBitsSetRHS = GetNbSetBits(RHS);

    if constexpr(nbBitsSetLHS == 1)
    {
        constexpr unsigned int idx = GetSetBitPosition(LHS);

        if constexpr(nbBitsSetRHS == 1)
        {
            m_GPRegs[idx] = m_GPRegs[GetSetBitPosition(RHS)];
        }
        else if constexpr(nbBitsSetRHS == 2)
        {

            m_GPRegs[idx] = m_mem.Read(GetMemAddr(RHS));
        }
    }
    else if constexpr(nbBitsSetLHS == 2)
    {
        if constexpr(nbBitsSetRHS == 1)
        {
            m_mem.Write(GetMemAddr(LHS), m_GPRegs[GetSetBitPosition(RHS)]);
        }
    }
}

constexpr uint16_t CPU::GetMemAddr(uint8_t reg)
{
    assert(GetNbSetBits(reg) <= 2);

    uint16_t addr{};
    uint8_t pos{};

    while(reg != 0)
    {
        if(reg & 1)
        {
            addr <<= 8;
            addr |= m_GPRegs[pos];
        }

        ++pos;
        reg >>= 1;
    }

    return addr;
}

template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
void CPU::ExecuteBinaryALU(uint8_t val, TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers)
{
    static_assert(std::is_invocable_v<TOp, uint8_t, uint8_t>, "Incorrect ALU operation");
    static_assert(std::is_invocable_v<TFlagSetter>, "Incorrect flag setter");
    //(static_assert(std::is_invocable_v<decltype(handlers), uint8_t, uint8_t>, "TODO"), ...);

    const uint8_t lhsVal = m_GPRegs[m_ACC_REGISTER_IDX];
    const uint8_t result = op(lhsVal, val);
    m_GPRegs[m_ACC_REGISTER_IDX] = result;

    // Set zero flag if result is 0
    m_GPRegs[m_FLAG_REGISTER_IDX] |= (result == 0 ? 0b10000000 : 0);

    // Unconditional flag setting
    flagSet();

    // Conditional flag setting
    (handlers(lhsVal, val), ...);
}

template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
void CPU::ExecuteBinaryALU(RegisterMask reg, TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers)
{
    const uint8_t regBits = static_cast<std::underlying_type_t<RegisterMask>>(reg);
    unsigned int nbBitsSet = GetNbSetBits(regBits);
    
    if (nbBitsSet == 1)
    {
        const uint8_t regIdx = GetSetBitPosition(static_cast<std::underlying_type_t<RegisterMask>>(reg));
        assert(regIdx < m_NB_REGISTERS && "Invalid register");

        ExecuteBinaryALU(m_GPRegs[regIdx], op, flagSet, handlers...);
    }
    else
    {
        ExecuteBinaryALU(m_mem.Read(GetMemAddr(regBits)), op, flagSet, handlers...);
    }
}

template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
void CPU::ExecuteBinaryALU(TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers)
{
    ExecuteBinaryALU(m_mem.Read(m_PC++), op, flagSet, handlers...);
}

template <typename TOp, typename TFlagSetter, typename... TFlagHandlers>
void CPU::ExecuteUnaryALU(RegisterMask reg, TOp&& op, TFlagSetter&& flagSet, TFlagHandlers&&... handlers)
{
    static_assert(std::is_invocable_v<TOp, uint8_t>, "Incorrect ALU operation");
    static_assert(std::is_invocable_v<TFlagSetter>, "Incorrect flag setter");

    const uint8_t regBits = static_cast<std::underlying_type_t<RegisterMask>>(reg);
    unsigned int nbBitsSet = GetNbSetBits(regBits);

    uint8_t result{};
    if (nbBitsSet == 1)
    {
        const uint8_t regIdx = GetSetBitPosition(static_cast<std::underlying_type_t<RegisterMask>>(reg));
        assert(regIdx < m_NB_REGISTERS && "Invalid register");

        result = op(m_GPRegs[regIdx]);
        m_GPRegs[regIdx] = result;
    }
    else
    {
        const uint16_t memAddr = GetMemAddr(regBits);
        result = op(m_mem.Read(memAddr));
        m_mem.Write(memAddr, result);
    }
    
    // Set zero flag if result is 0
    m_GPRegs[m_FLAG_REGISTER_IDX] |= (result == 0 ? 0b10000000 : 0);

    // Unconditional flag setting
    flagSet();

    // Conditional flag setting
    //(handlers(lhsVal, val), ...);
}

template <uint16_t Reg>
void CPU::PUSH()
{
    const uint8_t regBits = static_cast<std::underlying_type_t<RegisterMask>>(Reg);
    unsigned int nbBitsSet = GetNbSetBits(regBits);
    assert(nbBitsSet == 2 && "Push only works with register pairs");

    for(auto pos : GetSetBitPositions(Reg))
    {
        m_mem.Write(m_SP--, m_GPRegs[pos]);
    }
}

template <uint16_t Reg>
void CPU::POP()
{
    const uint8_t regBits = static_cast<std::underlying_type_t<RegisterMask>>(Reg);
    unsigned int nbBitsSet = GetNbSetBits(regBits);
    assert(nbBitsSet == 2 && "Push only works with register pairs");

    for(auto pos : GetSetBitPositions(Reg))
    {
        m_GPRegs[pos] = m_mem.Read(++m_SP);
    }
}
