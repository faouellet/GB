#include "cpu.h"

#include <cassert>

CPU::CPU()
{
    Reset();
}

void CPU::ExecuteNextInstruction()
{
#define FLAG(a) static_cast<std::underlying_type_t<FlagMask>>(FlagMask::a)
#define REG(a) static_cast<std::underlying_type_t<RegisterMask>>(RegisterMask::a)
#define ALU_OP(opcode, execALU, op, flagSet, ...) \
        case opcode:       execALU(RegisterMask::B,  op, flagSet, ##__VA_ARGS__); break; \
        case (opcode + 1): execALU(RegisterMask::C,  op, flagSet, ##__VA_ARGS__); break; \
        case (opcode + 2): execALU(RegisterMask::D,  op, flagSet, ##__VA_ARGS__); break; \
        case (opcode + 3): execALU(RegisterMask::E,  op, flagSet, ##__VA_ARGS__); break; \
        case (opcode + 4): execALU(RegisterMask::H,  op, flagSet, ##__VA_ARGS__); break; \
        case (opcode + 5): execALU(RegisterMask::L,  op, flagSet, ##__VA_ARGS__); break; \
        case (opcode + 6): execALU(RegisterMask::HL, op, flagSet, ##__VA_ARGS__); break; \
        case (opcode + 7): execALU(RegisterMask::A,  op, flagSet, ##__VA_ARGS__); break;

#define LD_OP(opcode, r)                                 \
        case (opcode):     LD<REG(r), REG(B)>();  break; \
        case (opcode + 1): LD<REG(r), REG(C)>();  break; \
        case (opcode + 2): LD<REG(r), REG(D)>();  break; \
        case (opcode + 3): LD<REG(r), REG(E)>();  break; \
        case (opcode + 4): LD<REG(r), REG(H)>();  break; \
        case (opcode + 5): LD<REG(r), REG(L)>();  break; \
        case (opcode + 6): LD<REG(r), REG(HL)>(); break; \
        case (opcode + 7): LD<REG(r), REG(A)>();  break;

    auto resetBits = [this](uint8_t reg, uint8_t mask) { return [this, mask, reg](){ ResetBits(reg, mask); }; };
    auto setBits   = [this](uint8_t reg, uint8_t mask) { return [this, mask, reg](){ SetBits(reg, mask); }; };

    auto setCarryFlags = [this](uint8_t lhs, uint8_t rhs)
    {
        const bool isSub = m_GPRegs[m_FLAG_REGISTER_IDX] & FLAG(N);
        const uint32_t res = isSub ? (lhs - rhs) : (lhs + rhs);
        const uint32_t carryBits = lhs ^ rhs ^ res;
        m_GPRegs[m_FLAG_REGISTER_IDX] |= (carryBits & 0x10) ? FLAG(H) : 0;
        m_GPRegs[m_FLAG_REGISTER_IDX] |= (carryBits & 0x100) ? FLAG(C) : 0;
    };

    auto ADC = [this](uint8_t lhs, uint8_t rhs) { return lhs + rhs + m_GPRegs[m_FLAG_REGISTER_IDX]; };
    auto SBC = [this](uint8_t lhs, uint8_t rhs) { return lhs - (rhs + m_GPRegs[m_FLAG_REGISTER_IDX]); };

    auto DEC = [](uint8_t val){ return --val; };
    auto INC = [](uint8_t val){ return ++val; };

    auto RST = [this](uint16_t addr){ m_mem.Write(m_SP--, m_PC); m_PC = addr; };

    auto SWAP = [](uint8_t val) 
    {
        const uint8_t lowerNibble = val & 0x0F;
        val >>= 4;
        return (lowerNibble << 4) | val;
    };

    uint8_t opcode = m_mem.Read(m_PC++);
    
    switch(opcode)
    {
        case 0x00: [](){}; break;
        case 0x01: [](){}; break;
        case 0x02: LD<REG(BC), REG(A)>(); break;
        case 0x03: [](){}; break;
        case 0x04: ExecuteUnaryALU(RegisterMask::B, INC, resetBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x05: ExecuteUnaryALU(RegisterMask::B, DEC, setBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x06: LD<REG(B)>(); break;
        case 0x07: [](){}; break;
        case 0x08: [](){}; break;
        case 0x09: [](){}; break;
        case 0x0A: LD<REG(A), REG(BC)>(); break;
        case 0x0B: [](){}; break;
        case 0x0C: ExecuteUnaryALU(RegisterMask::C, INC, resetBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x0D: ExecuteUnaryALU(RegisterMask::C, DEC, setBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x0E: LD<REG(C)>(); break;
        case 0x0F: [](){}; break;

        case 0x10: [](){}; break;
        case 0x11: [](){}; break;
        case 0x12: LD<REG(DE), REG(A)>(); break;
        case 0x13: [](){}; break;
        case 0x14: ExecuteUnaryALU(RegisterMask::D, INC, resetBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x15: ExecuteUnaryALU(RegisterMask::D, DEC, setBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x16: LD<REG(D)>(); break;
        case 0x17: [](){}; break;
        case 0x18: [](){}; break;
        case 0x19: [](){}; break;
        case 0x1A: LD<REG(A), REG(DE)>(); break;
        case 0x1B: [](){}; break;
        case 0x1C: ExecuteUnaryALU(RegisterMask::E, INC, resetBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x1D: ExecuteUnaryALU(RegisterMask::E, DEC, setBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x1E: LD<REG(E)>(); break;
        case 0x1F: [](){}; break;

        case 0x20: [](){}; break;
        case 0x21: [](){}; break;
        case 0x22: [](){}; break;
        case 0x23: [](){}; break;
        case 0x24: ExecuteUnaryALU(RegisterMask::H, INC, resetBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x25: ExecuteUnaryALU(RegisterMask::H, DEC, setBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x26: LD<REG(H)>(); break;
        case 0x27: [](){}; break;
        case 0x28: [](){}; break;
        case 0x29: [](){}; break;
        case 0x2A: [](){}; break;
        case 0x2B: [](){}; break;
        case 0x2C: ExecuteUnaryALU(RegisterMask::L, INC, resetBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x2D: ExecuteUnaryALU(RegisterMask::L, DEC, setBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x2E: LD<REG(L)>(); break;
        case 0x2F: [](){}; break;

        case 0x30: [](){}; break;
        case 0x31: [](){}; break;
        case 0x32: [](){}; break;
        case 0x33: ++m_SP; break;
        case 0x34: [](){}; break;
        case 0x35: [](){}; break;
        case 0x36: [](){}; break;
        case 0x37: [](){}; break;
        case 0x38: [](){}; break;
        case 0x39: [](){}; break;
        case 0x3A: [](){}; break;
        case 0x3B: --m_SP; break;
        case 0x3C: ExecuteUnaryALU(RegisterMask::A, INC, resetBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x3D: ExecuteUnaryALU(RegisterMask::A, DEC, setBits(REG(F), FLAG(N)), setCarryFlags); break;
        case 0x3E: LD<REG(A)>(); break;
        case 0x3F:  
            resetBits(REG(F), FLAG(N) | FLAG(H));
            // TODO
            break;

        // LD B, #
        LD_OP(0x40, B)

        // LD C, #
        LD_OP(0x48, C)

        // LD D, #
        LD_OP(0x50, D)

        // LD E, #
        LD_OP(0x58, B)
        
        // LD H, #
        LD_OP(0x60, H)

        // LD L, #
        LD_OP(0x68, L)

        // LD (HL), #
        case 0x70: LD<REG(HL), REG(B)>(); break;
        case 0x71: LD<REG(HL), REG(C)>(); break;
        case 0x72: LD<REG(HL), REG(D)>(); break;
        case 0x73: LD<REG(HL), REG(E)>(); break;
        case 0x74: LD<REG(HL), REG(H)>(); break;
        case 0x75: LD<REG(HL), REG(L)>(); break;
        case 0x76: [](){}; break;
        case 0x77: LD<REG(HL), REG(A)>(); break;

        // LD A, #
        LD_OP(0x78, A)
    
        // ADD
        ALU_OP(0x80, ExecuteBinaryALU, std::plus<uint8_t>{}, resetBits(REG(F), FLAG(N)), setCarryFlags)
        
        // ADC
        ALU_OP(0x88, ExecuteBinaryALU, ADC, resetBits(REG(F), FLAG(N)), setCarryFlags)
    
        // SUB
        ALU_OP(0x90, ExecuteBinaryALU, std::minus<uint8_t>{}, setBits(REG(F), FLAG(N)), setCarryFlags)

        // SBC
        ALU_OP(0x98, ExecuteBinaryALU, SBC, setBits(REG(F), FLAG(N)), setCarryFlags)

        // AND
        ALU_OP(0xA0, ExecuteBinaryALU, std::bit_and<uint8_t>{}, resetBits(REG(F), FLAG(N) | ~FLAG(H) | FLAG(C)))
        
        // XOR
        ALU_OP(0xA8, ExecuteBinaryALU, std::bit_xor<uint8_t>{}, resetBits(REG(F), FLAG(N) | FLAG(H) | FLAG(C)))

        // OR
        ALU_OP(0xB0, ExecuteBinaryALU, std::bit_or<uint8_t>{}, resetBits(REG(F), FLAG(N) | FLAG(H) | FLAG(C)))

        case 0xB8: [](){}; break;
        case 0xB9: [](){}; break;
        case 0xBA: [](){}; break;
        case 0xBB: [](){}; break;
        case 0xBC: [](){}; break;
        case 0xBD: [](){}; break;
        case 0xBE: [](){}; break;
        case 0xBF: [](){}; break;

        case 0xC0: [](){}; break;
        case 0xC1: POP<REG(BC)>(); break;
        case 0xC2: [](){}; break;
        case 0xC3: [](){}; break;
        case 0xC4: [](){}; break;
        case 0xC5: PUSH<REG(BC)>(); break;
        case 0xC6: [](){}; break;
        case 0xC7: RST(0x00); break;
        case 0xC8: [](){}; break;
        case 0xC9: [](){}; break;
        case 0xCA: [](){}; break;
        case 0xCB: 
            switch(m_mem.Read(m_PC++))
            {
                #define BIT_OP(opcode, op, mask)                          \
                    case (opcode):     op(REG(B),  mask); break; \
                    case (opcode + 1): op(REG(C),  mask); break; \
                    case (opcode + 2): op(REG(D),  mask); break; \
                    case (opcode + 3): op(REG(E),  mask); break; \
                    case (opcode + 4): op(REG(H),  mask); break; \
                    case (opcode + 5): op(REG(L),  mask); break; \
                    case (opcode + 6): op(REG(HL), mask); break; \
                    case (opcode + 7): op(REG(A),  mask); break;

                #define BIT_OP_FOR_ALL_BITS(opcode, op) \
                    BIT_OP(opcode,      op, 1)          \
                    BIT_OP(opcode + 8,  op, 1 << 1)     \
                    BIT_OP(opcode + 16, op, 1 << 2)     \
                    BIT_OP(opcode + 24, op, 1 << 3)     \
                    BIT_OP(opcode + 32, op, 1 << 4)     \
                    BIT_OP(opcode + 40, op, 1 << 5)     \
                    BIT_OP(opcode + 48, op, 1 << 6)     \
                    BIT_OP(opcode + 56, op, 1 << 7)

                // SWAP R
                ALU_OP(0x30, ExecuteUnaryALU, SWAP, resetBits(REG(F), FLAG(N) | FLAG(H) | FLAG(C)))

                // BIT b, R
                BIT_OP_FOR_ALL_BITS(0x40, [this](uint8_t reg, uint8_t mask){ TestBit(reg, mask); });

                // RES b, R
                BIT_OP_FOR_ALL_BITS(0x80, [this](uint8_t reg, uint8_t mask){ ResetBits(reg, mask); });

                // SET b, R
                BIT_OP_FOR_ALL_BITS(0xC0, [this](uint8_t reg, uint8_t mask){ SetBits(reg, mask); });

                #undef BIT_OP_FOR_ALL_BITS
                #undef BIT_OP             
            }
            break;
        case 0xCC: [](){}; break;
        case 0xCD: [](){}; break;
        case 0xCE: [](){}; break;
        case 0xCF: RST(0x08); break;

        case 0xD0: [](){}; break;
        case 0xD1: POP<REG(DE)>(); break;
        case 0xD2: [](){}; break;
        case 0xD3: assert("Invalid opcode"); break;
        case 0xD4: [](){}; break;
        case 0xD5: PUSH<REG(DE)>(); break;
        case 0xD6: [](){}; break;
        case 0xD7: RST(0x10); break;
        case 0xD8: [](){}; break;
        case 0xD9: [](){}; break;
        case 0xDA: [](){}; break;
        case 0xDB: assert("Invalid opcode"); break;
        case 0xDC: [](){}; break;
        case 0xDD: assert("Invalid opcode"); break;
        case 0xDE: [](){}; break;
        case 0xDF: RST(0x18); break;

        case 0xE0: [](){}; break;
        case 0xE1: POP<REG(HL)>(); break;
        case 0xE2: [](){}; break;
        case 0xE3: assert("Invalid opcode"); break;
        case 0xE4: assert("Invalid opcode"); break;
        case 0xE5: PUSH<REG(HL)>(); break;
        case 0xE6: [](){}; break;
        case 0xE7: RST(0x20); break;
        case 0xE8: [](){}; break;
        case 0xE9: [](){}; break;
        case 0xEA: [](){}; break;
        case 0xEB: assert("Invalid opcode"); break;
        case 0xEC: assert("Invalid opcode"); break;
        case 0xED: assert("Invalid opcode"); break;
        case 0xEE: [](){}; break;
        case 0xEF: RST(0x28); break;

        case 0xF0: [](){}; break;
        case 0xF1: POP<REG(AF)>(); break;
        case 0xF2: [](){}; break;
        case 0xF3: [](){}; break;
        case 0xF4: assert("Invalid opcode"); break;
        case 0xF5: PUSH<REG(AF)>(); break;
        case 0xF6: [](){}; break;
        case 0xF7: RST(0x30); break;
        case 0xF8: [](){}; break;
        case 0xF9: [](){}; break;
        case 0xFA: [](){}; break;
        case 0xFB: [](){}; break;
        case 0xFC: assert("Invalid opcode"); break;
        case 0xFD: assert("Invalid opcode"); break;
        case 0xFE: [](){}; break;
        case 0xFF: RST(0x38); break;
        default: assert("Unimplemented opcode"); break;
    }

#undef LD_OP
#undef ALU_OP
#undef FLAG
#undef REG
}

void CPU::Reset()
{
    m_PC = 0x100;
    m_SP = 0xFFFE;
    m_GPRegs.fill(0);
}

void CPU::ResetBits(uint8_t reg, uint8_t bitMask)
{
    m_GPRegs[GetSetBitPosition(reg)] &= ~bitMask;
}

void CPU::SetBits(uint8_t reg, uint8_t bitMask)
{
    m_GPRegs[GetSetBitPosition(reg)] |= bitMask;
}

void CPU::TestBit(uint8_t reg, uint8_t bitMask)
{
    assert((bitMask & (bitMask - 1)) == 0 && "More than one bit set");

    const unsigned int nbBitsSet = GetNbSetBits(reg);

    uint8_t val{};
    if (nbBitsSet == 1)
    {
        const uint8_t regIdx = GetSetBitPosition(reg);
        assert(regIdx < m_NB_REGISTERS && "Invalid register");

        val = m_GPRegs[regIdx];
    }
    else
    {
        const uint16_t memAddr = GetMemAddr(reg);
        val = m_mem.Read(memAddr);
    }

    const bool bitIsZero = ((bitMask & val) != bitMask);

    // Set zero flag if bit is 0
    m_GPRegs[m_FLAG_REGISTER_IDX] |= (bitIsZero ? 0b10000000 : 0);

    // Reset N flag
    m_GPRegs[m_FLAG_REGISTER_IDX] &= ~(static_cast<std::underlying_type_t<FlagMask>>(FlagMask::N));

    // Set H flag
    m_GPRegs[m_FLAG_REGISTER_IDX] |= (static_cast<std::underlying_type_t<FlagMask>>(FlagMask::H));
}

constexpr uint16_t CPU::GetMemAddr(uint8_t reg) const
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
