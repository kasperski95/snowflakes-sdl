#pragma once

class Matrix
{
public:
    Matrix(int n, int m);

    void print() const;

    // GETTERS & SETTERS
    int get(int x, int y) const;
    int nRows() const;
    int nCols() const;
    void set(int x, int y, int value);

    // OPERATOR OVERLOADING
    Matrix operator*(Matrix const &m);

    // RULE OF THREE
    Matrix& operator=(const Matrix &src);
    Matrix(const Matrix &src);
    ~Matrix();

protected:
    int **_matrix;
    int _nRows;
    int _nCols;
};
