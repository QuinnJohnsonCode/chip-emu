#include "interpreter.h"
#include <iostream>

int main()
{
    Interpreter chip{};

    chip.init();
    chip.load_font_into_memory();
    // chip.load_rom_into_memory("../roms/1-chip8-logo.ch8");
    // chip.load_rom_into_memory("../roms/2-ibm-logo.ch8");
    // chip.load_rom_into_memory("../roms/3-corax.ch8");
    // chip.load_rom_into_memory("../roms/4-flags.ch8");
    chip.load_rom_into_memory("../roms/6-keypad.ch8");

    // Font Memory
    //chip.display_memory(0x50, 0xA0);

    // Program Memory
    // chip.display_memory(0x200);

    chip.run();

    return 0;
}