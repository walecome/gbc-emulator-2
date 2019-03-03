#pragma once
#include <iostream>
#include "SDL2/SDL.h"

#ifdef DEBUG_BUILD
#define DEBUG(x) cerr << x
#else
#define DEBUG(x) \
    do {         \
    } while (0)
#endif

using uint = unsigned int;

class Renderer {
   public:
    // -Weffc++
    Renderer(uint width, uint height) : _width{width}, _height{height} {}
    Renderer() = delete;
    Renderer(const Renderer&) = delete;
    Renderer operator=(const Renderer&) = delete;
    ~Renderer();

    bool createWindow();
    bool createRenderer();

    int getWindowHeight() { return _height; }
    int getWindowWidth() { return _width; }
    SDL_Window* getWindow() { return _win; }
    SDL_Renderer* getRenderer() { return _ren; }

   private:
    SDL_Window* _win{nullptr};
    SDL_Renderer* _ren{nullptr};

    uint _width{};
    uint _height{};
};