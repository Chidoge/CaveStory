#include <iostream>

#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/sprite.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
                float posX, float posY) :
                    _x(posX),
                    _y(posY)
{
    this->_sourceRect.x = sourceX;
    this->_sourceRect.y = sourceY;
    this->_sourceRect.w = width;
    this->_sourceRect.h = height;

    this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));

    if (this->_spriteSheet == NULL) {
        std::cout << "\nError: Unable to load asset (" << filePath << "):\n" << std::endl;
    }
}

void Sprite::draw(Graphics &graphics, int x, int y) {
    SDL_Rect destinationRectangle = { 
        x, y, 
        int(this->_sourceRect.w * globals::SRPITE_SCALE), 
        int(this->_sourceRect.h * globals::SRPITE_SCALE) 
    };
    graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {}

Sprite::~Sprite() {}
