#pragma once

#include "cpu.h"

#include <string>

class Emulator
{
public:
    bool LoadROM(const std::string& romFilePath);
    void Play();

private:
    CPU m_cpu;
};