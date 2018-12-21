#include "../Objects.h"

Circle::Circle(gfx::Canvas* canvasToSet, int radiusToSet, int thicknessToSet, Color borderColorToSet)
    : Primitive (canvasToSet, thicknessToSet, borderColorToSet)
{
    _radius = radiusToSet;
}

void Circle::draw() {
    for (int rr = _radius; rr > (_radius - _thickness); --rr) {
        // initial points
        double x = this->location().x();
        double y = this->location().y();
        double xx = 0;
        double yy = rr;
        _canvas->px(x + xx, y + yy, _borderColor, this->motionBlur());
        _canvas->px(x - xx, y - yy, _borderColor, this->motionBlur());
        _canvas->px(x + yy, y + xx, _borderColor, this->motionBlur());
        _canvas->px(x - yy, y - xx, _borderColor, this->motionBlur());

        // intitial condition
        int d = 3 - 2 * rr;

        while (xx < yy) {
            if (d < 0) {
                d = d + 4 * xx + 6;
                xx++;
            } else {
                d = d + 4 * (xx - yy) + 10;
                xx++;
                yy--;
            }

            // symetry
            _canvas->px(x + xx, y + yy, _borderColor, this->motionBlur());
            _canvas->px(x + xx, y - yy, _borderColor, this->motionBlur());
            _canvas->px(x - xx, y + yy, _borderColor, this->motionBlur());
            _canvas->px(x - xx, y - yy, _borderColor, this->motionBlur());
            _canvas->px(x + yy, y + xx, _borderColor, this->motionBlur());
            _canvas->px(x + yy, y - xx, _borderColor, this->motionBlur());
            _canvas->px(x - yy, y + xx, _borderColor, this->motionBlur());
            _canvas->px(x - yy, y - xx, _borderColor, this->motionBlur());
        }
    }
}

void Circle::fill(Color color) {
    // TODO: not working with motionblur
    Color col, col2;
    int x = (int)round(this->location().x());
    int y = (int)round(this->location().y());
    int xx, yy;

    for (int dirX=-1, dirY=-1; dirY <= 1; dirY+=2, dirX=1) {
        xx = x;
        yy = y;
        if (xx < 0 || xx >= _canvas->width())
            break;

        while(yy >= 0 && yy < _canvas->height()) {
            // go left or right, until edge
            do {
                _canvas->px(xx, yy, color);

                if (xx + dirX < 0 || xx + dirX >= _canvas->width())
                    break;
                xx += dirX;
                col = _canvas->px(xx, yy);
            } while (col.r != _borderColor.r || col.g !=  _borderColor.g || col.b !=  _borderColor.b);

            // go back, until going up or down is possible
            do {
                if (xx - dirX < 0 || xx - dirX >= _canvas->width())
                    break;

                xx -= dirX;
                col = _canvas->px(xx, yy);

                // if it's not possible, stop going up or down
                if (yy + dirY >= _canvas->height() || yy + dirY < 0 || col.r == _borderColor.r && col.g == _borderColor.g && col.b == _borderColor.b)
                    goto loopEnd;

                col2 = _canvas->px(xx, yy + dirY);
            } while (col2.r == _borderColor.r && col2.g == _borderColor.g && col2.b == _borderColor.b);

            dirX = -dirX;
            yy += dirY;
        }
        loopEnd: continue;
    }
}
