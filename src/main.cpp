#include "interpreter.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    // Handle command-line
    std::string rom_path{"../roms/1-chip8-logo.ch8"};
    if (argc < 3)
    {
        std::cerr << "Usage: ./main.out [cycles_per_frame] [rom_path]" << '\n';
        return 1;
    }
    
    int cycles = std::stoi(argv[1]);
    rom_path = argv[2];
    
    // Chip Initialization
    Interpreter chip{};
    chip.init();
    chip.set_cycles_per_frame(cycles);
    chip.load_font_into_memory();
    chip.load_rom_into_memory(rom_path);

    // Chip Run Loop
    chip.run();

    return 0;
}