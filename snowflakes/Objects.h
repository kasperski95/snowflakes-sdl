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
        Primitive(gfx::Canvas* canvasToSet, int thicknessToSet, Color borderColorToSet) {
            _canvas = canvasToSet;
            _thickness = thicknessToSet;
            _borderColor = borderColorToSet;
        }
        virtual void draw() = 0;

    protected:
        gfx::Canvas* _canvas;
        Color _borderColor;
        int _thickness;

    };




    class Line : public Primitive
    {
    public:
        Line (gfx::Canvas* canvasToSet, Point p1, Point p2, int thicknessToSet, Color borderColorToSet)
        : Line (canvasToSet, p1.x(), p1.y(), p2.x(), p2.y(), thicknessToSet, borderColorToSet) {}

        Line (gfx::Canvas* canvasToSet, int x1, int y1, int x2, int y2, int thicknessToSet, Color borderColorToSet) :
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
        Shape(gfx::Canvas* canvasToSet, std::initializer_list<Point> pointsToSet, int thicknessToSet, Color borderColorToSet)
            : Primitive (canvasToSet, thicknessToSet, borderColorToSet)
        {
            _points.insert(_points.end(), pointsToSet.begin(), pointsToSet.end());
            assert(_points.size() > 2);

            for (int i = 0; i < _points.size() - 1; ++i) {
                _lines.push_back(new Line(_canvas, _points[i], _points[i+1], thicknessToSet, borderColorToSet));
                _lines[i]->parent(this);
            }
        }

        void draw() override {
            fill(Color(255,0,0)); //TODO: fixme
            for (int i = 0; i < _lines.size(); ++i) {
                _lines[i]->draw();
            }

        }

        void fill(Color color) {
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
        std::vector<Point> _points;
        std::vector<Line*> _lines;


        void _fillScanLine(Color fillColor) {
            std::vector<Edge> edges;
            std::vector<Edge> edgeBuffer;
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
                std::vector<int> pointsInLine;
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
