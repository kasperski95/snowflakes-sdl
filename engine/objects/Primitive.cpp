#include "../Objects.h"

Primitive::Primitive(gfx::Canvas* canvasToSet, int thicknessToSet, Color borderColorToSet) {
    _canvas = canvasToSet;
    _thickness = thicknessToSet;
    _borderColor = borderColorToSet;
    _mX = 0.f;
    _mY = 0.f;
}


void Primitive::move(Point p) { move(p.x(), p.y()); }
void Primitive::move(int x, int y) {
    _translate(x + location().x() + _mX, y + location().y() + _mY);
}

// GETTERS & SETTESRS
float Primitive::momentumX() {return _mX;}
float Primitive::momentumY() {return _mY;}
void Primitive::momentum(float x, float y) {
    _mX = x;
    _mY = y;
}
