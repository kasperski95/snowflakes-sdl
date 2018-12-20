#include "../Objects.h"


Shape::Shape(gfx::Canvas* canvasToSet, std::initializer_list<Point> pointsToSet, int thicknessToSet, Color borderColorToSet)
    : Primitive (canvasToSet, thicknessToSet, borderColorToSet)
{
    _points.insert(_points.end(), pointsToSet.begin(), pointsToSet.end());
    assert(_points.size() > 2);

    for (int i = 0; i < _points.size() - 1; ++i) {
        _lines.push_back(new Line(_canvas, _points[i], _points[i+1], thicknessToSet, borderColorToSet));
        _lines[i]->parent(this);
    }
}

void Shape::draw() {
    fill(Color(255,0,0)); //TODO: fixme
    for (int i = 0; i < _lines.size(); ++i) {
        _lines[i]->draw();
    }

}

void Shape::fill(Color color) {
    _fillScanLine(color);
}

void Shape::move(Point p) { move(p.x(), p.y()); }
void Shape::move(int x, int y) {
    _translate(x + location().x(), y + location().y());
}

Shape::~Shape() {
    for (int i = 0; i < _lines.size(); ++i) { delete _lines[i];}
}



void Shape::_fillScanLine(Color fillColor) {
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
