#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <array>

class DisplayManager
{
public:
    DisplayManager();
    // Destructor
    ~DisplayManager();

    void event_loop();
    void delay(std::uint32_t ms);
    void draw_from_buffer_scaled(const std::array<std::uint32_t, 64 * 32>& buffer);
    bool is_running();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Event event;

    static constexpr SDL_Color colors[2] = {{0, 0, 0, 255}, {255, 255, 255, 255}};
    bool running;

    static constexpr int STRETCH_FACTOR = 20;
    static constexpr int CHIP_WIDTH = 64;
    static constexpr int CHIP_HEIGHT = 32;
    static constexpr int SCREEN_WIDTH = CHIP_WIDTH * STRETCH_FACTOR;
    static constexpr int SCREEN_HEIGHT = CHIP_HEIGHT * STRETCH_FACTOR;

    SDL_Rect chip_rect{0, 0, CHIP_WIDTH, CHIP_HEIGHT};
    SDL_Rect screen_rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

};

#endif