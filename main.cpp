#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>

#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#include <cmath>
#define pi 3.14159265359
#include <ctime>
#include <iostream>

#include <assert.h>
#include <vector>
#include <chrono>
#include <algorithm>

#include "snowflakes/math/Math.h"

using namespace std;

SDL_Surface *screen;
int width = 800;
int height = 600;

char const* tytul = "Snowflakes by Arkadiusz Kasprzyk";

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);


void Funkcja1();
//========================================================================

namespace gfx{
    const int FPS = 30;
    class Canvas* canvas = nullptr;
    const double exposureTime = 1000.f / FPS;
    unsigned long long physicsDeltaTime = 0;
    unsigned long long physicsTimeStamp = 0;
    unsigned long long deltaTime = 0;
    unsigned long long timeStamp = 0;



    class Point : public math::Matrix {
    public:
        Point (Matrix matrix)
            : Point(matrix.get(0, 0), matrix.get(1, 0)) {}

        Point (int xToSet=0, int yToSet=0)
            : Matrix(3,1)
        {
            _matrix[0][0] = xToSet;
            _matrix[1][0] = yToSet;
            _matrix[2][0] = 1;
        }



        void swap() {
            int tmp = _matrix[0][0];
            _matrix[0][0] = _matrix[1][0];
            _matrix[1][0] = tmp;
        }

        void x(int xToSet) { _matrix[0][0] = xToSet; }
        void y(int yToSet) { _matrix[1][0] = yToSet; }

        int x() const { return _matrix[0][0]; }
        int y() const { return _matrix[1][0]; }

        Point& operator-=(const Point &rhs){
            this->_matrix[0][0] = rhs.get(0, 0);
            this->_matrix[1][0] = rhs.get(1, 0);
            return *this;
        }
    };


    class Edge {
    public:
        Edge(Point p1, Point p2) {
            _p1 = p1;
            _p2 = p2;
            _calculateSlope();
        }

        Point p1() { return _p1; }
        Point p2() { return _p2; }
        double a() { return _a; }
        double b() { return _b; }
        bool bVertical() { return _bVertical; }

    private:
        Point _p1;
        Point _p2;
        double _a;
        double _b;
        bool _bVertical;

        void _calculateSlope() {
            if (_p2.x() == _p1.x()) {
                _a = 0;
                _b = 0;
                _bVertical = true;
                return;
            }

            _a = (double)(_p2.y() - _p1.y()) / (_p2.x() - _p1.x());
            _b = _p1.y() - _a*_p1.x();
            _bVertical = false;
        }
    };



    struct Color {
        Uint16 r;
        Uint16 g;
        Uint16 b;

        Color (Uint16 rToSet=0, Uint16 gToSet=0, Uint16 bToSet=0) {
            r = rToSet;
            g = gToSet;
            b = bToSet;
        }

        Color& operator+=(const Color &rhs) {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            return *this;
        }

        Color& operator/=(const int &rhs) {
            r /= rhs;
            g /= rhs;
            b /= rhs;
            return *this;
        }
    };


    //-----------------------------------------------------

    Color RED     (255, 0, 0);
    Color GREEN   (0, 255, 0);
    Color BLUE    (0, 0, 255);
    Color CYAN    (0, 255, 255);
    Color MAGENTA (255, 0, 255);
    Color YELLOW  (255, 255, 0);
    Color WHITE   (255, 255, 255);
    Color BLACK   (0, 0, 0);
    Color DEFAULT_COLOR = WHITE;



    namespace io{
        Point mousePosition() {
            int x, y;
            SDL_GetMouseState(&x, &y);
            return Point(x, y);
        }
    }



    class Canvas {
    public:
        Canvas(SDL_Surface* screenToSet, int widthToSet, int heightToSet) {
            _screen = screenToSet;
            _width = widthToSet;
            _height = heightToSet;
            _subframes = 0;
        }


        int width() { return _width; }

        int height() { return _height; }

        void px(math::Matrix m, Color color, bool motionBlur=false) {
            this->px(m.get(0,0), m.get(1,0), color.r, color.g, color.b, motionBlur);
        }

        void px(Point p, Color color) {
            this->px(p.x(), p.y(), color.r, color.g, color.b);
        }

        void px(int x, int y, Color color, bool motionBlur=false) {
            this->px(x, y, color.r, color.g, color.b, motionBlur);
        }

        void px(int x, int y, Uint8 r, Uint8 g, Uint8 b, bool motionBlur=false) {
            //assert (x >= 0);
            //assert (y >= 0);
            //assert (x < _width);
            //assert (y < _height);
            x = math::clamp(x, 0, _width);
            y = math::clamp(y, 0, _height);


            if (motionBlur) {
                float factor = (float)physicsDeltaTime / exposureTime;
                Color pxCol = px(x, y);
                setPixel(x, y, math::clamp(pxCol.r + r * factor, 0, 255), math::clamp(pxCol.g + g * factor, 0, 255), math::clamp(pxCol.b + b * factor, 0, 255));
            } else {
                setPixel(x, y, math::clamp(r, 0, 255), math::clamp(g, 0, 255), math::clamp(b, 0, 255));
            }
        }


        Color px(int x, int y) {
            return Color(getPixel(x,y).r, getPixel(x,y).g, getPixel(x,y).b);
        }


        void update() {
            SDL_Flip(_screen);
            SDL_FillRect(screen, 0, SDL_MapRGB(_screen->format, 0, 0, 0));
        }


    private:
        SDL_Surface* _screen;
        int _width;
        int _height;
        int _subframes;
    };


    class Actor
    {
    public:
        Actor()
            : _transformationMatrix(3,3)
            {}

        void parent(Actor* parentToSet) {
            _parent = parentToSet;
        }

        void location(int x, int y) {
            _translate(x, y);
        }

        Point location() {
            return Point(_transformationMatrix.get(0,2), _transformationMatrix.get(1,2));
        }

        math::Matrix transformationMatrix() {
            math::Matrix tmpTransformationMatrix = _transformationMatrix;
            if (_parent) {
                tmpTransformationMatrix = _transformationMatrix * _parent->transformationMatrix();
            }
            return tmpTransformationMatrix;
        }

        void motionBlur(bool value) {
            _motionBlur = value;
        }

        bool motionBlur() {
            bool parentMotionBlur = false;
            if (_parent) {
                parentMotionBlur = _parent->motionBlur();
            }
            return parentMotionBlur || _motionBlur;
        }

    protected:
        math::Matrix _transformationMatrix;
        class Actor* _parent;
        bool _motionBlur;

        void _translate(math::Matrix m) {
            this->_translate(m.get(0,0), m.get(1,0));
        }

        void _translate(int x, int y) {
            _transformationMatrix.set(0, 2, x);
            _transformationMatrix.set(1, 2, y);
        }
    };


    class Primitive : public Actor
    {
    public:
        Primitive(Canvas* canvasToSet, int thicknessToSet, Color borderColorToSet)
        {
            _canvas = canvasToSet;
            _thickness = thicknessToSet;
            _borderColor = borderColorToSet;
        }
        virtual void draw() = 0;

    protected:
        Canvas* _canvas;
        Color _borderColor;
        int _thickness;

    };





    class Line : public Primitive
    {
    public:
        Line (Canvas* canvasToSet, Point p1, Point p2, int thicknessToSet=1, Color borderColorToSet=WHITE)
        : Line (canvasToSet, p1.x(), p1.y(), p2.x(), p2.y(), thicknessToSet, borderColorToSet) {}

        Line (Canvas* canvasToSet, int x1, int y1, int x2, int y2, int thicknessToSet=1, Color borderColorToSet=WHITE) :
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

        void draw() override {
            math::Matrix tmpTransformationMatrix = _transformationMatrix;

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

        Point endPoint() {
            math::Matrix tmp = _endPoint * transformationMatrix();
            return Point(tmp.get(0, 0), tmp.get(1, 0));
        }

    private:
        Point _endPoint;
    };




    class Shape : public Primitive
    {
    public:
        Shape(Canvas* canvasToSet, std::initializer_list<Point> pointsToSet, int thicknessToSet=1, Color borderColorToSet=WHITE)
            : Primitive (canvasToSet, thicknessToSet, borderColorToSet)
        {
            _points.insert(_points.end(), pointsToSet.begin(), pointsToSet.end());
            assert(_points.size() > 2);

            for (int i = 0; i < _points.size() - 1; ++i) {
                _lines.push_back(new Line(_canvas, _points[i], _points[i+1]));
                _lines[i]->parent(this);
            }
        }

        void draw() override {
            fill();
            for (int i = 0; i < _lines.size(); ++i) {
                _lines[i]->draw();
            }

        }

        void fill(Color color=RED) {
            _fillScanLine(color);
        }

        void move(Point p) { move(p.x(), p.y()); }
        void move(int x, int y) {
            _translate(x + location().x(), y + location().y());
        }

        ~Shape() {
            for (int i = 0; i < _lines.size(); ++i) { delete _lines[i];}
        }
    private:
        vector<Point> _points;
        vector<Line*> _lines;


        void _fillScanLine(Color fillColor) {
            vector<Edge> edges;
            vector<Edge> edgeBuffer;
            int x,y;



            for(int i = 0; i < _lines.size(); ++i) {
                if (_lines[i]->location().y() < _lines[i]->endPoint().y())
                    edges.push_back(Edge(_lines[i]->location() * transformationMatrix(), _lines[i]->endPoint()));
                else
                    edges.push_back(Edge(_lines[i]->endPoint(), _lines[i]->location() * transformationMatrix()));
            }

            if (edges.size() > 0) {
                y = edges[0].p1().y();
                x = edges[0].p1().x();
            }



            while (!edges.empty() || !edgeBuffer.empty()) {
                for(int i = 0; i < edges.size(); i++) {
                    // move from edges edge to edgeBuffer if edgePoint1.y == y
                    if (edges[i].p1().y() == y) {
                        edgeBuffer.push_back(edges[i]);
                        edges.erase(edges.begin() + i);
                        i--;
                    }
                }


                sort(edgeBuffer.begin(), edgeBuffer.end(), [](Edge e1, Edge e2) {
                    if (e1.p1().x() != e2.p1().x())
                        return e1.p1().x() < e2.p1().x();
                    else
                        return e1.p2().x() < e2.p2().x();
                });


                // loop through edgeBuffers
                vector<int> pointsInLine;
                for (int j = 0; j < edgeBuffer.size(); j++) {
                    // calculate x in respect to y
                    if (edgeBuffer[j].bVertical()) {
                        pointsInLine.push_back(edgeBuffer[j].p1().x());
                    } else if (edgeBuffer[j].a() != 0) {
                        x = round(double(y - edgeBuffer[j].b()) / edgeBuffer[j].a());
                        pointsInLine.push_back(x);
                    } else {
                        pointsInLine.push_back(edgeBuffer[j].p1().x());
                    }
                }

                for (int j = 0; j < pointsInLine.size() - 1; j += 2) {
                    if (pointsInLine[j] == pointsInLine[j+1]) {
                        j--;
                        continue;
                    }

                    for (int x = pointsInLine[j] + _thickness; x < pointsInLine[j+1]; ++x) {
                        _canvas->px(x, y, fillColor, motionBlur());
                    }
                }


                // remove edges from edgeBuffer if  y == edgePoint2.y
                for (int j = 0; j < edgeBuffer.size(); j++) {
                    if (edgeBuffer[j].p2().y() == y) {
                        edgeBuffer.erase(edgeBuffer.begin() + j);
                        j--;
                    }
                }
                y++;
            }
        }
    };

    vector<Primitive*> Primitives;
    void tick() {
        using namespace std::chrono;

        unsigned long long ms = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        ).count() % 2000;

        //Primitives[0]->location(io::mousePosition().x(), io::mousePosition().y());
        Primitives[0]->location(100, sin(ms/1000.f * pi) * 200 + 300);
        Primitives[0]->draw();
    }




    void init() {
        using namespace gfx;
        SDL_ShowCursor(SDL_DISABLE);
        SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, -1);



        Shape* s1 = new Shape(canvas,{
            Point(50,50),
            Point(50,-50),
            Point(-50,-50),
            Point(-50,50),
            Point(50,50)
        });



        s1->motionBlur(true);
        Primitives.push_back(s1);
        Primitives[0]->location(50, 50);
        Primitives[0]->draw();
    }






    void _tick() {
        using namespace std::chrono;
        unsigned long long ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        physicsDeltaTime = ms - physicsTimeStamp;
        physicsTimeStamp = ms;
        tick();
        if (ms - timeStamp > exposureTime) {
            canvas->update();
            timeStamp = ms;
            deltaTime = ms - timeStamp;
        }
    }


};

//========================================================================






void Funkcja1() {
    //TODO: funkcja 1


    //s1.draw();
}


void Funkcja2() {

    SDL_Flip(screen);
}


void Funkcja3() {

    SDL_Flip(screen);
}


void Funkcja4() {

    SDL_Flip(screen);
}


void Funkcja5() {


    SDL_Flip(screen);
}

void Funkcja6() {


    SDL_Flip(screen);
}


//========================================================================


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B) {
  if ((x>=0) && (x<width) && (y>=0) && (y<height))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru pixela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informacji ile bajtów zajmuje jeden pixel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres pixela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość pixela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;

    }
         /* update the screen (aka double buffering) */
  }
}


void ladujBMP(char const* nazwa, int x, int y)
{
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Rect dstrect;
        dstrect.x = x;
        dstrect.y = y;
        SDL_BlitSurface(bmp, 0, screen, &dstrect);
        SDL_Flip(screen);
        SDL_FreeSurface(bmp);
    }

}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_Flip(screen);

}




SDL_Color getPixel (int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<width) && (y>=0) && (y<height)) {
        //determine position
        char* pPosition=(char*)screen->pixels ;
        //offset by y
        pPosition+=(screen->pitch*y) ;
        //offset by x
        pPosition+=(screen->format->BytesPerPixel*x);
        //copy pixel data
        memcpy(&col, pPosition, screen->format->BytesPerPixel);
        //convert color
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}







int main ( int argc, char** argv )
{
    // console output
    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(width, height, 32,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }

    // initialize bridge
    gfx::canvas = new gfx::Canvas(screen, width, height);
    gfx::init();

    SDL_WM_SetCaption( tytul , NULL );
    // program main loop
    bool done = false;
    while (!done)
    {
        gfx::_tick();

        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    if (event.key.keysym.sym == SDLK_1)
                        Funkcja1();
                    if (event.key.keysym.sym == SDLK_2)
                        Funkcja2();
                    if (event.key.keysym.sym == SDLK_3)
                        Funkcja3();
                    if (event.key.keysym.sym == SDLK_4)
                        Funkcja4();
                    if (event.key.keysym.sym == SDLK_5)
                        Funkcja5();
                    if (event.key.keysym.sym == SDLK_6)
                        Funkcja6();
                    if (event.key.keysym.sym == SDLK_a)
                        ladujBMP("obrazek1.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_s)
                        ladujBMP("obrazek2.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_d)
                        ladujBMP("obrazek3.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_f)
                        ladujBMP("obrazek4.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_g)
                        ladujBMP("obrazek5.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_b)
                        czyscEkran(0, 0, 10);          break;
                     }
            } // end switch
        } // end of message processing

    } // end main loop


    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
