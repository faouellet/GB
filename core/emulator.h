#pragma once

#include "cpu.h"

#include <memory>
#include <string>

class Emulator
{
public:
    bool LoadCartridge(const std::string& filePath);
    void Play();

private:
    Memory m_mem;
    CPU m_cpu;
};