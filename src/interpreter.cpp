#include "interpreter.h"
#include <iostream>
#include <fstream>

void Interpreter::run()
{
    while (display_manager.is_running())
    {
        // Handle SDL Events
        display_manager.event_loop();

        // Cycle
    }
}

void Interpreter::cycle()
{
    // Fetch

    // Decode

    // Execute
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
