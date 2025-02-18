#include "interpreter.h"
#include <iostream>

int main()
{
    Interpreter chip{};

    chip.init();
    chip.load_font_into_memory();
    // chip.load_rom_into_memory("../roms/1-chip8-logo.ch8");
    chip.load_rom_into_memory("../roms/IBM.ch8");

    // Font Memory
    //chip.display_memory(0x50, 0xA0);

    // Program Memory
    chip.display_memory(0x200);

    chip.run();

    return 0;
}