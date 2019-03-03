#include "Renderer.hh"
#include <iostream>

Renderer::~Renderer() {
    if (_ren != nullptr) {
        // DEBUG("Freeing rendering component");
        std::cout << "Freeing rendering component" << std::endl;
        SDL_DestroyRenderer(_ren);
    }

    if (_win != nullptr) {
        // DEBUG("Freeing window component");
        std::cout << "Freeing window component" << std::endl;
        SDL_DestroyWindow(_win);
    }
}

bool Renderer::createWindow() {
    _win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, _width, _height,
                            SDL_WINDOW_SHOWN);

    if (_win == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

bool Renderer::createRenderer() {
    _ren = SDL_CreateRenderer(
        _win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_ren == nullptr) {
        SDL_DestroyWindow(_win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}