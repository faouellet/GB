#include "cpu.h"

#include <cassert>

CPU::CPU()
{
    Reset();
}

void CPU::ExecuteNextInstruction()
{
#define Flag(a) static_cast<std::underlying_type_t<FlagMask>>(FlagMask::a)
#define Reg(a) static_cast<std::underlying_type_t<RegisterMask>>(RegisterMask::a)

    auto setFlags   = [this](uint8_t mask) { return [this, mask](){ m_GPRegs[m_FLAG_REGISTER_IDX] |= mask; }; };
    auto resetFlags = [this](uint8_t mask) { return [this, mask](){ m_GPRegs[m_FLAG_REGISTER_IDX] &= ~mask; }; };

    auto setCarryFlags = [this](uint8_t lhs, uint8_t rhs)
    {
        const bool isSub = m_GPRegs[m_FLAG_REGISTER_IDX] & Flag(N);
        const uint32_t res = isSub ? (lhs - rhs) : (lhs + rhs);
        const uint32_t carryBits = lhs ^ rhs ^ res;
        m_GPRegs[m_FLAG_REGISTER_IDX] |= (carryBits & 0x10) ? Flag(H) : 0;
        m_GPRegs[m_FLAG_REGISTER_IDX] |= (carryBits & 0x100) ? Flag(C) : 0;
    };

    auto ADC = [this](uint8_t lhs, uint8_t rhs)
    {
        return lhs + rhs + m_GPRegs[m_FLAG_REGISTER_IDX];
    };

    auto SBC = [this](uint8_t lhs, uint8_t rhs)
    {
        return lhs - (rhs + m_GPRegs[m_FLAG_REGISTER_IDX]);
    };

    uint8_t opcode = m_mem.Read(m_PC++);
    
    switch(opcode)
    {
        case 0x00: [](){}; break;
        case 0x01: [](){}; break;
        case 0x02: LD<Reg(BC), Reg(A)>(); break;
        case 0x03: [](){}; break;
        case 0x04: [](){}; break;
        case 0x05: [](){}; break;
        case 0x06: LD<Reg(B)>(); break;
        case 0x07: [](){}; break;
        case 0x08: [](){}; break;
        case 0x09: [](){}; break;
        case 0x0A: LD<Reg(A), Reg(BC)>(); break;
        case 0x0B: [](){}; break;
        case 0x0C: [](){}; break;
        case 0x0D: [](){}; break;
        case 0x0E: LD<Reg(C)>(); break;
        case 0x0F: [](){}; break;

        case 0x10: [](){}; break;
        case 0x11: [](){}; break;
        case 0x12: LD<Reg(DE), Reg(A)>(); break;
        case 0x13: [](){}; break;
        case 0x14: [](){}; break;
        case 0x15: [](){}; break;
        case 0x16: LD<Reg(D)>(); break;
        case 0x17: [](){}; break;
        case 0x18: [](){}; break;
        case 0x19: [](){}; break;
        case 0x1A: LD<Reg(A), Reg(DE)>(); break;
        case 0x1B: [](){}; break;
        case 0x1C: [](){}; break;
        case 0x1D: [](){}; break;
        case 0x1E: LD<Reg(E)>(); break;
        case 0x1F: [](){}; break;

        case 0x20: [](){}; break;
        case 0x21: [](){}; break;
        case 0x22: [](){}; break;
        case 0x23: [](){}; break;
        case 0x24: [](){}; break;
        case 0x25: [](){}; break;
        case 0x26: LD<Reg(H)>(); break;
        case 0x27: [](){}; break;
        case 0x28: [](){}; break;
        case 0x29: [](){}; break;
        case 0x2A: [](){}; break;
        case 0x2B: [](){}; break;
        case 0x2C: [](){}; break;
        case 0x2D: [](){}; break;
        case 0x2E: LD<Reg(L)>(); break;
        case 0x2F: [](){}; break;

        case 0x30: [](){}; break;
        case 0x31: [](){}; break;
        case 0x32: [](){}; break;
        case 0x33: [](){}; break;
        case 0x34: [](){}; break;
        case 0x35: [](){}; break;
        case 0x36: [](){}; break;
        case 0x37: [](){}; break;
        case 0x38: [](){}; break;
        case 0x39: [](){}; break;
        case 0x3A: [](){}; break;
        case 0x3B: [](){}; break;
        case 0x3C: [](){}; break;
        case 0x3D: [](){}; break;
        case 0x3E: LD<Reg(A)>(); break;
        case 0x3F: [](){}; break;

        // LD B, #
        case 0x40: LD<Reg(B), Reg(B)>(); break;
        case 0x41: LD<Reg(B), Reg(C)>(); break;
        case 0x42: LD<Reg(B), Reg(D)>(); break;
        case 0x43: LD<Reg(B), Reg(E)>(); break;
        case 0x44: LD<Reg(B), Reg(H)>(); break;
        case 0x45: LD<Reg(B), Reg(L)>(); break;
        case 0x46: LD<Reg(B), Reg(HL)>(); break;
        case 0x47: LD<Reg(B), Reg(A)>(); break;

        // LD C, #
        case 0x48: LD<Reg(C), Reg(B)>(); break;
        case 0x49: LD<Reg(C), Reg(C)>(); break;
        case 0x4A: LD<Reg(C), Reg(D)>(); break;
        case 0x4B: LD<Reg(C), Reg(E)>(); break;
        case 0x4C: LD<Reg(C), Reg(H)>(); break;
        case 0x4D: LD<Reg(C), Reg(L)>(); break;
        case 0x4E: LD<Reg(C), Reg(HL)>(); break;
        case 0x4F: LD<Reg(C), Reg(A)>(); break;

        // LD D, #
        case 0x50: LD<Reg(D), Reg(B)>(); break;
        case 0x51: LD<Reg(D), Reg(C)>(); break;
        case 0x52: LD<Reg(D), Reg(D)>(); break;
        case 0x53: LD<Reg(D), Reg(E)>(); break;
        case 0x54: LD<Reg(D), Reg(H)>(); break;
        case 0x55: LD<Reg(D), Reg(L)>(); break;
        case 0x56: LD<Reg(D), Reg(HL)>(); break;
        case 0x57: LD<Reg(D), Reg(A)>(); break;

        // LD E, #
        case 0x58: LD<Reg(E), Reg(B)>(); break;
        case 0x59: LD<Reg(E), Reg(C)>(); break;
        case 0x5A: LD<Reg(E), Reg(D)>(); break;
        case 0x5B: LD<Reg(E), Reg(E)>(); break;
        case 0x5C: LD<Reg(E), Reg(H)>(); break;
        case 0x5D: LD<Reg(E), Reg(L)>(); break;
        case 0x5E: LD<Reg(E), Reg(HL)>(); break;
        case 0x5F: LD<Reg(E), Reg(A)>(); break;

        // LD H, #
        case 0x60: LD<Reg(H), Reg(B)>(); break;
        case 0x61: LD<Reg(H), Reg(C)>(); break;
        case 0x62: LD<Reg(H), Reg(D)>(); break;
        case 0x63: LD<Reg(H), Reg(E)>(); break;
        case 0x64: LD<Reg(H), Reg(H)>(); break;
        case 0x65: LD<Reg(H), Reg(L)>(); break;
        case 0x66: LD<Reg(H), Reg(HL)>(); break;
        case 0x67: LD<Reg(H), Reg(A)>(); break;

        // LD L, #
        case 0x68: LD<Reg(L), Reg(B)>(); break;
        case 0x69: LD<Reg(L), Reg(C)>(); break;
        case 0x6A: LD<Reg(L), Reg(D)>(); break;
        case 0x6B: LD<Reg(L), Reg(E)>(); break;
        case 0x6C: LD<Reg(L), Reg(H)>(); break;
        case 0x6D: LD<Reg(L), Reg(L)>(); break;
        case 0x6E: LD<Reg(L), Reg(HL)>(); break;
        case 0x6F: LD<Reg(L), Reg(A)>(); break;

        // LD (HL), #
        case 0x70: LD<Reg(HL), Reg(B)>(); break;
        case 0x71: LD<Reg(HL), Reg(C)>(); break;
        case 0x72: LD<Reg(HL), Reg(D)>(); break;
        case 0x73: LD<Reg(HL), Reg(E)>(); break;
        case 0x74: LD<Reg(HL), Reg(H)>(); break;
        case 0x75: LD<Reg(HL), Reg(L)>(); break;
        case 0x76: [](){}; break;
        case 0x77: LD<Reg(HL), Reg(A)>(); break;

        // LD A, #
        case 0x78: LD<Reg(A), Reg(B)>(); break;
        case 0x79: LD<Reg(A), Reg(C)>(); break;
        case 0x7A: LD<Reg(A), Reg(D)>(); break;
        case 0x7B: LD<Reg(A), Reg(E)>(); break;
        case 0x7C: LD<Reg(A), Reg(H)>(); break;
        case 0x7D: LD<Reg(A), Reg(L)>(); break;
        case 0x7E: LD<Reg(A), Reg(HL)>(); break;
        case 0x7F: LD<Reg(A), Reg(A)>(); break;
    
        // ADD
        case 0x80: ExecuteALU(RegisterMask::B, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x81: ExecuteALU(RegisterMask::C, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x82: ExecuteALU(RegisterMask::D, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x83: ExecuteALU(RegisterMask::E, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x84: ExecuteALU(RegisterMask::H, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x85: ExecuteALU(RegisterMask::L, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x86: ExecuteALU(RegisterMask::HL, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x87: ExecuteALU(RegisterMask::A, std::plus<uint8_t>{}, resetFlags(Flag(N)), setCarryFlags); break;

        // ADC
        case 0x88: ExecuteALU(RegisterMask::B, ADC, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x89: ExecuteALU(RegisterMask::C, ADC, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x8A: ExecuteALU(RegisterMask::D, ADC, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x8B: ExecuteALU(RegisterMask::E, ADC, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x8C: ExecuteALU(RegisterMask::H, ADC, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x8D: ExecuteALU(RegisterMask::L, ADC, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x8E: ExecuteALU(RegisterMask::HL, ADC, resetFlags(Flag(N)), setCarryFlags); break;
        case 0x8F: ExecuteALU(RegisterMask::A, ADC, resetFlags(Flag(N)), setCarryFlags); break;
    
        // SUB
        case 0x90: ExecuteALU(RegisterMask::B, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;
        case 0x91: ExecuteALU(RegisterMask::C, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;
        case 0x92: ExecuteALU(RegisterMask::D, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;
        case 0x93: ExecuteALU(RegisterMask::E, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;
        case 0x94: ExecuteALU(RegisterMask::H, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;
        case 0x95: ExecuteALU(RegisterMask::L, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;
        case 0x96: ExecuteALU(RegisterMask::HL, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;
        case 0x97: ExecuteALU(RegisterMask::A, std::minus<uint8_t>{}, setFlags(Flag(N)), setCarryFlags); break;

        // SBC
        case 0x98: ExecuteALU(RegisterMask::B, SBC, setFlags(Flag(N)), setCarryFlags); break;
        case 0x99: ExecuteALU(RegisterMask::C, SBC, setFlags(Flag(N)), setCarryFlags); break;
        case 0x9A: ExecuteALU(RegisterMask::D, SBC, setFlags(Flag(N)), setCarryFlags); break;
        case 0x9B: ExecuteALU(RegisterMask::E, SBC, setFlags(Flag(N)), setCarryFlags); break;
        case 0x9C: ExecuteALU(RegisterMask::H, SBC, setFlags(Flag(N)), setCarryFlags); break;
        case 0x9D: ExecuteALU(RegisterMask::L, SBC, setFlags(Flag(N)), setCarryFlags); break;
        case 0x9E: ExecuteALU(RegisterMask::HL, SBC, setFlags(Flag(N)), setCarryFlags); break;
        case 0x9F: ExecuteALU(RegisterMask::A, SBC, setFlags(Flag(N)), setCarryFlags); break;

        // AND
        case 0xA0: ExecuteALU(RegisterMask::B, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        case 0xA1: ExecuteALU(RegisterMask::C, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        case 0xA2: ExecuteALU(RegisterMask::D, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        case 0xA3: ExecuteALU(RegisterMask::E, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        case 0xA4: ExecuteALU(RegisterMask::H, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        case 0xA5: ExecuteALU(RegisterMask::L, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        case 0xA6: ExecuteALU(RegisterMask::HL, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        case 0xA7: ExecuteALU(RegisterMask::A, std::bit_and<uint8_t>{}, resetFlags(Flag(N) | ~Flag(H) | Flag(C))); break;
        
        // XOR
        case 0xA8: ExecuteALU(RegisterMask::B, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xA9: ExecuteALU(RegisterMask::C, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xAA: ExecuteALU(RegisterMask::D, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xAB: ExecuteALU(RegisterMask::E, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xAC: ExecuteALU(RegisterMask::H, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xAD: ExecuteALU(RegisterMask::L, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xAE: ExecuteALU(RegisterMask::HL, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xAF: ExecuteALU(RegisterMask::A, std::bit_xor<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;

        // OR
        case 0xB0: ExecuteALU(RegisterMask::B, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xB1: ExecuteALU(RegisterMask::C, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xB2: ExecuteALU(RegisterMask::D, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xB3: ExecuteALU(RegisterMask::E, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xB4: ExecuteALU(RegisterMask::H, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xB5: ExecuteALU(RegisterMask::L, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xB6: ExecuteALU(RegisterMask::HL, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;
        case 0xB7: ExecuteALU(RegisterMask::A, std::bit_or<uint8_t>{}, resetFlags(Flag(N) | Flag(H) | Flag(C))); break;

        case 0xB8: [](){}; break;
        case 0xB9: [](){}; break;
        case 0xBA: [](){}; break;
        case 0xBB: [](){}; break;
        case 0xBC: [](){}; break;
        case 0xBD: [](){}; break;
        case 0xBE: [](){}; break;
        case 0xBF: [](){}; break;

        case 0xC0: [](){}; break;
        case 0xC1: POP<Reg(BC)>(); break;
        case 0xC2: [](){}; break;
        case 0xC3: [](){}; break;
        case 0xC4: [](){}; break;
        case 0xC5: PUSH<Reg(BC)>(); break;
        case 0xC6: [](){}; break;
        case 0xC7: [](){}; break;
        case 0xC8: [](){}; break;
        case 0xC9: [](){}; break;
        case 0xCA: [](){}; break;
        case 0xCB: [](){}; break;
        case 0xCC: [](){}; break;
        case 0xCD: [](){}; break;
        case 0xCE: [](){}; break;
        case 0xCF: [](){}; break;

        case 0xD0: [](){}; break;
        case 0xD1: POP<Reg(DE)>(); break;
        case 0xD2: [](){}; break;
        case 0xD3: assert("Invalid opcode"); break;
        case 0xD4: [](){}; break;
        case 0xD5: PUSH<Reg(DE)>(); break;
        case 0xD6: [](){}; break;
        case 0xD7: [](){}; break;
        case 0xD8: [](){}; break;
        case 0xD9: [](){}; break;
        case 0xDA: [](){}; break;
        case 0xDB: assert("Invalid opcode"); break;
        case 0xDC: [](){}; break;
        case 0xDD: assert("Invalid opcode"); break;
        case 0xDE: [](){}; break;
        case 0xDF: [](){}; break;

        case 0xE0: [](){}; break;
        case 0xE1: POP<Reg(HL)>(); break;
        case 0xE2: [](){}; break;
        case 0xE3: assert("Invalid opcode"); break;
        case 0xE4: assert("Invalid opcode"); break;
        case 0xE5: PUSH<Reg(HL)>(); break;
        case 0xE6: [](){}; break;
        case 0xE7: [](){}; break;
        case 0xE8: [](){}; break;
        case 0xE9: [](){}; break;
        case 0xEA: [](){}; break;
        case 0xEB: assert("Invalid opcode"); break;
        case 0xEC: assert("Invalid opcode"); break;
        case 0xED: assert("Invalid opcode"); break;
        case 0xEE: [](){}; break;
        case 0xEF: [](){}; break;

        case 0xF0: [](){}; break;
        case 0xF1: POP<Reg(AF)>(); break;
        case 0xF2: [](){}; break;
        case 0xF3: [](){}; break;
        case 0xF4: assert("Invalid opcode"); break;
        case 0xF5: PUSH<Reg(AF)>(); break;
        case 0xF6: [](){}; break;
        case 0xF7: [](){}; break;
        case 0xF8: [](){}; break;
        case 0xF9: [](){}; break;
        case 0xFA: [](){}; break;
        case 0xFB: [](){}; break;
        case 0xFC: assert("Invalid opcode"); break;
        case 0xFD: assert("Invalid opcode"); break;
        case 0xFE: [](){}; break;
        case 0xFF: [](){}; break;
    }

#undef Flag
#undef Reg
}

void CPU::Reset()
{
    m_PC = 0x100;
    m_SP = 0xFFFE;
    m_GPRegs.fill(0);
}
