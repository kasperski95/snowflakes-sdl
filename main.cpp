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
#define pi 3.14
#include <time.h>
#include <iostream>
#include <assert.h>
#include <vector>
using namespace std;

SDL_Surface *screen;
int width = 900;
int height = 600;
char const* tytul = "Snowflakes";

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);

//========================================================================

namespace gfx{
    class Canvas* canvas = nullptr;

    class Matrix {
    public:
        Matrix(int n, int m) {
            _nRows=m;
            _nCols=n;
            _matrix = new int*[_nCols];

            for (int i = 0; i < _nCols; ++i) {
                _matrix[i] = new int[_nRows];
                for (int j = 0; j < _nRows; ++j) {
                    if (i == j)
                        _matrix[i][j] = 1;
                    else
                        _matrix[i][j] = 0;
                }
            }
        }

        int get(int x, int y) const {
            return _matrix[x][y];
        }

        void set(int x, int y, int value) {
            _matrix[x][y] = value;
        }

        Matrix operator*(Matrix const &m) {
            assert(_nCols == m.nRows());
            Matrix result(m.nCols(), _nRows);

            for (int x = 0; x < result.nCols(); ++x) {
                for (int y = 0; y < result.nRows(); ++y) {
                    result.set(x, y, 0);
                    for (int i = 0; i < _nCols; ++i) {
                        result.set(x, y, result.get(x, y) + _matrix[i][y] * m.get(x, i) );
                    }
                }
            }
            return result;
        }


        void print() const {
            for (int y = 0; y < _nRows; ++y) {
                for (int x = 0; x < _nCols; ++x) {
                    cout << " " << _matrix[x][y];
                }
                cout << endl;
            }
        }

        int nRows() const { return _nRows; }
        int nCols() const { return _nCols; }

    protected:
        int **_matrix;
        int _nRows;
        int _nCols;
    };

    class Point : public Matrix {
    public:
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

    struct Color {
        Uint8 r;
        Uint8 g;
        Uint8 b;

        Color (Uint8 rToSet=0, Uint8 gToSet=0, Uint8 bToSet=0) {
            r = rToSet;
            g = gToSet;
            b = bToSet;
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


    class Canvas {
    public:
        Canvas(SDL_Surface* screenToSet, int widthToSet, int heightToSet) {
            _screen = screenToSet;
            _width = widthToSet;
            _height = heightToSet;

            _buffer = new Color**[_width];
            for(int x = 0; x < _width; ++x) {
                _buffer[x] = new Color*[_height];
                for (int y = 0; y < _height; ++y) {
                    _buffer[x][y] = new Color(0, 0, 0);
                }
            }
        }


        int width() { return _width; }


        int height() { return _height; }


        void px(Matrix m, Color color) {
            this->px(m.get(0,0), m.get(1,0), color.r, color.g, color.b);
        }

        void px(Point p, Color color) {
            this->px(p.x(), p.y(), color.r, color.g, color.b);
        }

        void px(int x, int y, Color color) {
            this->px(x, y, color.r, color.g, color.b);
        }

        void px(int x, int y, Uint8 rToSet, Uint8 gToSet, Uint8 bToSet) {
            assert (x >= 0);
            assert (y >= 0);
            assert (x < _width);
            assert (y < _height);
            if (_buffer) {
                _buffer[x][y]->r = rToSet;
                _buffer[x][y]->g = gToSet;
                _buffer[x][y]->b = bToSet;
            }
        }


        Color* px(int x, int y) {
            return new Color(getPixel(x,y).r, getPixel(x,y).g, getPixel(x,y).b);
        }


        void update() {
            for (int x = 0; x < _width; ++x) {
                for (int y = 0; y < _height; ++y) {
                    setPixel(x, y, _buffer[x][y]->r, _buffer[x][y]->g, _buffer[x][y]->b);
                }
            }
            SDL_Flip(_screen);
        }


        void refresh() {
            for(int x = 0; x < _width; ++x) {
                for (int y = 0; y < _height; ++y) {
                    _buffer[x][y]->r = getPixel(x,y).r;
                    _buffer[x][y]->g = getPixel(x,y).g;
                    _buffer[x][y]->b = getPixel(x,y).b;
                }
            }
        }


    private:
        SDL_Surface* _screen;
        int _width;
        int _height;
        Color*** _buffer;
    };


    class Primitive  {
    public:
        Primitive(Canvas* canvasToSet, int thicknessToSet, Color borderColorToSet)
            : _transformationMatrix(3,3)
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
        Matrix _transformationMatrix;

        void _translate(Matrix m) {
            this->_translate(m.get(0,0), m.get(1,0));
        }

        void _translate(int x, int y) {
            _transformationMatrix.set(0, 2, x);
            _transformationMatrix.set(1, 2, y);
        }

    };


    class Line : public Primitive {
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

            float a = 0.f;
            if (_endPoint.x() != 0)
                a = (float)_endPoint.y() / _endPoint.x();

            if (_endPoint.x() != 0 && abs(a) <= 1.0f ) {
                for (int x = 0; x <= _endPoint.x(); ++x) {
                    Point p(x, x * a);
                    _canvas->px(p * _transformationMatrix, _borderColor);
                }
            } else {
                a = (float)_endPoint.x() / _endPoint.y();
                for (int y = 0; y <= _endPoint.y(); ++y) {
                    Point p(y * a, y);
                    _canvas->px(p * _transformationMatrix, _borderColor);
                }
            }
        }

    private:
        Point _endPoint;
    };


    class Shape : public Primitive {
    public:
        Shape(Canvas* canvasToSet, std::initializer_list<Point> pointsToSet, int thicknessToSet=1, Color borderColorToSet=WHITE)
            : Primitive (canvasToSet, thicknessToSet, borderColorToSet)
        {
            _points.insert(_points.end(), pointsToSet.begin(), pointsToSet.end());
            assert(_points.size() > 2);

            for (int i = 0; i < _points.size() - 1; ++i) {
                _lines.push_back(new Line(_canvas, _points[i], _points[i+1]));
            }
        }

        void draw() override {
            for (int i = 0; i < _lines.size(); ++i) {
                _lines[i]->draw();
            }
        }



    private:
        vector<Point> _points;
        vector<Line*> _lines;
    };

}

//========================================================================

void Funkcja1() {
    using namespace gfx;

    Shape s1(canvas,{
             Point(100,100),
             Point(200,200),
             Point(300,150),
             Point(100,100)
             });
    s1.draw();


    canvas->update();
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
        gfx::canvas->refresh();
        SDL_FreeSurface(bmp);
    }

}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_Flip(screen);
    gfx::canvas->refresh();

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

    // initialize bridge between SDL and programmer
    gfx::canvas = new gfx::Canvas(screen, width, height);

    SDL_WM_SetCaption( tytul , NULL );
    // program main loop
    bool done = false;
    while (!done)
    {
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
