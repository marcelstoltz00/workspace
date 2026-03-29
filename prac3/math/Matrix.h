#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <iomanip>

template <int n>
class Vector;

template <int n, int m>
class Matrix
{
protected:
    float **arr;

public:
    Matrix();
    Matrix(float **);
    Matrix(const Matrix<n, m> &);
    virtual ~Matrix();
    float *&operator[](int index) const
    {
        if (index >= n || index < 0)
        {
            throw "Invalid index";
        }

        return arr[index];
    }

    Matrix<n, m> &operator=(const Matrix<n, m> &);
    template <int a>
    Matrix<n, a> operator*(const Matrix<m, a>) const;
    Matrix<n, m> operator*(const float) const;
    Matrix<n, m> operator+(const Matrix<n, m>) const;
    Matrix<m, n> operator~() const;
    // --- Add these inside your Matrix class definition in Matrix.h ---

    static Matrix<4, 4> identity();

    static Matrix<4, 4> translate(float tx, float ty, float tz);

    static Matrix<4, 4> scale(float sx, float sy, float sz);

    static Matrix<4, 4> rotateX(float angle);
    static Matrix<4, 4> rotateY(float angle);
    static Matrix<4, 4> rotateZ(float angle);
    void print() const
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                std::cout << (*this)[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    int getM() const;
    int getN() const;

    float determinant() const;
};
#ifndef MATRIX_CPP_COMPILATION_UNIT
#include "Matrix.cpp"
#endif

#endif /*MATRIX_H*/