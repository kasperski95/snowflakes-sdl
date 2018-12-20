#include "../Structures.h"


Color::Color (Uint16 rToSet, Uint16 gToSet, Uint16 bToSet) {
    r = rToSet;
    g = gToSet;
    b = bToSet;
}

Color& Color::operator+=(const Color &rhs) {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
}

Color& Color::operator/=(const int &rhs) {
    r /= rhs;
    g /= rhs;
    b /= rhs;
    return *this;
}
