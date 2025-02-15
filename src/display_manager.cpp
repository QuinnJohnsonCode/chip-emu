#include "display_manager.h"
#include <iostream>

DisplayManager::DisplayManager() : window(nullptr), renderer(nullptr)
{
    // Critical Error: Video failed to initialize
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_INIT_VIDEO failed: " << SDL_GetError() << '\n';
        throw std::runtime_error("SDL_INIT_VIDEO Failed");
    }

    // Window
    window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, SDL_WINDOW_SHOWN);
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

    // Set running
    running = true;
}

// Destructor
DisplayManager::~DisplayManager()
{
    SDL_DestroyRenderer(renderer);
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

bool DisplayManager::is_running()
{
    return running;
}