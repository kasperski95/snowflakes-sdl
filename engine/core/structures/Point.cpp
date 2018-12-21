#include "../Structures.h"


Point::Point(Matrix matrix)
    : Point(matrix.get(0, 0), matrix.get(1, 0)) {}

Point::Point(float xToSet, float yToSet)
    : Matrix(3,1)
{
    _matrix[0][0] = xToSet;
    _matrix[1][0] = yToSet;
    _matrix[2][0] = 1;
}


void Point::swap() {
    float tmp = _matrix[0][0];
    _matrix[0][0] = _matrix[1][0];
    _matrix[1][0] = tmp;
}


// GETTERS & SETTERS
void Point::x(float xToSet) { _matrix[0][0] = xToSet; }
void Point::y(float yToSet) { _matrix[1][0] = yToSet; }
float Point::x() const { return _matrix[0][0]; }
float Point::y() const { return _matrix[1][0]; }


// OPERATORS
Point& Point::operator-=(const Point &rhs){
    this->_matrix[0][0] = rhs.get(0, 0);
    this->_matrix[1][0] = rhs.get(1, 0);
    return *this;
}
