#include <iostream>
#include <cstdint>
#include <array>

class Interpreter
{
public:
    // Registers
    uint16_t pc{};
    uint16_t index{};

    // General Variable Registers
    std::array<uint8_t, 16> registers{};

    // Stack
    std::array<uint16_t, 16> stack{};
    uint8_t sp{};

    // Memory
    std::array<uint8_t, 4096> memory{};
    
    // Timers
    uint8_t delay_timer{};
    uint8_t sound_timer{};

    // Screen (64 * 32 addressable pixels (0/1 monochrome))
    std::array<uint32_t, 64 * 32> display{};
};

int main()
{

    std::cout << "Hello\n";
    return 0;
}