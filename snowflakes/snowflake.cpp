#include "snowflakes.h"

void Snowflake::draw() {
    _canvas->px(50, 50, Color(0,0,255), motionBlur());
}
