#include "Snowflakes.h"
#include <cstdlib>
#include <ctime>

SnowflakeGenerator::SnowflakeGenerator(gfx::Canvas* canvasToSet, int nSnowflakes) :
    Primitive (canvasToSet, 0, Color(255,255,255)),
    _snowflakes(nSnowflakes)
{
    srand(time(0));

    {
        int x, y;
        for (int i = 0; i < nSnowflakes; ++i) {
            x = rand() % (int)(canvasToSet->width() * 1.5);
            y = (rand() % ((int)canvasToSet->height())) - canvasToSet->height();
            _snowflakes[i] = new Snowflake(canvasToSet, 1.f);
            _snowflakes[i]->location(x, y);
        }
    }
}

void SnowflakeGenerator::draw() {

    for (int i = 0; i< _snowflakes.size(); ++i) {
        _snowflakes[i]->draw();

        if (_snowflakes[i]->location().y() > _canvas->height())
            _snowflakes[i]->location().y(-30);
    }
}

void SnowflakeGenerator::move(float dt, float x, float y, unsigned long long timestamp) {
    for (int i = 0; i< _snowflakes.size(); ++i) {
        _snowflakes[i]->move(dt, x, y, timestamp);
    }
}
