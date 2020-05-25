#include <iostream>
#include <SDL2/SDL.h>
#undef main

int main(int argc, const char* argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error \n";
    }

    std::cout << "Hello world \n";

    return EXIT_SUCCESS;
}