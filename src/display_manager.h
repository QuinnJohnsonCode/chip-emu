#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <array>
#include <unordered_map>

class DisplayManager
{
public:
    DisplayManager();
    // Destructor
    ~DisplayManager();

    void event_loop(std::array<std::uint8_t, 16>& chip_keypad);
    void delay(std::uint32_t ms);
    void draw_from_buffer_scaled(const std::array<std::uint32_t, 64 * 32>& buffer);
    bool is_running();

private:
    // SDL-Related
    SDL_Window* window;
    SDL_Renderer* renderer;
    // SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Event event;

    // Screen Constants
    static constexpr int STRETCH_FACTOR = 20;
    static constexpr int CHIP_WIDTH = 64;
    static constexpr int CHIP_HEIGHT = 32;
    static constexpr int SCREEN_WIDTH = CHIP_WIDTH * STRETCH_FACTOR;
    static constexpr int SCREEN_HEIGHT = CHIP_HEIGHT * STRETCH_FACTOR;
    static constexpr int pitch = 256;

    SDL_Rect chip_rect{0, 0, CHIP_WIDTH, CHIP_HEIGHT};
    SDL_Rect screen_rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Keyboard
    const std::unordered_map<int, std::uint8_t> key_map =
    {
        {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
        {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
        {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
        {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF},
    };

    // Misc
    static constexpr SDL_Color colors[2] = {{0, 0, 0, 255}, {255, 255, 255, 255}};
    bool running;
};

#endif