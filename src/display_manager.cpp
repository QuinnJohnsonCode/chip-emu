#include "display_manager.h"
#include <iostream>

#include <memory>
#include <cstring>

DisplayManager::DisplayManager() : window(nullptr), renderer(nullptr)
{
    // Critical Error: Video failed to initialize
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_INIT_VIDEO failed: " << SDL_GetError() << '\n';
        throw std::runtime_error("SDL_INIT_VIDEO Failed");
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

    // https://stackoverflow.com/questions/55107529/how-to-create-a-1-bit-per-pixel-surface-with-sdl2
    // Surface (Store pixel information 64x32)
    surface = SDL_CreateRGBSurfaceWithFormat(0, 64, 32, 8, SDL_PIXELFORMAT_INDEX8);
    SDL_SetPaletteColors(surface->format->palette, colors, 0, 2);
    if (!surface)
    {
        std::cerr << "Surface Creation Failed: " << SDL_GetError() << '\n';
        throw std::runtime_error("Surface Creation Failed");
    }


    // Set running
    running = true;
}

// Destructor
DisplayManager::~DisplayManager()
{
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void DisplayManager::event_loop()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
    }
}

void DisplayManager::delay(std::uint32_t ms)
{
    SDL_Delay(ms);
}

void DisplayManager::draw_from_buffer_scaled(const std::array<std::uint32_t, 64 * 32>& buffer)
{
    // Mirror buffer to surface pixel buffer
    std::uint8_t *pixels = static_cast<std::uint8_t*>(surface->pixels);
    for (std::uint32_t x = 0; x < CHIP_WIDTH; ++x)
    {
        for (std::uint32_t y = 0; y < CHIP_HEIGHT; ++y)
        {
            int index = (CHIP_WIDTH * y) + x;
            pixels[index] = buffer[index];
        }
    }

    /*
    Note on BlitScaled():
        BlitSurface() was working on our 64x32 blit to the window's surface, but BlitScaled was not,
        despite taking in common arguments. Determined that BlitScaled() requires the pixel format of
        both the src surface and dest surface to match, thus, after writing to our INDEX8 surface, we must use
        ConvertSurfaceFormat() passing in the dest surface's format with our surface.
    */

    // Blit
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    SDL_Surface *converted_surface = SDL_ConvertSurfaceFormat(surface, window_surface->format->format, 0);
    
    SDL_RenderClear(renderer);
    SDL_BlitScaled(converted_surface, &chip_rect, window_surface, &screen_rect);
    SDL_UpdateWindowSurface(window);

    // Clean up surface copy
    SDL_FreeSurface(converted_surface);
}

bool DisplayManager::is_running()
{
    return running;
}