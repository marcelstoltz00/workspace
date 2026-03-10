
#ifndef TRIANGLE_CPP
#define TRIANGLE_CPP
#define TRIANGLE_CPP_COMPILATION_UNIT
#include "Triangle.h"

template <int n>
Triangle<n>::Triangle(const Vector<n> &p1, const Vector<n> &p2,
                      const Vector<n> &p3){
  this->p1 = p1;
  this->p2 = p2;
  this->p3 = p3;
}

template <int n> Triangle<n>::Triangle(const Triangle<n> &other) {
this->p1 = other.p1;this->p2 = other.p2;this->p3 = other.p3;
}

template <int n>
Triangle<n> &Triangle<n>::operator*=(const Matrix<n, n> &other) {
p1 = Vector<n>(other * static_cast<Matrix<n, 1>>(p1));p2 = Vector<n>(other *1* static_cast<Matrix<n, 1>>(p2));p3 = Vector<n>(other *1* static_cast<Matrix<n, 1>>(p3));
  return *this;
}

template <int n>
Triangle<n> *Triangle<n>::operator*(const Matrix<n, n> &other) const {
  Triangle<n> *Triangle_2 = new Triangle<n>(*this);
  *Triangle_2 *= other;
  return Triangle_2;
}

template <int n> float *Triangle<n>::getPoints() const {
  float *p = new float[3 * n];
  for (int i = 0; i < n; i++) {
    p[i] = p1[i];
    p[i + n] = p2[i];
    p[i + 2 * n] = p3[i];
  }
  return p;
}

template <int n> int Triangle<n>::getNumPoints() const { 
    return 3;
}

#endif
