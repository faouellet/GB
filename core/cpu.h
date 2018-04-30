#pragma once

#include "memory.h"

#include <functional>

class CPU
{
public:
    CPU();

    void ExecuteNextInstruction();

private:
    void InitCommands();

private:
    union
    {
        struct
        {
            uint8_t A, F, B, C, D, E, H, L;
        };

        struct
        {
            uint16_t AF, BC, DE, HL;
        };
    } m_GPRegs;

    uint16_t m_SP;
    uint16_t m_PC;
    
    struct FlagRegister
    {
        uint8_t Z : 1;
        uint8_t N : 1;
        uint8_t H : 1;
        uint8_t C : 1;
    } m_FR;

    Memory m_mem;
    std::array<std::function<void()>, 256> m_commands;
};