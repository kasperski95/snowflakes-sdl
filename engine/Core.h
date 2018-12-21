#pragma once

#ifdef __APPLE__
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif

#include <vector>
#include "Math.h"
#include "core/Structures.h"

namespace gfx {
    SDL_Color getPixel (int x, int y, int width, int height, SDL_Surface* screen);
    void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B, SDL_Surface* screen);
    void clearScreen(Uint8 R, Uint8 G, Uint8 B);
    void loadBMP(char const* name, int x, int y);


    class Canvas {
    public:
        Canvas(SDL_Surface* screen, int width, int height);
        void update();

        // GETTERS & SETTERS
        int width();
        int height();
        Color px(float x, float y);
        void px(math::Matrix<float> m, Color color, bool motionBlur=false);
        void px(Point p, Color color);
        void px(float x, float y, Color color, bool motionBlur=false);
        void px(float x, float y, Uint8 r, Uint8 g, Uint8 b, bool motionBlur=false);
        void factor(float factor);

    private:
        SDL_Surface* _screen;
        int _width;
        int _height;
        float _factor;
    };
}
