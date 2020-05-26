#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

/* AnimatedSprite class
 * Holds logic for animating sprites
 */

#include <map>
#include <string>
#include <vector>

#include "globals.h"
#include "sprite.h"

class Graphics;

class AnimatedSprite : public Sprite {
    public:
        AnimatedSprite();
        AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
            float posX, float posY, float timeToUpdate);
        
        /* void playAnimation
         * Plays animation provided it's not already playing
         */
        void playAnimation(std::string animation, bool once = false);

        /* void update
         * Updated the animated sprite timer
         */
        void update(int elapsedTime);

        /* void draw
         * Draws sprite to screen
         */
        void draw(Graphics &graphics, int x, int y);

        /* void setupAnimations
         * A required function that sets up all animations for a sprite
         */
        virtual void setupAnimations();

    private:
        std::map<std::string, std::vector<SDL_Rect>> _animations;
        std::map <std::string, Vector2> _offsets;

        int _frameIndex;
        double _timeElapsed;
        bool _visible;

    protected:
        double _timeToUpdate;
        bool _currentAnimationOnce;
        std::string _currentAnimation;

        /* void addAnimation
         * Adds an animation to the map of animations for the sprite
         */
        void addAnimation(int frames, int x, int y, std::string animationName, int width, int height, Vector2 offset);

        /* void resetAnimations
         * Resets all animations associated with the sprite
         */
        void resetAnimations();

        /* void stopAnimation
         * Stops the current animation
         */
        void stopAnimation();

        /* void setVisible
         * Changes visibility of the animated sprite
         */
        void setVisible(bool visible);

        /* void animationDone
         * Logic that happens when an animation ends
         */
        virtual void animationDone(std::string currentAnimation);
};

#endif