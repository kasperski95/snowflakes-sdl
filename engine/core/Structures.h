#pragma once

#include "../Math.h"

#ifdef __APPLE__
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif


class Point : public math::Matrix<float> {
public:
    Point(Matrix matrix);
    Point(float x=0, float y=0);

    void swap();

    // GETTERS & SETTERS
    float x() const;
    float y() const;
    void x(float xToSet);
    void y(float yToSet);

    // OPERATORS
    Point& operator-=(const Point &rhs);
};


struct Color {
    Uint16 r;
    Uint16 g;
    Uint16 b;
    Color (Uint16 rToSet=0, Uint16 gToSet=0, Uint16 bToSet=0);
    Color& operator+=(const Color &rhs);
    Color& operator/=(const int &rhs);
};
