#include "Matrix.h"

template<int n, int m>
Matrix<n, m>::Matrix() {}

template<int n, int m>
Matrix<n, m>::Matrix(float **) {}

template<int n, int m>
Matrix<n, m>::Matrix(const Matrix<n, m> &) {}

template<int n, int m>
Matrix<n, m>::~Matrix() {}

template<int n, int m>
Matrix<n, m> &Matrix<n, m>::operator=(const Matrix<n, m> &) { return *this; }

template<int n, int m>
template<int a>
Matrix<n, a> Matrix<n, m>::operator*(const Matrix<m, a>) const { return Matrix<n, a>(); }

template<int n, int m>
Matrix<n, m> Matrix<n, m>::operator*(const float) const { return Matrix<n, m>(); }

template<int n, int m>
Matrix<n, m> Matrix<n, m>::operator+(const Matrix<n, m>) const { return Matrix<n, m>(); }

template<int n, int m>
Matrix<m, n> Matrix<n, m>::operator~() const { return Matrix<m, n>(); }
