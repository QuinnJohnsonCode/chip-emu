#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>

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

        // Decrement timers
        update_clocks();

        // Cycle
        // 11 instructions per frame = 660 instructions per second
        for (int i = 0; i < 11; ++i)
            cycle();

        // Refresh display (only draw when display buffer is modified)
        if (draw_flag)
        {
            display_manager.draw_from_buffer_scaled(display);
            draw_flag = false;
        }

        // Delay 16ms
        display_manager.delay(16);
    }
}

void Interpreter::cycle()
{
    /* Fetch and Decode */
    
    fetch(); // Stores instruction in curr_instruction
    // Extract instruction parameters from the current instruction
    struct instruction_parameters ip = {
        .X = static_cast<std::uint8_t>((curr_instruction >> 8) & 0x0F),
        .Y = static_cast<std::uint8_t>((curr_instruction >> 4) & 0x0F),
        .N = static_cast<std::uint8_t>(curr_instruction & 0x0F),
        .NN = static_cast<std::uint8_t>(curr_instruction & 0xFF),
        .NNN = static_cast<std::uint16_t>(curr_instruction & 0xFFF),
    };

    /* Execute */
    execute(ip);
}

void Interpreter::fetch()
{
    // Build 16-bit instruction from two 8 bit values
    curr_instruction = (memory[pc] << 8) | memory[pc + 1];

    // Increment Program Counter
    step_program_counter();
}

void Interpreter::execute(const instruction_parameters& ip)
{
    switch ((curr_instruction >> 12) & 0xF)
    {
        case 0x0:
            switch (curr_instruction & 0xF)
            {
                case 0x0:
                    routine_00E0();
                    break;
                case 0xE:
                    routine_00EE();
                    break;
                default:
                    std::cerr << "Unknown routine: " << std::format("{:04X}", curr_instruction) << '\n';
                    throw std::runtime_error("Unknown routine");
            }
            break;
        case 0x1:
            routine_1nnn(ip);
            break;
        case 0x2:
            routine_2nnn(ip);
            break;
        case 0x3:
            routine_3xnn(ip);
            break;
        case 0x4:
            routine_4xnn(ip);
            break;
        case 0x5:
            routine_5xy0(ip);
            break;
        case 0x6:
            routine_6xnn(ip);
            break;
        case 0x7:
            routine_7xnn(ip);
            break;
        case 0x8:
            switch (curr_instruction & 0xF)
            {
                case 0x0:
                    routine_8xy0(ip);
                    break;
                case 0x1:
                    routine_8xy1(ip);
                    break;
                case 0x2:
                    routine_8xy2(ip);
                    break;
                case 0x3:
                    routine_8xy3(ip);
                    break;
                case 0x4:
                    routine_8xy4(ip);
                    break;
                case 0x5:
                    routine_8xy5(ip);
                    break;
                case 0x6:
                    routine_8xy6(ip);
                    break;
                case 0x7:
                    routine_8xy7(ip);
                    break;
                case 0xE:
                    routine_8xyE(ip);
                    break;
                default:
                    std::cerr << "Unknown routine: " << std::format("{:04X}", curr_instruction) << '\n';
                    throw std::runtime_error("Unknown routine");
            }
            break;
        case 0x9:
            routine_9xy0(ip);
            break;
        case 0xA:
            routine_Annn(ip);
            break;
        case 0xB:
            routine_Bnnn(ip);
            break;
        case 0xD:
            routine_Dxyn(ip);
            break;
        case 0xF:
            switch (curr_instruction & 0xFF)
            {
                case 0x1E:
                    routine_Fx1E(ip);
                    break;
                case 0x33:
                    routine_Fx33(ip);
                    break;
                case 0x55:
                    routine_Fx55(ip);
                    break;
                case 0x65:
                    routine_Fx65(ip);
                    break;
                default:
                    std::cerr << "Unknown routine: " << std::format("{:04X}", curr_instruction) << '\n';
                    throw std::runtime_error("Unknown routine");
            }
            break;
        default:
            std::cerr << "Unknown routine: " << std::format("{:04X}", curr_instruction) << '\n';
            throw std::runtime_error("Unknown routine");
    }
}

void Interpreter::step_program_counter()
{
    pc += 2;
}

void Interpreter::update_clocks()
{
    delay_timer = (delay_timer > 0) ? delay_timer - 1 : delay_timer;
    sound_timer = (sound_timer > 0) ? sound_timer - 1 : sound_timer;
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

void Interpreter::debug_ip(const instruction_parameters& ip)
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
    std::fill(display.begin(), display.end(), 0);
    draw_flag = true;
}

void Interpreter::routine_00EE()
{
    // Set PC = stack top and pop stack
    pc = stack.top();
    stack.pop();
}

void Interpreter::routine_1nnn(const instruction_parameters& ip)
{
    // Set PC = nnn
    pc = ip.NNN;
}

void Interpreter::routine_2nnn(const instruction_parameters& ip)
{
    // Push PC to stack, PC = nnn
    stack.push(pc);
    pc = ip.NNN;
}

void Interpreter::routine_3xnn(const instruction_parameters& ip)
{
    // Skip instruction if Vx = nn
    if (registers[ip.X] == ip.NN)
        step_program_counter();
}

void Interpreter::routine_4xnn(const instruction_parameters& ip)
{
    // Skip instruction if Vx != nn
    if (registers[ip.X] != ip.NN)
        step_program_counter();
}

void Interpreter::routine_5xy0(const instruction_parameters& ip)
{
    // Skip instruction if Vx = Vy
    if (registers[ip.X] == registers[ip.Y])
        step_program_counter();
}

void Interpreter::routine_6xnn(const instruction_parameters& ip)
{
    // Set Vx = nn
    registers[ip.X] = ip.NN;
}

void Interpreter::routine_7xnn(const instruction_parameters& ip)
{
    // Set Vx = Vx + kk
    registers[ip.X] = registers[ip.X] + ip.NN;
}

void Interpreter::routine_8xy0(const instruction_parameters& ip)
{
    // Set Vx = Vy
    registers[ip.X] = registers[ip.Y];
}

void Interpreter::routine_8xy1(const instruction_parameters& ip)
{
    // Set Vx = Vx or Vy
    registers[ip.X] |= registers[ip.Y];
}

void Interpreter::routine_8xy2(const instruction_parameters& ip)
{
    // Set Vx = Vx and Vy
    registers[ip.X] &= registers[ip.Y];
}

void Interpreter::routine_8xy3(const instruction_parameters& ip)
{
    // Set Vx = Vx xor Vy
    registers[ip.X] ^= registers[ip.Y];
}

void Interpreter::routine_8xy4(const instruction_parameters& ip)
{
    // Set Vx = Vx + Vy
    // Vf = 1 if overflow, otherwise 0
    std::uint16_t sum = registers[ip.X] + registers[ip.Y];
    registers[ip.X] = sum & 0xFF;
    registers[0xF] = (sum > 0xFF) ? 1 : 0;
}

void Interpreter::routine_8xy5(const instruction_parameters& ip)
{
    // Set Vx = Vx - Vy (Vf = 0 if underflow, otherwise 0)
    std::uint8_t reg_x = registers[ip.X];
    std::uint8_t reg_y = registers[ip.Y];
    
    registers[ip.X] = reg_x - reg_y;
    registers[0xF] = (reg_x < reg_y) ? 0 : 1;
}

void Interpreter::routine_8xy6(const instruction_parameters& ip)
{
    // Shift Right Vx
    std::uint8_t bit_out = registers[ip.X] & 0b1;
    registers[ip.X] >>= 0b1;
    registers[0xF] = bit_out;
    
}

void Interpreter::routine_8xy7(const instruction_parameters& ip)
{
    // Set Vx = Vy - Vx
    // Vf set to 0 if underflow, 1 if not
    std::uint8_t reg_x = registers[ip.X];
    std::uint8_t reg_y = registers[ip.Y];

    registers[ip.X] = reg_y - reg_x;
    registers[0xF] = (reg_y < reg_x) ? 0 : 1;
}

void Interpreter::routine_8xyE(const instruction_parameters& ip)
{
    // Shift Left Vx
    std::uint8_t bit_out = (registers[ip.X] >> 0x7);
    registers[ip.X] <<= 0b1;
    registers[0xF] = bit_out;
    
}

void Interpreter::routine_9xy0(const instruction_parameters& ip)
{
    // Skip instruction if Vx != Vy
    if (registers[ip.X] != registers[ip.Y])
        step_program_counter();
}

void Interpreter::routine_Annn(const instruction_parameters& ip)
{
    // Set I = nnn
    index = ip.NNN;
}

void Interpreter::routine_Bnnn(const instruction_parameters& ip)
{
    // Jump to address NNN + V0
    pc = ip.NNN + registers[0];
}

void Interpreter::routine_Dxyn(const instruction_parameters& ip)
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

    draw_flag = true;
}

void Interpreter::routine_Fx1E(const instruction_parameters& ip)
{
    // Set I = I + Vx;
    index += registers[ip.X];
}

void Interpreter::routine_Fx33(const instruction_parameters& ip)
{
    // Store BCD of Vx to memory at I, I + 1, and I + 2
    // Zero out memory locations to initialize
    memory[index] = 0, memory[index + 1] = 0, memory[index + 2] = 0;
    int dec = static_cast<int>(registers[ip.X]);

    // Store digits
    int count = 2;
    while (dec > 0)
    {
        // Place in correct position
        memory[index + count] = dec % 10;
        count--;
        dec /= 10;
    }
}

void Interpreter::routine_Fx55(const instruction_parameters& ip)
{
    // Save registers V0 - Vx to memory starting at I
    for (std::uint8_t i = 0; i <= ip.X; ++i)
        memory[index + i] = registers[i];
}

void Interpreter::routine_Fx65(const instruction_parameters& ip)
{
    // Load registers V0 - Vx from memory starting at I
    for (std::uint8_t i = 0; i <= ip.X; ++i)
        registers[i] = memory[index + i];
}