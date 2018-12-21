#pragma once

#include "../engine/Core.h"
#include "../engine/Objects.h"
#include <vector>

class Snowflake : public Primitive {
public:
    Snowflake(gfx::Canvas* canvas, float size, float opacity, int distortionOffset=0);
    void draw() override;
    void move(float dt, float x, float y, unsigned long long timestamp) override;
private:
    float _size;
    float _opacity;
    float _weight;
    int _distortionOffset;
};



class SnowflakeGenerator : public Primitive {
public:
    SnowflakeGenerator(gfx::Canvas* canvas, int amount);
    void draw() override;
    void move(float dt, float x, float y, unsigned long long timestamp) override;
private:
    std::vector<Snowflake*> _snowflakes;
};
