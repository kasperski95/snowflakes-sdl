#pragma once

#include "../Math.h"

#ifdef __APPLE__
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif


class Point : public math::Matrix {
public:
    Point(Matrix matrix);
    Point(int x=0, int y=0);

    void swap();

    // GETTERS & SETTERS
    int x() const;
    int y() const;
    void x(int xToSet);
    void y(int yToSet);

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
