#include "emulator.h"

#include <fstream>
#include <iterator>

bool Emulator::LoadROM(const std::string& romFilePath)
{
    std::ifstream inputStream{romFilePath, std::ios::binary};
    if(inputStream)
    {
        std::vector<uint8_t> gameData{std::istreambuf_iterator<char>(inputStream),
                                      std::istreambuf_iterator<char>()};       
    }
}

void Emulator::Play()
{
    for(;;)
    {
        m_cpu.ExecuteNextInstruction();
    }
}