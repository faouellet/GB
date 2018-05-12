#include "emulator.h"

#include <fstream>
#include <iterator>

bool Emulator::LoadCartridge(const std::string& filePath)
{
    std::ifstream inputStream{filePath, std::ios::binary};
    if(inputStream)
    {
        std::vector<uint8_t> gameData{std::istreambuf_iterator<char>(inputStream),
                                      std::istreambuf_iterator<char>()};

        m_mem.LoadROMBank(gameData);
        return true;
    }

    return false;
}

void Emulator::Play()
{
    m_cpu.Reset();

    for(;;)
    {
        m_cpu.ExecuteNextInstruction();
    }
}