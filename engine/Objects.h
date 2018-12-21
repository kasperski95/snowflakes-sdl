#pragma once

#include <vector>
#include <cmath>
#include <assert.h>
#include <algorithm>

#include "core/Structures.h"
#include "Core.h"

/*
Color RED     (255, 0, 0);
Color GREEN   (0, 255, 0);
Color BLUE    (0, 0, 255);
Color CYAN    (0, 255, 255);
Color MAGENTA (255, 0, 255);
Color YELLOW  (255, 255, 0);
Color WHITE   (255, 255, 255);
Color BLACK   (0, 0, 0);
Color DEFAULT_COLOR = WHITE;
*/


using namespace gfx;

class Edge {
public:
    Edge(Point p1, Point p2);

    // GETTERS & SETTERS
    Point p1();
    Point p2();
    double a();
    double b();
    bool bVertical();

private:
    Point _p1;
    Point _p2;
    double _a;
    double _b;
    bool _bVertical;
    void _calculateSlope();
};


class Actor {
    public:
        Actor();

        // GETTERS & SETTERS
        Point location();
        math::Matrix<float> transformationMatrix();
        bool motionBlur();
        void parent(Actor* parentToSet);
        void location(float x, float y);
        void motionBlur(bool value);

    protected:
        math::Matrix<float> _transformationMatrix;
        class Actor* _parent;
        bool _motionBlur;
        void _translate(math::Matrix<float> m);
        void _translate(float x, float y);
    };



class Primitive : public Actor {
public:
    Primitive(Canvas* canvasToSet, int thicknessToSet, Color borderColorToSet);
    virtual void draw() = 0;
    virtual void move(float dt, float x=0, float y=0, unsigned long long timestamp=0);
    void momentum(float x, float y);
    Point momentum();

protected:
    Canvas* _canvas;
    Color _borderColor;
    int _thickness;
    Point _momentum;
};



class Line : public Primitive {
public:
    Line (Canvas* canvasToSet, Point p1, Point p2, int thicknessToSet, Color borderColorToSet);
    Line (Canvas* canvasToSet, int x1, int y1, int x2, int y2, int thicknessToSet, Color borderColorToSet);
    void draw() override;
    Point endPoint();

private:
    Point _endPoint;
};


class Circle : public Primitive {
public:
    Circle(gfx::Canvas* canvasToSet, int radius, int thicknessToSet, Color borderColorToSet);
    void draw() override;
    void fill(Color color);
private:
    float _radius;
};


class Shape : public Primitive {
public:
    Shape(gfx::Canvas* canvasToSet, std::initializer_list<Point> pointsToSet, int thicknessToSet, Color borderColorToSet);

    void draw() override;
    void fill(Color color);
    void move(Point p);
    void move(int x, int y);
    ~Shape();

private:
    std::vector<Point> _points;
    std::vector<Line*> _lines;
    void _fillScanLine(Color fillColor);
};
