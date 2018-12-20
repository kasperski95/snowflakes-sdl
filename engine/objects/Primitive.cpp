#include "../Objects.h"

Primitive::Primitive(gfx::Canvas* canvasToSet, int thicknessToSet, Color borderColorToSet) {
    _canvas = canvasToSet;
    _thickness = thicknessToSet;
    _borderColor = borderColorToSet;
}
