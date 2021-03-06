#include "../Core.h"
#include <cmath>

using namespace gfx;

Canvas::Canvas(SDL_Surface* screenToSet, int widthToSet, int heightToSet) {
    _screen = screenToSet;
    _width = widthToSet;
    _height = heightToSet;
    _factor = 1.f;
}


void Canvas::update() {
    SDL_Flip(_screen);
    SDL_FillRect(_screen, 0, SDL_MapRGB(_screen->format, 0, 0, 0));
}


// GETTERS & SETTERS
int Canvas::width() { return _width; }
int Canvas::height() { return _height; }

Color Canvas::px(float x, float y) {
    return Color(getPixel((int)round(x), (int)round(y), _width, _height, _screen).r, getPixel(x, y, _width, _height, _screen).g, getPixel(x, y, _width, _height, _screen).b);
}


void Canvas::px(math::Matrix<float> m, Color color, bool motionBlur) {
    this->px(m.get(0,0), m.get(1,0), color.r, color.g, color.b, motionBlur);
}

void Canvas::px(Point p, Color color) {
    this->px(p.x(), p.y(), color.r, color.g, color.b);
}

void Canvas::px(float x, float y, Color color, bool motionBlur, Uint8 alpha) {
    this->px(x, y, color.r, color.g, color.b, motionBlur, alpha);
}

void Canvas::px(float x, float y, Uint8 r, Uint8 g, Uint8 b, bool motionBlur, Uint8 alpha) {
    x = math::clamp((int)round(x), 0, _width);
    y = math::clamp((int)round(y), 0, _height);

    if (x > 0 && x < _width && y >0 && y < _height) {
        alpha = math::clamp(alpha, 0, 255);
        float mix = alpha / 255.f;
        r = r * _factor * mix;
        g = g * _factor * mix;
        b = b * _factor * mix;

        if (motionBlur) {
            Color pxCol = px(x, y);
            setPixel(x, y, math::clamp(pxCol.r + r, 0, 255), math::clamp(pxCol.g + g, 0, 255), math::clamp(pxCol.b + b, 0, 255), _screen);
        } else {
            setPixel(x, y, math::clamp(r, 0, 255), math::clamp(g, 0, 255), math::clamp(b, 0, 255), _screen);
        }
    }
}

void Canvas::factor(float factorToSet) {
    _factor = factorToSet;
}


