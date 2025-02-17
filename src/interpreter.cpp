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
    cycle();
    cycle();
    cycle();
    while (display_manager.is_running())
    {
        
        // Handle SDL Events
        display_manager.event_loop();

        // Cycle

        // Delay for 16ms
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

    debug_ip(ip);


    // Execute
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
    display_manager.draw_from_buffer_scaled(display);
}