
#include "../Math.h"

using namespace math;

/*
Matrix::Matrix(int n, int m) {
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


void Matrix::print() const {
    for (int y = 0; y < _nRows; ++y) {
        for (int x = 0; x < _nCols; ++x) {
            std::cout << " " << _matrix[x][y];
        }
        std::cout << std::endl;
    }
}


// GETTERS & SETTERS
int Matrix::get(int x, int y) const {return _matrix[x][y];}
int Matrix::nRows() const {return _nRows;}
int Matrix::nCols() const {return _nCols;}

void Matrix::set(int x, int y, int value) {_matrix[x][y] = value;}


// OPERATOR OVERLOADING
Matrix Matrix::operator*(Matrix const &m) {
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


// RULE OF THREE
Matrix& Matrix::operator=(const Matrix &src) {
    assert(_nRows == src._nRows);
    assert(_nCols == src._nCols);

    _nRows = src._nRows;
    _nCols = src._nCols;

    for (int i = 0; i < _nCols; ++i) {
        for (int j = 0; j < _nRows; ++j) {
            _matrix[i][j] = src._matrix[i][j];
        }
    }

    return *this;
}

Matrix::Matrix(const Matrix &src) {
    _nRows = src.nRows();
    _nCols = src.nCols();

    _matrix = new int*[_nCols];

    for (int i = 0; i < _nCols; ++i) {
        _matrix[i] = new int[_nRows];
        for (int j = 0; j < _nRows; ++j) {
            _matrix[i][j] = src._matrix[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < _nCols; ++i)
        delete[] _matrix[i];
    delete[] _matrix;
}
*/
