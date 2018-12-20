#include "../Core.h"
#include <chrono>

using namespace gfx;

Engine::Engine(SDL_Surface* screenToSet, int widthToSet, int heightToSet, int fpsToSet) {
    SDL_ShowCursor(SDL_DISABLE);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, -1);

    _fps = fpsToSet;

    _canvas = new Canvas(screenToSet, widthToSet, heightToSet);
    // init primitives
}


void Engine::tick() {
    using namespace std::chrono;
    unsigned long long ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    _physxDeltaTime = ms - _physxTimeStamp;
    _physxTimeStamp = ms;
    _canvas->factor((float)_physxDeltaTime /_exposureTime);
    if (ms - _gfxTimeStamp > _exposureTime) {
        _canvas->update();
        _gfxTimeStamp = ms;
        _gfxDeltaTime = ms - _gfxTimeStamp;
    }

    //tick primitives
}

Canvas* Engine::canvas() {return _canvas;}
