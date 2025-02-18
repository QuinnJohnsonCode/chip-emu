#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <algorithm>

void Interpreter::init()
{
    // Set Program Counter to START_ADDRESS
    pc = START_ADDRESS;
}

void Interpreter::run()
{
    while (display_manager.is_running())
    {
        
        // Handle SDL Events
        display_manager.event_loop();

        // Decrement timers (TODO)

        // Cycle
        // 11 instructions per frame = 660 instructions per second
        for (int i = 0; i < 11; ++i)
            cycle();

        // Refresh display
        display_manager.draw_from_buffer_scaled(display);

        // Delay 16ms
        display_manager.delay(16);
    }
}

void Interpreter::cycle()
{
    /* Fetch */
    fetch(); // Stores instruction in curr_instruction

    /* Decode */

    // Extract instruction parameters from the current instruction
    struct instruction_parameters ip = {
        .X = static_cast<std::uint8_t>((curr_instruction >> 8) & 0x0F),
        .Y = static_cast<std::uint8_t>((curr_instruction >> 4) & 0x0F),
        .N = static_cast<std::uint8_t>(curr_instruction & 0x0F),
        .NN = static_cast<std::uint8_t>(curr_instruction & 0xFF),
        .NNN = static_cast<std::uint16_t>(curr_instruction & 0xFFF),
    };

    // Execute
    switch ((curr_instruction >> 12) & 0xF)
    {
        case 0x0:
            routine_00E0();
            break;
        case 0x6:
            routine_6xnn(ip);
            break;
        case 0xA:
            routine_Annn(ip);
            break;
        case 0xD:
            routine_Dxyn(ip);
            break;
        case 0x1:
            routine_1nnn(ip);
            break;
        case 0x7:
            routine_7xnn(ip);
            break;
        default:
            std::cerr << "Unknown routine: " << std::format("{:04X}", curr_instruction) << '\n';
            throw std::runtime_error("Unknown routine");
    }

}

void Interpreter::fetch()
{
    // Build 16-bit instruction from two 8 bit values
    curr_instruction = (memory[pc] << 8) | memory[pc + 1];

    // Increment Program Counter
    step_program_counter();
}

void Interpreter::step_program_counter()
{
    pc += 2;
}

void Interpreter::load_font_into_memory()
{
    for (std::uint8_t i = 0; i < font.sprites.size(); ++i)
        memory[FONT_ADDRESS + i] = font.sprites[i];
}

void Interpreter::load_rom_into_memory(const std::string& path)
{
    std::ifstream rom{path, rom.binary | rom.in};

    if (!rom.is_open())
        std::cout << "Failed to open " << path << '\n';
    else
    {
        std::uint8_t byte;
        int i = 0;
        while (rom.read(reinterpret_cast<char*>(&byte), sizeof byte))
            memory[START_ADDRESS + i++] = byte;

        END_ADDRESS = START_ADDRESS + i;
    }
}

void Interpreter::display_memory(uint16_t start_addr)
{
    display_memory(start_addr, END_ADDRESS);
}

void Interpreter::display_memory(uint16_t start_addr, uint16_t end_addr)
{
    const char SEPARATOR = ' ';
    const char WIDTH = 6;
    for (int i = start_addr; i < end_addr; ++i)
    {            
        if ((i - start_addr) % WIDTH == 0 && i != start_addr)
            std::cout << '\n';
        std::cout << std::format("{:02X}", memory[i]) << SEPARATOR;
    }
    std::cout << "\n";
}

void Interpreter::debug_ip(struct instruction_parameters& ip)
{
    std::cout << std::format("{:04X}", curr_instruction) << '\n';
    std::cout << "X: " << std::format("{:02X}", ip.X) << '\n';
    std::cout << "Y: " << std::format("{:02X}", ip.Y) << '\n';
    std::cout << "N: " << std::format("{:02X}", ip.N) << '\n';
    std::cout << "NN: " << std::format("{:02X}", ip.NN) << '\n';
    std::cout << "NNN: " << std::format("{:04X}", ip.NNN) << '\n';
}

/* Routines */
void Interpreter::routine_00E0()
{
    // Clear Screen and Display
    std::fill(display.begin(), display.end(), 0);
}

void Interpreter::routine_6xnn(struct instruction_parameters& ip)
{
    // Set Vx = nn
    registers[ip.X] = ip.NN;
}

void Interpreter::routine_Annn(struct instruction_parameters& ip)
{
    // Set I = nnn
    index = ip.NNN;
}

void Interpreter::routine_Dxyn(struct instruction_parameters& ip)
{
    // Draw

    // Set X/Y to Vx % 64 and Vy % 32 respectively
    auto x = registers[ip.X] & 63;
    auto y = registers[ip.Y] & 31;

    // Set Vf to 0
    registers[0xF] = 0;

    for (int row = 0; row < ip.N; ++row)
    {
        // Get Nth byte of sprite data using I
        auto sprite_byte = memory[index + row];
        for (int col = 0; col < 8; ++col)
        {
            std::uint8_t bit = (sprite_byte >> (7 - col)) & 1; // Take first bit
            std::uint32_t position = (64 * (y + row)) + x + col; // Get (x,y) position offsetted by row/col

            if (bit == 0x01)
            {
                if (display[position] == 1)
                    registers[0xF] = 1;
                
                // XOR the display pixel
                display[position] ^= 1;
            }
        }
    }
}

void Interpreter::routine_1nnn(struct instruction_parameters& ip)
{
    // Set PC = nnn
    pc = ip.NNN;
}

void Interpreter::routine_7xnn(struct instruction_parameters& ip)
{
    // Set Vx = Vx + kk
    registers[ip.X] = registers[ip.X] + ip.NN;
}