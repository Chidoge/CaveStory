#ifndef GRAPHICS_H
#define GPAPHICS_H

/* Graphics class
 * Holds all information dealing with graphics for the game
 */

#include <map>
#include <string>

/* Forward declaration to avoid multiple instances of #include */
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;

class Graphics {
    public:
        Graphics();
        ~Graphics();

        /* SDL_Surface* loadImage
         * Loads an image into the _spriteSheets map if it doesn't already exist.
         * Returns image based on file path
         */
        SDL_Surface* loadImage(const std::string &filePath);

        /* void blitSurface
         * Draws a texture to a certain part of the screen
         * sourceRectangle - which part of the spritesheet to copy
         * destinationRectangle - which part of screen to copy onto
         */
        void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

        /* void flip
         * Renders everything to the screen
         */
        void flip();

        /* void clear
         * Clears the screen
         */
        void clear();

        SDL_Renderer* getRenderer() const;
    
    private:
        SDL_Window* _window;
        SDL_Renderer* _renderer;

        std::map <std::string, SDL_Surface*> _spriteSheets;
};

#endif