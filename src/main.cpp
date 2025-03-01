#include "interpreter.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    // Handle command-line
    std::string rom_path{"../roms/1-chip8-logo.ch8"};
    if (argc > 1)
        rom_path = argv[1];

    Interpreter chip{};

    chip.init();
    chip.load_font_into_memory();
    chip.load_rom_into_memory(rom_path);

    // Font Memory
    //chip.display_memory(0x50, 0xA0);

    // Program Memory
    // chip.display_memory(0x200);

    chip.run();

    return 0;
}