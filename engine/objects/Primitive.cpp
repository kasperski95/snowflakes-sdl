#include "../Objects.h"

Primitive::Primitive(gfx::Canvas* canvasToSet, int thicknessToSet, Color borderColorToSet) {
    _canvas = canvasToSet;
    _thickness = thicknessToSet;
    _borderColor = borderColorToSet;
    _momentum = Point();
}


void Primitive::move(float dt, Point p) { move(p.x(), p.y()); }
void Primitive::move(float dt, float x, float y) {
    //std::cout << dt << std::endl;
    //location().print();
    location(location().x() + (x + _momentum.x()) * dt, location().y() + (y + _momentum.y()) * dt);
    _translate(location().x(), location().y());
}

// GETTERS & SETTESRS
Point Primitive::momentum() {return _momentum;}
void Primitive::momentum(float x, float y) {
    _momentum.x(x);
    _momentum.y(y);
}
