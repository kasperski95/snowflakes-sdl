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
            x = (rand() % (int)(canvasToSet->width() * 3)) - canvasToSet->width();
            y = (rand() % (int)(canvasToSet->height() * 1.5)) - canvasToSet->height();
            _snowflakes[i] = new Snowflake(canvasToSet, 1.f, (rand()%100)/100.f, rand() % 1000);
            _snowflakes[i]->location(x, y);
        }
    }
}

void SnowflakeGenerator::draw() {
    int x;
    for (int i = 0; i< _snowflakes.size(); ++i) {
        _snowflakes[i]->draw();

        if (_snowflakes[i]->location().y() > _canvas->height()) {
            x = (rand() % (int)(_canvas->width() * 3)) - _canvas->width();
            _snowflakes[i]->location(x, -8);
        }
    }
}

void SnowflakeGenerator::move(float dt, float x, float y, unsigned long long timestamp) {
    for (int i = 0; i< _snowflakes.size(); ++i) {
        _snowflakes[i]->move(dt, x, y, timestamp);
    }
}
