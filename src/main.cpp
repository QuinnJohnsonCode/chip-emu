#include "interpreter.h"
#include <iostream>

int main()
{
    Interpreter chip{};
    chip.load_rom_into_memory("../roms/IBM.ch8");
    chip.display_memory(0x200);

    chip.run();

    return 0;
}