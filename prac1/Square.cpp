#include "Square.h"

template <int n>
Square<n>::Square(const Vector<n>&, float, float) {}

template <int n>
Square<n>::Square(const Vector<n>&, const Vector<n>&, const Vector<n>&, const Vector<n>&) {}

template <int n>
Square<n>::Square(const Square<n>&) {}

template <int n>
Square<n>& Square<n>::operator*=(const Matrix<n, n>&) { return *this; }

template <int n>
Square<n>* Square<n>::operator*(const Matrix<n, n>&) const { return nullptr; }

template <int n>
float* Square<n>::getPoints() const { return nullptr; }

template <int n>
int Square<n>::getNumPoints() const { return 0; }
