#include <format>
#include <iostream>
#include <cstdint>
#include <array>
#include <stack>
#include <string>
#include <fstream>

class Font
{
    std::array<uint8_t, 5 * 16> sprites{{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    }};
};

class Interpreter
{
public:
    // Registers
    uint16_t pc{};
    uint16_t index{};
    // General Variable Registers
    std::array<uint8_t, 16> registers{};
    // Stack
    std::stack<uint16_t> stack{};
    uint8_t sp{};
    // Memory
    std::array<uint8_t, 4096> memory{};
    // Timers
    uint8_t delay_timer{};
    uint8_t sound_timer{};
    // Screen (64 * 32 addressable pixels (0/1 monochrome))
    std::array<uint32_t, 64 * 32> display{};
    // Keypad
    std::array<uint8_t, 16> keypad{};

    uint16_t END_ADDRESS = 0x200;

    // Constants
    const uint16_t START_ADDRESS = 0x200;

    // Methods
    Interpreter() = default;


    void load_rom_into_memory(std::string path)
    {
        std::fstream rom{path, rom.binary | rom.in};

        if (!rom.is_open())
            std::cout << "Failed to open " << path << '\n';
        else
        {
            uint8_t byte;
            int i = 0;
            while (rom.read(reinterpret_cast<char*>(&byte), sizeof byte))
                memory[START_ADDRESS + i++] = byte;

            END_ADDRESS = START_ADDRESS + i;
        }
    }
    
    void display_memory(uint16_t start_addr)
    {
        display_memory(start_addr, END_ADDRESS);
    }

    void display_memory(uint16_t start_addr, uint16_t end_addr)
    {
        const char SEPARATOR = ' ';
        const char WIDTH = 6;
        for (int i = start_addr; i < end_addr; ++i)
        {            
            if ((i - start_addr) % WIDTH == 0 && i != start_addr)
                std::cout << '\n';
            std::cout << std::format("{:#04x}", memory[i]) << SEPARATOR;

        }
        std::cout << '\n';
    }
};

int main()
{
    Interpreter chip{};
    chip.load_rom_into_memory("IBM.ch8");
    chip.display_memory(0x200);

    return 0;
}