#include "../core/Structures.h"
#include "../Objects.h"


Edge::Edge(Point p1, Point p2) {
    _p1 = p1;
    _p2 = p2;
    _calculateSlope();
}

// GETTERS & SETTERS
Point Edge::p1() { return _p1; }
Point Edge::p2() { return _p2; }
double Edge::a() { return _a; }
double Edge::b() { return _b; }
bool Edge::bVertical() { return _bVertical; }

// PRIVATE
void Edge::_calculateSlope() {
    if (_p2.x() == _p1.x()) {
        _a = 0;
        _b = 0;
        _bVertical = true;
        return;
    }

    _a = (double)(_p2.y() - _p1.y()) / (_p2.x() - _p1.x());
    _b = _p1.y() - _a*_p1.x();
    _bVertical = false;
}
