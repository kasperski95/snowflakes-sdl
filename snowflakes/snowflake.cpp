#include "Snowflakes.h"
#include "../engine/Math.h"


Snowflake::Snowflake(gfx::Canvas* canvasToSet, float opacityToSet) :
    Primitive (canvasToSet, 1, Color(255,255,0)) {
    _opacity = opacityToSet;
    _weight = _opacity * _opacity;
    this->motionBlur(true);
    this->location(50, 50);
    this->momentum(0,250);
}


void Snowflake::move(float dt, float x, float y, unsigned long long ms) {
    int frequency = 2000;
    float factor = (float)(ms % frequency) / frequency;
    float strength = 0.25;
    float noise = sin(factor * 2 * math::PI) * strength;
    Point tmpMomentum(_momentum.y() * noise, _momentum.y() * (1 - abs(noise)));
    location(location().x() + (x + tmpMomentum.x()) * dt, location().y() + (y + tmpMomentum.y()) * dt);
    _translate(location().x(), location().y());
}


void Snowflake::draw() {
    int x = (int)round(this->location().x());
    int y = (int)round(this->location().y());
    Color white(255,255,255);
    float opacity = _opacity;
    bool mb = this->motionBlur();

    _canvas->px(x+1, y-7, white, mb, (int)(opacity *48));

    _canvas->px(x, y-6, white, mb, (int)(opacity *65));
    _canvas->px(x+1, y-6, white, mb, (int)(opacity *131));

    _canvas->px(x-5, y-5, white, mb, (int)(opacity *49));
    _canvas->px(x-4, y-5, white, mb, (int)(opacity *32));
    _canvas->px(x-2, y-5, white, mb, (int)(opacity *45));
    _canvas->px(x-1, y-5, white, mb, (int)(opacity *58));
    _canvas->px(x, y-5, white, mb, (int)(opacity *125));
    _canvas->px(x+1, y-5, white, mb, (int)(opacity *137));
    _canvas->px(x+2, y-5, white, mb, (int)(opacity *38));

    _canvas->px(x-6, y-4, white, mb, (int)(opacity *49));
    _canvas->px(x-5, y-4, white, mb, (int)(opacity *175));
    _canvas->px(x-4, y-4, white, mb, (int)(opacity *105));
    _canvas->px(x-3, y-4, white, mb, (int)(opacity *134));
    _canvas->px(x-2, y-4, white, mb, (int)(opacity *57));
    _canvas->px(x-1, y-4, white, mb, (int)(opacity *138));
    _canvas->px(x, y-4, white, mb, (int)(opacity *222));
    _canvas->px(x+1, y-4, white, mb, (int)(opacity *182));
    _canvas->px(x+2, y-4, white, mb, (int)(opacity *98));
    _canvas->px(x+3, y-4, white, mb, (int)(opacity *30));

    _canvas->px(x-6, y-3, white, mb, (int)(opacity *27));
    _canvas->px(x-5, y-3, white, mb, (int)(opacity *159));
    _canvas->px(x-4, y-3, white, mb, (int)(opacity *231));
    _canvas->px(x-3, y-3, white, mb, (int)(opacity *184));
    _canvas->px(x-2, y-3, white, mb, (int)(opacity *80));
    _canvas->px(x-1, y-3, white, mb, (int)(opacity *92));
    _canvas->px(x, y-3, white, mb, (int)(opacity *231));
    _canvas->px(x+1, y-3, white, mb, (int)(opacity *148));
    _canvas->px(x+2, y-3, white, mb, (int)(opacity *55));
    _canvas->px(x+3, y-3, white, mb, (int)(opacity *54));
    _canvas->px(x+4, y-3, white, mb, (int)(opacity *35));

    _canvas->px(x-6, y-2, white, mb, (int)(opacity *54));
    _canvas->px(x-5, y-2, white, mb, (int)(opacity *162));
    _canvas->px(x-4, y-2, white, mb, (int)(opacity *232));
    _canvas->px(x-3, y-2, white, mb, (int)(opacity *254));
    _canvas->px(x-2, y-2, white, mb, (int)(opacity *178));
    _canvas->px(x-1, y-2, white, mb, (int)(opacity *158));
    _canvas->px(x, y-2, white, mb, (int)(opacity *249));
    _canvas->px(x+1, y-2, white, mb, (int)(opacity *125));
    _canvas->px(x+2, y-2, white, mb, (int)(opacity *99));
    _canvas->px(x+3, y-2, white, mb, (int)(opacity *128));
    _canvas->px(x+4, y-2, white, mb, (int)(opacity *106));
    _canvas->px(x+5, y-2, white, mb, (int)(opacity *91));
    _canvas->px(x+6, y-2, white, mb, (int)(opacity *68));
    _canvas->px(x+7, y-2, white, mb, (int)(opacity *19));

    _canvas->px(x-6, y-1, white, mb, (int)(opacity *49));
    _canvas->px(x-5, y-1, white, mb, (int)(opacity *77));
    _canvas->px(x-4, y-1, white, mb, (int)(opacity *120));
    _canvas->px(x-3, y-1, white, mb, (int)(opacity *209));
    _canvas->px(x-2, y-1, white, mb, (int)(opacity *255));
    _canvas->px(x-1, y-1, white, mb, (int)(opacity *224));
    _canvas->px(x, y-1, white, mb, (int)(opacity *230));
    _canvas->px(x+1, y-1, white, mb, (int)(opacity *158));
    _canvas->px(x+2, y-1, white, mb, (int)(opacity *197));
    _canvas->px(x+3, y-1, white, mb, (int)(opacity *212));
    _canvas->px(x+4, y-1, white, mb, (int)(opacity *188));
    _canvas->px(x+5, y-1, white, mb, (int)(opacity *125));
    _canvas->px(x+6, y-1, white, mb, (int)(opacity *54));

    _canvas->px(x-6, y, white, mb, (int)(opacity *90));
    _canvas->px(x-5, y, white, mb, (int)(opacity *96));
    _canvas->px(x-4, y, white, mb, (int)(opacity *107));
    _canvas->px(x-3, y, white, mb, (int)(opacity *145));
    _canvas->px(x-2, y, white, mb, (int)(opacity *224));
    _canvas->px(x-1, y, white, mb, (int)(opacity *255));
    _canvas->px(x, y, white, mb, (int)(opacity *255));
    _canvas->px(x+1, y, white, mb, (int)(opacity *241));
    _canvas->px(x+2, y, white, mb, (int)(opacity *205));
    _canvas->px(x+3, y, white, mb, (int)(opacity *152));
    _canvas->px(x+4, y, white, mb, (int)(opacity *139));
    _canvas->px(x+5, y, white, mb, (int)(opacity *50));

    _canvas->px(x-6, y+1, white, mb, (int)(opacity *115));
    _canvas->px(x-5, y+1, white, mb, (int)(opacity *214));
    _canvas->px(x-4, y+1, white, mb, (int)(opacity *234));
    _canvas->px(x-3, y+1, white, mb, (int)(opacity *255));
    _canvas->px(x-2, y+1, white, mb, (int)(opacity *253));
    _canvas->px(x-1, y+1, white, mb, (int)(opacity *255));
    _canvas->px(x, y+1, white, mb, (int)(opacity *255));
    _canvas->px(x+1, y+1, white, mb, (int)(opacity *189));
    _canvas->px(x+2, y+1, white, mb, (int)(opacity *113));
    _canvas->px(x+3, y+1, white, mb, (int)(opacity *68));
    _canvas->px(x+4, y+1, white, mb, (int)(opacity *64));
    _canvas->px(x+5, y+1, white, mb, (int)(opacity *41));

    _canvas->px(x-8, y+2, white, mb, (int)(opacity *34));
    _canvas->px(x-7, y+2, white, mb, (int)(opacity *143));
    _canvas->px(x-6, y+2, white, mb, (int)(opacity *195));
    _canvas->px(x-5, y+2, white, mb, (int)(opacity *231));
    _canvas->px(x-4, y+2, white, mb, (int)(opacity *203));
    _canvas->px(x-3, y+2, white, mb, (int)(opacity *150));
    _canvas->px(x-2, y+2, white, mb, (int)(opacity *171));
    _canvas->px(x-1, y+2, white, mb, (int)(opacity *244));
    _canvas->px(x, y+2, white, mb, (int)(opacity *183));
    _canvas->px(x+1, y+2, white, mb, (int)(opacity *246));
    _canvas->px(x+2, y+2, white, mb, (int)(opacity *175));
    _canvas->px(x+3, y+2, white, mb, (int)(opacity *101));
    _canvas->px(x+4, y+2, white, mb, (int)(opacity *92));
    _canvas->px(x+5, y+2, white, mb, (int)(opacity *43));

    _canvas->px(x-8, y+3, white, mb, (int)(opacity *27));
    _canvas->px(x-7, y+3, white, mb, (int)(opacity *73));
    _canvas->px(x-6, y+3, white, mb, (int)(opacity *88));
    _canvas->px(x-5, y+3, white, mb, (int)(opacity *142));
    _canvas->px(x-4, y+3, white, mb, (int)(opacity *106));
    _canvas->px(x-3, y+3, white, mb, (int)(opacity *91));
    _canvas->px(x-2, y+3, white, mb, (int)(opacity *193));
    _canvas->px(x-1, y+3, white, mb, (int)(opacity *249));
    _canvas->px(x, y+3, white, mb, (int)(opacity *128));
    _canvas->px(x+1, y+3, white, mb, (int)(opacity *164));
    _canvas->px(x+2, y+3, white, mb, (int)(opacity *224));
    _canvas->px(x+3, y+3, white, mb, (int)(opacity *200));
    _canvas->px(x+4, y+3, white, mb, (int)(opacity *102));
    _canvas->px(x+5, y+3, white, mb, (int)(opacity *24));

    _canvas->px(x-6, y+4, white, mb, (int)(opacity *9));
    _canvas->px(x-5, y+4, white, mb, (int)(opacity *62));
    _canvas->px(x-4, y+4, white, mb, (int)(opacity *50));
    _canvas->px(x-3, y+4, white, mb, (int)(opacity *118));
    _canvas->px(x-2, y+4, white, mb, (int)(opacity *209));
    _canvas->px(x-1, y+4, white, mb, (int)(opacity *213));
    _canvas->px(x, y+4, white, mb, (int)(opacity *88));
    _canvas->px(x+1, y+4, white, mb, (int)(opacity *98));
    _canvas->px(x+2, y+4, white, mb, (int)(opacity *166));
    _canvas->px(x+3, y+4, white, mb, (int)(opacity *196));
    _canvas->px(x+4, y+4, white, mb, (int)(opacity *140));
    _canvas->px(x+5, y+4, white, mb, (int)(opacity *31));

    _canvas->px(x-5, y+5, white, mb, (int)(opacity *8));
    _canvas->px(x-4, y+5, white, mb, (int)(opacity *48));
    _canvas->px(x-3, y+5, white, mb, (int)(opacity *122));
    _canvas->px(x-2, y+5, white, mb, (int)(opacity *212));
    _canvas->px(x-1, y+5, white, mb, (int)(opacity *178));
    _canvas->px(x, y+5, white, mb, (int)(opacity *82));
    _canvas->px(x+1, y+5, white, mb, (int)(opacity *48));
    _canvas->px(x+2, y+5, white, mb, (int)(opacity *83));
    _canvas->px(x+3, y+5, white, mb, (int)(opacity *98));
    _canvas->px(x+4, y+5, white, mb, (int)(opacity *132));
    _canvas->px(x+5, y+5, white, mb, (int)(opacity *54));

    _canvas->px(x-5, y+6, white, mb, (int)(opacity *5));
    _canvas->px(x-4, y+6, white, mb, (int)(opacity *75));
    _canvas->px(x-3, y+6, white, mb, (int)(opacity *182));
    _canvas->px(x-2, y+6, white, mb, (int)(opacity *82));
    _canvas->px(x-1, y+6, white, mb, (int)(opacity *31));
    _canvas->px(x, y+6, white, mb, (int)(opacity *8));
    _canvas->px(x+1, y+6, white, mb, (int)(opacity *12));
    _canvas->px(x+2, y+6, white, mb, (int)(opacity *20));
    _canvas->px(x+3, y+6, white, mb, (int)(opacity *16));
    _canvas->px(x+4, y+6, white, mb, (int)(opacity *15));

    _canvas->px(x-4, y+7, white, mb, (int)(opacity *42));
    _canvas->px(x-3, y+7, white, mb, (int)(opacity *121));
    _canvas->px(x-2, y+7, white, mb, (int)(opacity *24));

    _canvas->px(x-3, y+8, white, mb, (int)(opacity *32));
}
