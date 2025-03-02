#include "display_manager.h"
#include <iostream>

#include <memory>
#include <cstring>

DisplayManager::DisplayManager() : window(nullptr), renderer(nullptr)
{
    // Critical Error: Video failed to initialize
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "SDL_INIT_VIDEO/SDL_INIT_AUDIO failed: " << SDL_GetError() << '\n';
        throw std::runtime_error("SDL_INIT_VIDEO/SDL_INIT_AUDIO Failed");
    }

    // Window
    window = SDL_CreateWindow("Chip-8", 
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED, 
                            SCREEN_WIDTH, 
                            SCREEN_HEIGHT, 
                            SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window Creation Failed: " << SDL_GetError() << '\n';
        throw std::runtime_error("Window Creation Failed");
    }

    // Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer Creation Failed: " << SDL_GetError() << '\n';
        throw std::runtime_error("Renderer Creation Failed");
    }

    // Texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, CHIP_WIDTH, CHIP_HEIGHT);
    if (!texture)
    {
        std::cerr << "Texture Creation Failed: " << SDL_GetError() << '\n';
        throw std::runtime_error("Texture Creation Failed");
    }

    // Set running
    running = true;
}

// Destructor
DisplayManager::~DisplayManager()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void DisplayManager::event_loop(std::array<std::uint8_t, 16>& chip_keypad)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                int key = event.key.keysym.sym;
                if (auto it = key_map.find(key); it != key_map.end())
                    chip_keypad[it->second] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                break;
        }
    }
}

void DisplayManager::delay(std::uint32_t ms)
{
    SDL_Delay(ms);
}

void DisplayManager::draw_from_buffer_scaled(const std::array<std::uint32_t, 64 * 32>& buffer)
{
    SDL_UpdateTexture(texture, nullptr, static_cast<const void*>(buffer.data()), pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool DisplayManager::is_running()
{
    return running;
}