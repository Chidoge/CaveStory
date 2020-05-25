/* Game class
 * This class holds all information for the main game loop 
 */

#include <SDL2/SDL.h>
#include "../headers/game.h"
#include "../headers/graphics.h"
#include "../headers/input.h"

namespace {
    const int FPS = 50;
    const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    this->gameLoop();
}

void Game::gameLoop() {
    Graphics graphics;
    Input input;
    SDL_Event event;

    int lastUpdateTime = SDL_GetTicks();

    while (true) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                /* This condition makes sure that held key doesn't perpetually trigger this action */
                if (event.key.repeat == 0) {
                    input.keyDownEvent(event);
                }
            }
            else if (event.type == SDL_KEYUP) {
                input.keyUpEvent(event);
            }
            else if (event.type == SDL_QUIT) {
                return;
            }
        }

        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
            return;
        }

        const int currentTimeMS = SDL_GetTicks();
        int elapsedTimeMS = currentTimeMS - lastUpdateTime;
        lastUpdateTime = currentTimeMS;

        this->update(std::min(currentTimeMS, MAX_FRAME_TIME));
    }
}

void Game::draw(Graphics &graphics) {

}

void Game::update(float elapsedTime) {

}

Game::~Game() {

}