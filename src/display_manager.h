#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

class DisplayManager
{
public:
    DisplayManager();
    // Destructor
    ~DisplayManager();

    void event_loop();
    bool is_running();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    bool running;
};

#endif