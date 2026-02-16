#include "Triangle.h"

template <int n>
Triangle<n>::Triangle(const Vector<n>&, const Vector<n>&, const Vector<n>&) {}

template <int n>
Triangle<n>::Triangle(const Triangle<n>&) {}

template <int n>
Triangle<n>& Triangle<n>::operator*=(const Matrix<n, n>&) { return *this; }

template <int n>
Triangle<n>* Triangle<n>::operator*(const Matrix<n, n>&) const { return nullptr; }

template <int n>
float* Triangle<n>::getPoints() const { return nullptr; }

template <int n>
int Triangle<n>::getNumPoints() const { return 0; }
