#include "../headers/level.h"
#include "../headers/graphics.h"
#include <SDL2/SDL.h>

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) : 
    _mapName(mapName),
    _spawnPoint(spawnPoint),
    _size(Vector2(0, 0))    
{
    this->loadMap(mapName, graphics);
}

void Level::loadMap(std::string mapName, Graphics &graphics) {
    this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("assets/backgrounds/bkBlue.png"));
    this->_size = Vector2(1280, 960);
}

void Level::update(int elapsedTime) {}

void Level::draw(Graphics &graphics) {
    int LENGTH = 64;
    SDL_Rect sourceRect = { 0, 0, LENGTH, LENGTH };
    SDL_Rect destRect;

    for (int x = 0; x < this->_size.x/LENGTH; x++) {
        for (int y = 0; y < this->_size.y/LENGTH; y++) {
            destRect.x = x * LENGTH * globals::SRPITE_SCALE;
            destRect.y = y * LENGTH * globals::SRPITE_SCALE;
            destRect.w = LENGTH * globals::SRPITE_SCALE;
            destRect.h = LENGTH * globals::SRPITE_SCALE;
            
            graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
        }
    }
}
 
Level::~Level() {}