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
    void execute(const  instruction_parameters& ip);

    void step_program_counter();
    void step_back_program_counter();
    void update_clocks();
    void set_cycles_per_frame(int cycles);
    void load_font_into_memory();
    void load_rom_into_memory(const std::string& path);
    void display_memory(uint16_t start_addr);
    void display_memory(uint16_t start_addr, uint16_t end_addr);
    void debug_ip(const instruction_parameters& ip);

    /* Routines */
    void routine_00E0();
    void routine_00EE();
    void routine_1nnn(const instruction_parameters& ip);
    void routine_2nnn(const instruction_parameters& ip);
    void routine_3xnn(const instruction_parameters& ip);
    void routine_4xnn(const instruction_parameters& ip);
    void routine_5xy0(const instruction_parameters& ip);
    void routine_6xnn(const instruction_parameters& ip);
    void routine_7xnn(const instruction_parameters& ip);
    void routine_8xy0(const instruction_parameters& ip);
    void routine_8xy1(const instruction_parameters& ip);
    void routine_8xy2(const instruction_parameters& ip);
    void routine_8xy3(const instruction_parameters& ip);
    void routine_8xy4(const instruction_parameters& ip);
    void routine_8xy5(const instruction_parameters& ip);
    void routine_8xy6(const instruction_parameters& ip);
    void routine_8xy7(const instruction_parameters& ip);
    void routine_8xyE(const instruction_parameters& ip);
    void routine_9xy0(const instruction_parameters& ip);
    void routine_Annn(const instruction_parameters& ip);
    void routine_Bnnn(const instruction_parameters& ip);
    void routine_Cxnn(const instruction_parameters& ip);
    void routine_Dxyn(const instruction_parameters& ip);
    void routine_Ex9E(const instruction_parameters& ip);
    void routine_ExA1(const instruction_parameters& ip);
    void routine_Fx07(const instruction_parameters& ip);
    void routine_Fx0A(const instruction_parameters& ip);
    void routine_Fx15(const instruction_parameters& ip);
    void routine_Fx18(const instruction_parameters& ip);
    void routine_Fx1E(const instruction_parameters& ip);
    void routine_Fx29(const instruction_parameters& ip);
    void routine_Fx33(const instruction_parameters& ip);
    void routine_Fx55(const instruction_parameters& ip);
    void routine_Fx65(const instruction_parameters& ip);

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
    int cycles_per_frame = 8;

    // Constants
    static constexpr std::uint16_t START_ADDRESS = 0x200;
    mutable std::uint16_t END_ADDRESS = 0x200;

    // Drawing
    DisplayManager display_manager{};
    bool draw_flag = true; // Draw once
    
    // Font
    Font font{};
    static constexpr std::uint16_t FONT_ADDRESS = 0x50;
};


#endif