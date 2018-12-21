#include "../Objects.h"


Line::Line (gfx::Canvas* canvasToSet, Point p1, Point p2, int thicknessToSet, Color borderColorToSet)
: Line (canvasToSet, p1.x(), p1.y(), p2.x(), p2.y(), thicknessToSet, borderColorToSet) {}

Line::Line (gfx::Canvas* canvasToSet, int x1, int y1, int x2, int y2, int thicknessToSet, Color borderColorToSet) :
    Primitive (canvasToSet, thicknessToSet, borderColorToSet)
{
    float a = 0.f;
    if (x1 != x2)
        a = (float)(y2 - y1) / (x2 - x1);

    if (x1 != x2 && fabs(a) <= 1.0f) {
        if (x1 < x2) {
            _endPoint.x(x2 - x1);
            _endPoint.y(y2 - y1);
            _translate(x1, y1);
        } else {
            _endPoint.x(x1 - x2);
            _endPoint.y(y1 - y2);
            _translate(x2, y2);
        }
    } else {
        if (y1 < y2) {
            _endPoint.x(x2 - x1);
            _endPoint.y(y2 - y1);
            _translate(x1, y1);
        } else {
            _endPoint.x(x1 - x2);
            _endPoint.y(y1 - y2);
            _translate(x2, y2);
        }
    }
}

void Line::draw() {
    math::Matrix<double> tmpTransformationMatrix = _transformationMatrix;

    if (_parent) {
        tmpTransformationMatrix = _transformationMatrix * _parent->transformationMatrix();
    }

    float a = 0.f;
    if (_endPoint.x() != 0)
        a = (float)_endPoint.y() / _endPoint.x();

    if (_endPoint.x() != 0 && abs(a) <= 1.0f ) {
        for (int x = 0; x <= _endPoint.x(); ++x) {
            Point p(x, round(x * a));
            _canvas->px(p * tmpTransformationMatrix, _borderColor, motionBlur());
        }
    } else {
        a = (float)_endPoint.x() / _endPoint.y();
        for (int y = 0; y <= _endPoint.y(); ++y) {
            Point p(round(y * a), y);
            _canvas->px(p * tmpTransformationMatrix, _borderColor, motionBlur());
        }
    }

}

Point Line::endPoint() {
    math::Matrix<double> tmp = _endPoint * transformationMatrix();
    return Point(tmp.get(0, 0), tmp.get(1, 0));
}

