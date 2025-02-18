#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "display_manager.h"
#include "font.h"
#include <format>
#include <cstdint>
#include <array>
#include <stack>
#include <string>

struct instruction_parameters
{
    const std::uint8_t X;     // 4-bit
    const std::uint8_t Y;     // 4-bit
    const std::uint8_t N;     // 4-bit (high 4 bits are 0000)
    const std::uint8_t NN;
    const std::uint16_t NNN;  // 12-bit (high 4 bits are 0000)
};

class Interpreter
{
public:
    // Methods
    Interpreter() = default;

    void init();
    void run();
    void cycle();
    void fetch();
    void step_program_counter();
    void load_font_into_memory();
    void load_rom_into_memory(const std::string& path);
    void display_memory(uint16_t start_addr);
    void display_memory(uint16_t start_addr, uint16_t end_addr);
    void debug_ip(struct instruction_parameters& ip);

    /* Routines */
    void routine_00E0();
    void routine_6xnn(struct instruction_parameters& ip);
    void routine_Annn(struct instruction_parameters& ip);
    void routine_Dxyn(struct instruction_parameters& ip);
    void routine_1nnn(struct instruction_parameters& ip);
    void routine_7xnn(struct instruction_parameters& ip);

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

    // Instructions
    std::uint16_t curr_instruction{};

    // Constants
    static constexpr std::uint16_t START_ADDRESS = 0x200;
    mutable std::uint16_t END_ADDRESS = 0x200;

    DisplayManager display_manager{};
    
    // Font
    Font font{};
    static constexpr std::uint16_t FONT_ADDRESS = 0x50;
};


#endif