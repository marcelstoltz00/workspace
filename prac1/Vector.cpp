#include "Vector.h"
#include <initializer_list>

template <int n>
Vector<n>::Vector() {}

template <int n>
Vector<n>::Vector(std::initializer_list<float>) {}

template <int n>
Vector<n>::Vector(float *) {}

template <int n>
Vector<n>::~Vector() {}

template <int n>
Vector<n>::Vector(const Vector<n> &) {}

template <int n>
Vector<n>::Vector(const Matrix<n, 1> &) {}

template <int n>
Vector<n> &Vector<n>::operator=(const Vector<n> &) { return *this; }

template <int n>
Vector<n> Vector<n>::operator+(const Vector<n>) const { return Vector<n>(); }

template <int n>
Vector<n> Vector<n>::operator-(const Vector<n>) const { return Vector<n>(); }

template <int n>
Vector<n> Vector<n>::operator*(const float) const { return Vector<n>(); }

template <int n>
float Vector<n>::operator*(const Vector<n>) const { return 0.0f; }

template <int n>
float Vector<n>::magnitude() const { return 0.0f; }

template <int n>
Vector<n>::operator Matrix<n, 1>() const { return Matrix<n, 1>(); }

template <>
Vector<3> Vector<3>::crossProduct(const Vector<3>) const { return Vector<3>(); }

template <int n>
Vector<n> Vector<n>::unitVector() const { return Vector<n>(); }

template <int n>
int Vector<n>::getN() const { return n; }