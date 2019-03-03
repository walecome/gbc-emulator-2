#include <iostream>
#include "Renderer.hh"
#include "SDL2/SDL.h"

/*  Handle the the passed in event.
    Return true if the program should halt */
bool handle_event(SDL_Event &event) {
    switch (event.type) {
        case SDL_QUIT:
            std::cout << "SDL_QUIT" << std::endl;
            return true;
            break;
    }

    return false;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    Renderer ren{640, 480};

    ren.createWindow();
    ren.createRenderer();

    SDL_SetRenderDrawColor(ren.getRenderer(), 0, 0, 0, 255);
    SDL_RenderClear(ren.getRenderer());

    SDL_RenderPresent(ren.getRenderer());

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            quit = handle_event(event);
        }
    }

    // Clean up
    SDL_Quit();

    std::cout << "Exiting..." << std::endl;
    return 0;
}