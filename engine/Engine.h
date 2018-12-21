#pragma once

#include "Core.h"
#include "Objects.h"

namespace gfx {
    class Canvas;

    class Engine {
    public:
        Engine(SDL_Surface* screen, int width, int height, int fps);
        void tick();
        void addTickingPrimitive(Primitive* primitive);
        void wind(float wind);
        Canvas* canvas();
    private:
        std::vector<Primitive*> _primitives;
        int _fps;
        float _wind;
        Canvas* _canvas;
        double _exposureTime;
        unsigned long long _physxDeltaTime;
        unsigned long long _physxTimeStamp;
        unsigned long long _gfxDeltaTime;
        unsigned long long _gfxTimeStamp;
    };
}
