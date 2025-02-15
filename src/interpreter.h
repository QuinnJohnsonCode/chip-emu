#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "display_manager.h"
#include <format>
#include <cstdint>
#include <array>
#include <stack>
#include <string>


class Interpreter
{
public:
    // Methods
    Interpreter() = default;

    void run();
    void load_rom_into_memory(const std::string& path);
    void display_memory(uint16_t start_addr);
    void display_memory(uint16_t start_addr, uint16_t end_addr);

private:
    // Registers
    std::uint16_t pc{};
    std::uint16_t index{};
    // General Variable Registers
    std::array<std::uint8_t, 16> registers{};
    // Stack
    std::stack<std::uint16_t> stack{};
    std::uint8_t sp{};
    // Memory
    std::array<std::uint8_t, 4096> memory{};
    // Timers
    std::uint8_t delay_timer{};
    std::uint8_t sound_timer{};
    // Screen (64 * 32 addressable pixels (0/1 monochrome))
    std::array<std::uint32_t, 64 * 32> display{};
    // Keypad
    std::array<std::uint8_t, 16> keypad{};

    // Constants
    static constexpr std::uint16_t START_ADDRESS = 0x200;
    mutable std::uint16_t END_ADDRESS = 0x200;

    DisplayManager display_manager{};
};


#endif